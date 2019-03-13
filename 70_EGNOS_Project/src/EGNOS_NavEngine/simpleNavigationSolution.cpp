
#include "simpleNavigationSolution.hpp"

namespace EGNOS {
	namespace EGNOS_UTILITY {

#pragma region Solver	

		const double SimpleNavSolver::wie_e = 7.292115e-5;
		const double SimpleNavSolver::c_mps = 299792458;

		std::vector<double> SimpleNavSolver::get_Result(void) {

			std::vector<double> sol;
			for (size_t i = 0; i < roverPos.size(); i++) {

				sol.push_back(roverPos[i]);
			}

			return sol;
		}

		void SimpleNavSolver::print_Result(void) {

			std::cerr << "" << roverPos[0] << " ";
			std::cerr << " " << roverPos[1] << " ";
			std::cerr << " " << roverPos[2] << "	";
			std::cerr << "Clock bias: " << roverPos[3] << endl;
		}

		double SimpleNavSolver::calculateTropoDelay(gpstk::CommonTime &time, gpstk::Xvt &SV, gpstk::Xvt  &RX) {

			// must test RX for reasonableness to avoid corrupting TropModel
			if (pTropModel == NULL) {
				throw domain_error("Tropo model ptr is NULL");
			}

			Position R(RX), S(SV);
			double tc = R.getHeight(), elev = R.elevationGeodetic(SV);

			if (elev < 0.0 || tc > 10000.0 || tc < -1000) {

				tc = 0.0;
			}
			else {

				tc = pTropModel->correction(R, S, time);
			}

			return tc;
		}

		double SimpleNavSolver::calculateIonoDelay(gpstk::CommonTime &time, gpstk::Xvt &SV, gpstk::Xvt  &RX) {

			if (pIonoModel == NULL) {
				throw domain_error("Iono model ptr is NULL");
			}

			gpstk::Position R(RX);
			gpstk::Position S(SV);

			double el = R.elevationGeodetic(S);
			double az = R.azimuthGeodetic(S);

			if (el < 0.0) {
				throw domain_error("Elevation is negative");
			}
			

			IonCorrandVar iCorrVar = {0};
			double ic = 0;
			try
			{
				iCorrVar = this->pIonoModel->getCorrection(time, RX, el, az);

				ic = iCorrVar.CorrinMeter;
			}
			catch (const std::exception&)
			{

			}
			
			return ic;
		}

		bool SimpleNavSolver::calculatePosition(gpstk::CommonTime &time, vector<SatID> &vid, vector<double> &prv, gpstk::TropModel *pTropModel, IonoModel *ionoModel) {

			int iterNumber = 0;
			bool validResult = true;
			setSimpleNaviagtionCalculator(time, vid, prv, pTropModel, ionoModel);


			double norm;
			// do at least twice so that
			// trop and iono model gets evaluated
			do {
				norm = updatePosition(iterNumber, false, false);
				applyElevationMask();
				if (norm < EGNOS_UTILITY_CONVERGENCE_LIMIT && iterNumber > 0) {
					break;
				}
				if (iterNumber > 25) {
					validResult = false;
					break;
				}

				iterNumber++;
			} while (1);

			checkElevation();
			return validResult;
		}

		void SimpleNavSolver::checkElevation(void) {
		
			gpstk::Xvt satPos;
			double elev = 0;
			for (size_t i = 0; i < gpsSatIds_original.size(); i++)
			{
				satPos = getSatXvt(gpsTime, gpsSatIds_original[i]);

				gpstk::Xvt RX;
				RX.x[0] = roverPos[0];
				RX.x[1] = roverPos[1];
				RX.x[2] = roverPos[2];

				Position R(RX), S(satPos);
				elev = R.elevationGeodetic(S);

				if (elev < elevationMask) {
					cerr << "Elevation angle: "<< elev << " Elevation mask: " << elevationMask << endl;
				}
			}
		}

		void SimpleNavSolver::applyElevationMask(void) {
		
			gpsSatIds.clear();
			gpsPrs.clear();

			gpstk::Xvt satPos;
			double elev = 0;
			for (size_t i = 0; i < gpsSatIds_original.size(); i++)
			{
				satPos = getSatXvt(gpsTime, gpsSatIds_original[i]);

				gpstk::Xvt RX;
				RX.x[0] = roverPos[0];
				RX.x[1] = roverPos[1];
				RX.x[2] = roverPos[2];

				Position R(RX), S(satPos);
				elev = R.elevation(S);

				if (elev >= elevationMask) {
					gpsSatIds.push_back(gpsSatIds_original[i]);
					gpsPrs.push_back(gpsPrs_original[i]);
				}
			}

		}

		double SimpleNavSolver::updatePosition(int iterNumber, bool tropoActive, bool ionoActive) {

			double rho = 0;

			gpstk::Xvt satPos, temp_satPos;
			double temp_satClock, temp_satRelCorr;
			gpstk::CommonTime time_of_transmission, time_of_arrival;
			double travelTime = 0.0, travelTime_old = 0.0;
			double temp_dist;

			Eigen::MatrixXd covMatrix = Eigen::MatrixXd::Zero(4, 4);
			Eigen::MatrixXd designMatrix(gpsSatIds.size(), 4);
			Eigen::VectorXd geometryDistance = Eigen::VectorXd::Zero(gpsSatIds.size());
			Eigen::VectorXd PRObservations = Eigen::VectorXd::Zero(gpsSatIds.size());
			Eigen::VectorXd y(gpsSatIds.size());
			Eigen::VectorXd x(4);

			for (int i = 0; i < gpsSatIds.size(); i++) {

				if (gpsSatIds[i] < 0) {
					continue;
				}

				try
				{
					satPos = getSatXvt(this->gpsTime, gpsSatIds[i]);
				}
				catch (const std::exception& e)
				{
					continue;
				}

				PRObservations(i) = gpsPrs[i];

				// Calculate geometry distance from i. svh
				// Calculate normal vector to i. svh
				rho = 0;
				geometryDistance(i) = 0;

				// Correct with earth roation
				temp_satPos = satPos;

				time_of_arrival = gpsTime;
				time_of_transmission = gpsTime;

				// We itarete once to get better satPos
				temp_dist = calculateDistance(roverPos, satPos);

				travelTime_old = travelTime;
				travelTime = temp_dist / c_mps;
				time_of_transmission = time_of_arrival - travelTime;

				try
				{
					satPos = getSatXvt(time_of_transmission, gpsSatIds[i]);
				}
				catch (const std::exception& e)
				{
					continue;
				}

				correctwSagnacEffect(time_of_transmission - time_of_arrival, satPos, temp_satPos);
				satPos = temp_satPos;

				gpstk::Xvt RX;
				RX.x[0] = roverPos[0];
				RX.x[1] = roverPos[1];
				RX.x[2] = roverPos[2];

				Position R(RX), S(satPos);
				double elev = R.elevationGeodetic(S);

				double tc = 0;
				try
				{
					if (tropoActive == true) {
						tc = calculateTropoDelay(gpsTime, satPos, RX);
					}
					else {
						tc = 0;
					}
					
				}
				catch (const std::exception&)
				{
					tc = 0;
				}

				double ic = 0;
				try
				{
					if (ionoActive == true) {
						ic = calculateIonoDelay(gpsTime, satPos, RX);
					}
					else {
						ic = 0;
					}
					
				}
				catch (const std::exception&)
				{
					ic = 0;
				}
				

				// Geometry distance
				geometryDistance(i) = calculateDistance(roverPos, satPos);

				// Correct the PR with clock correction
				PRObservations(i) += (satPos.clkbias + satPos.relcorr) * c_mps;

				// Correct the PR with tropo correction
				PRObservations(i) -= tc;

				// Correct the PR with iono correction
				PRObservations(i) -= ic;

				// Set up design matrix
				rho = calculateDistance(roverPos, satPos);

				designMatrix(i, 0) = (roverPos[0] - satPos.x[0]) / rho;
				designMatrix(i, 1) = (roverPos[1] - satPos.x[1]) / rho;
				designMatrix(i, 2) = (roverPos[2] - satPos.x[2]) / rho;
				designMatrix(i, 3) = 1.0;
			}

			// Create covariance matrix
			covMatrix = designMatrix.transpose() * designMatrix;
			covMatrix = covMatrix.inverse();

			try
			{
				updateDOP(covMatrix);
			}
			catch (...)
			{
				updateDOP();
			}
			

			// Set up observetion vector 
			// Multiply Covariance matrix with A.' and PR residual matrix
			y = PRObservations - geometryDistance;
			x = covMatrix * designMatrix.transpose() * y;

			// Update rover position and clock bias
			roverPos[0] += x(0);
			roverPos[1] += x(1);
			roverPos[2] += x(2);

			roverPos[3] = x(3);

			double res_norm = Norm(x);
			return res_norm;
		}

		void SimpleNavSolver::resetStartPosition(void) {

			roverPos[0] = 6300000;
			roverPos[1] = 0;
			roverPos[2] = 0;
			roverPos[3] = 0;
		}

		void SimpleNavSolver::reset(void) {

			gpsSatIds.clear();
			gpsPrs.clear();
			gpsSatIds_original.clear();
			gpsPrs_original.clear();
			pTropModel = NULL;
		}

		void SimpleNavSolver::correctwSagnacEffect(double deltat, gpstk::Xvt &old_pos, gpstk::Xvt &new_pos) {

			Eigen::Matrix3d rotMatrix = Eigen::Matrix3d::Zero();

			rotMatrix << cos(wie_e*deltat), -sin(wie_e*deltat), 0,
				sin(wie_e*deltat), cos(wie_e*deltat), 0,
				0, 0, 1;

			Eigen::Vector3d posV;
			posV << old_pos.x[0], old_pos.x[1], old_pos.x[2];

			posV = rotMatrix*posV;

			new_pos.x[0] = posV(0);
			new_pos.x[1] = posV(1);
			new_pos.x[2] = posV(2);

		}

		double SimpleNavSolver::calculateDistance(std::array<double, 4> &rover, gpstk::Xvt &sat) {

			double dist = 0;
			for (int a = 0; a < 3; a++) {
				dist += pow((rover[a] - sat.x[a]), 2);
			}

			return sqrt(dist);
		}


		double SimpleNavSolver::Norm(Eigen::VectorXd x) {

			return sqrt(x(0) * x(0) + x(1) * x(1) + x(2) * x(2));
		}

#pragma endregion

#pragma region Utility

		void SimpleNavSolver::set_bcestore(GPSEphemerisStore &in_bcestore) {

			bcestore = in_bcestore;
		}


		void SimpleNavSolver::setSimpleNaviagtionCalculator(gpstk::CommonTime &time, vector<SatID> &vid, vector<double> &prv, gpstk::TropModel *tropModel, IonoModel *ionoModel) {

			this->reset();

			this->pTropModel = tropModel;
			this->pIonoModel = ionoModel;
			this->gpsTime = time;

			for (int i = 0; i < vid.size(); i++) { // TODO checker. size of vid and prv shall be the same
				if (vid[i].system == SatID::systemGPS) {
					if (vid[i].id > 0 & prv[i] > 1000) {
						gpsSatIds_original.push_back(vid[i].id);
						gpsPrs_original.push_back(prv[i]);
					}
				}
			}

		}

		RTKPOST_Parser::RTKPOST_Pos_Data SimpleNavSolver::getRTKPOST_data(void) {

			RTKPOST_Parser::RTKPOST_Pos_Data data;
			data.age = 0;
			data.dataTime = gpsTime;
			data.numberOfSvId = gpsSatIds.size();
			data.ratio = 0;
			data.typeOfSolution = 5;		// 5 - single freq, basic solution
			data.sde = sqrt(Cov_enu(0,0));
			data.sdeu = Cov_enu(0, 2);
			data.sdn = sqrt(Cov_enu(1, 1));
			data.sdne = Cov_enu(0, 1);
			data.sdu = sqrt(Cov_enu(2, 2));
			data.sdun = Cov_enu(1, 2);
		
			gpstk::Position rovllh(roverPos[0], roverPos[1], roverPos[2],
									gpstk::Position::CoordinateSystem::Cartesian,
									NULL, ReferenceFrame::WGS84);
			data.pos = rovllh;
 
			return data;
		}

		gpstk::Xvt SimpleNavSolver::getSatXvt(gpstk::CommonTime &gpstime, int satId) {

			SatID id;
			id.id = satId;
			id.system = SatID::SatelliteSystem::systemGPS;

			Xvt xvt;

			try {

				xvt = bcestore.getXvt(id, gpstime);
				return xvt;
			}
			catch (Exception& e) {
				throw domain_error("Sat position cannot be calculated");
			}
		}

		Eigen::MatrixXd SimpleNavSolver::getECEF2ENUMatrix(double lat, double lon) {
		
			Eigen::MatrixXd ecef2enu = Eigen::MatrixXd(3, 3);

			double sLat = sin(lat);
			double cLat = cos(lat);
			double sLon = sin(lon);
			double cLon = cos(lon);

			ecef2enu <<		-sLat, -cLat*sLon,	cLat*cLon,
							 cLat, -sLat*sLon,	sLat*cLon,
							 0,		cLon,		sLon;

			return ecef2enu;
		}
		void SimpleNavSolver::updateDOP(void){	
			DOP_ecef = Eigen::MatrixXd::Identity(4, 4);
			Cov_ecef = Eigen::MatrixXd::Identity(3, 3);	
			Cov_enu = Eigen::MatrixXd::Identity(3, 3);	
		};

		void SimpleNavSolver::updateDOP(Eigen::MatrixXd& dop4x4) {

			gpstk::Position rovllh(roverPos[0], roverPos[1], roverPos[2],
				gpstk::Position::CoordinateSystem::Cartesian,  
				NULL, ReferenceFrame::WGS84);

			Eigen::MatrixXd ecef2enu = getECEF2ENUMatrix(rovllh.geodeticLatitude(), rovllh.longitude());
			
			DOP_ecef = dop4x4;
			Cov_ecef = dop4x4.block(0, 0, 3, 3);
			Cov_enu = ecef2enu*Cov_ecef*ecef2enu.transpose();
		}


		gpstk::Position SimpleNavSolver::transform2CommonPositionFormat(double x_ecef, double y_ecef, double z_ecef) {

			gpstk::Position pos(x_ecef,
				y_ecef,
				z_ecef,
				gpstk::Position::CoordinateSystem::Cartesian,
				NULL, ReferenceFrame::WGS84);

			return pos;

		}

		RTKPOST_Parser::RTKPOST_Pos_Data SimpleNavSolver::createRTKPOST_data(double x_ecef, double y_ecef, double z_ecef, int numberOfUsedSv, Eigen::MatrixXd &Cov_enu, gpstk::CommonTime &time) {

			RTKPOST_Parser::RTKPOST_Pos_Data data;

			gpstk::Position rovllh(x_ecef,
				y_ecef,
				z_ecef,
				gpstk::Position::CoordinateSystem::Cartesian,
				NULL, ReferenceFrame::WGS84);

			data.age = 0;
			data.dataTime = time;
			data.numberOfSvId = numberOfUsedSv;
			data.ratio = 0;
			data.typeOfSolution = 5;		// 5 - single freq, basic solution
			data.sde = sqrt(Cov_enu(0, 0));
			data.sdeu = Cov_enu(0, 2);
			data.sdn = sqrt(Cov_enu(1, 1));
			data.sdne = Cov_enu(0, 1);
			data.sdu = sqrt(Cov_enu(2, 2));
			data.sdun = Cov_enu(1, 2);

			data.pos = rovllh;

			return data;
		}

		Eigen::MatrixXd SimpleNavSolver::transformCovEcef2CovEnu(double lat, double lon, gpstk::Matrix<double> &cov_ecef) {
		
			Eigen::MatrixXd ecef2enu = getECEF2ENUMatrix(lat, lat);

			Eigen::MatrixXd Cov_enu = Eigen::MatrixXd(3, 3);
			Eigen::MatrixXd Cov_ecef = Eigen::MatrixXd(3, 3);

			for (size_t i = 0; i < 3; i++) {
				for (size_t j = 0; j < 3; j++) {
					Cov_ecef(i, j) = cov_ecef(i, j);
				}
			}

			Cov_enu = ecef2enu*Cov_ecef*ecef2enu.transpose();

			return Cov_enu;
		}


#pragma endregion

	};
};