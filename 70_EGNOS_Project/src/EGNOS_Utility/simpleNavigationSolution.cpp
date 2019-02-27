
#include "simpleNavigationSolution.hpp"


namespace EGNOS_UTILITY{

#pragma region Solver	

	const double SimpleNavSolver::wie_e = 7.292115e-5;
	const double SimpleNavSolver::c_mps = 299792458;

	std::vector<double> SimpleNavSolver::get_Result(void) {

		std::vector<double> sol;
		for (size_t i = 0; i < roverPos.size(); i++){

			sol.push_back(roverPos[i]);
		}

		return sol;
	}

	void SimpleNavSolver::print_Result(void) {

		std::cout << "" << roverPos[0] << " ";
		std::cout << " " << roverPos[1] << " ";
		std::cout << " " << roverPos[2] << "	";
		std::cout << "Clock bias: " << roverPos[3] << endl;
	}

	bool SimpleNavSolver::calculatePosition(gpstk::CommonTime &time, vector<SatID> &vid, vector<double> &prv) {

		int iterNumber = 0;
		bool validResult = true;
		setSimpleNaviagtionCalculator(time, vid, prv);

		while (updatePosition() > EGNOS_UTILITY_CONVERGENCE_LIMIT) {
			if (iterNumber > 25) {
				validResult = false;
				break;
			}
			iterNumber++;
		};
	
		return validResult;
	}

	double SimpleNavSolver::updatePosition(void) {

		double rho = 0;

		std::array<double, 3> satPos, temp_satPos;
		double temp_satClock, temp_satRelCorr;
		gpstk::CommonTime time_of_transmission, time_of_arrival;
		double travelTime = 0.0, travelTime_old = 0.0;
		double temp_dist;

		Eigen::MatrixXd covMatrix = Eigen::MatrixXd::Zero(4,4);
		Eigen::MatrixXd designMatrix(gpsSatIds.size(), 4);
		Eigen::VectorXd geometryDistance = Eigen::VectorXd::Zero(gpsSatIds.size());
		Eigen::VectorXd PRObservations = Eigen::VectorXd::Zero(gpsSatIds.size());
		Eigen::VectorXd y(gpsSatIds.size());
		Eigen::VectorXd x(4);

		for (int i = 0; i < gpsSatIds.size(); i++) {

			if (gpsSatIds[i] < 0) {
				continue;
			}

			// Get i. satelite position at given time
			if (get_satPos(this->gpsTime, gpsSatIds[i], satPos) == false) {
				continue;
			}

			PRObservations(i) = gpsPrs[i];

			// Calculate geometry distance from i. svh
			// Calculate normal vector to i. svh
			rho = 0;
			geometryDistance(i) = 0;

			// Correct with earth roation
			temp_satPos[0] = satPos[0];
			temp_satPos[1] = satPos[1];
			temp_satPos[2] = satPos[2];

			time_of_arrival = gpsTime;
			time_of_transmission = gpsTime;

			// We itarete once to get better satPos
				temp_dist = calculateDistance(roverPos, satPos);

				travelTime_old = travelTime;
				travelTime = temp_dist / c_mps;

				if (get_satPos(time_of_transmission, gpsSatIds[i], satPos) == false) {
					continue;
				}
			

				time_of_transmission = time_of_arrival - travelTime;				
				get_satRelCorr(time_of_transmission, gpsSatIds[i], temp_satRelCorr);
				get_satClock( time_of_transmission, gpsSatIds[i], temp_satClock);

				if (get_satPos(time_of_transmission, gpsSatIds[i], satPos) == false) {
					continue;
				}


			correctwSagnacEffect(time_of_transmission - time_of_arrival, satPos, temp_satPos);

			satPos[0] = temp_satPos[0];
			satPos[1] = temp_satPos[1];
			satPos[2] = temp_satPos[2];

			// Geometry distance
			geometryDistance(i) = calculateDistance(roverPos, satPos);

			// Correct the PR with clock correction
			get_satClock(time_of_transmission, gpsSatIds[i], temp_satClock);
			get_satRelCorr(time_of_transmission, gpsSatIds[i], temp_satRelCorr);
			PRObservations(i) += (temp_satClock + temp_satRelCorr) * c_mps;

			// Set up design matrix
			rho = calculateDistance(roverPos, satPos);

			designMatrix(i, 0) = (roverPos[0] - satPos[0]) / rho;
			designMatrix(i, 1) = (roverPos[1] - satPos[1]) / rho;
			designMatrix(i, 2) = (roverPos[2] - satPos[2]) / rho;
			designMatrix(i, 3) = 1.0;
		}

		// Create covariance matrix
		covMatrix = designMatrix.transpose() * designMatrix;
		covMatrix = covMatrix.inverse();

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

	void SimpleNavSolver::reset(void) {

		gpsSatIds.clear();
		gpsPrs.clear();
		roverPos.fill(0);
	}

	void SimpleNavSolver::correctwSagnacEffect(double deltat, std::array<double, 3> &old_pos, std::array<double, 3> &new_pos) {

		Eigen::Matrix3d rotMatrix = Eigen::Matrix3d::Zero();

		rotMatrix << cos(wie_e*deltat),  -sin(wie_e*deltat),		0,
						sin(wie_e*deltat),	 cos(wie_e*deltat),		0,
						0,					 0,						1;

		Eigen::Vector3d posV;
		posV << old_pos[0], old_pos[1], old_pos[2];

		posV = rotMatrix*posV;

		new_pos[0] = posV(0);
		new_pos[1] = posV(1);
		new_pos[2] = posV(2);

	}

	double SimpleNavSolver::calculateDistance(std::array<double, 4> &rover, std::array<double, 3> &sat) {

		double dist = 0;
		for (int a = 0; a < 3; a++) {
			dist += pow((rover[a] - sat[a]), 2);
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

	void setIonoModel(IonoModel* ionoModel) {

	};

	void SimpleNavSolver::setSimpleNaviagtionCalculator(gpstk::CommonTime &time, vector<SatID> &vid, vector<double> &prv) {

		this->reset();
	
		this->gpsTime = time;

		for (int i = 0; i < vid.size(); i++) { // TODO checker. size of vid and prv shall be the same
			if (vid[i].system == SatID::systemGPS) {
				if (vid[i].id > 0 & prv[i] > 1000) {
					gpsSatIds.push_back(vid[i].id);
					gpsPrs.push_back(prv[i]);
				}
			}
		}

	}

	bool SimpleNavSolver::get_satPos(gpstk::CommonTime &gpstime, int satId, std::array<double,3> &pos) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;


		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);
			pos[0] = xvt.x[0];
			pos[1] = xvt.x[1];
			pos[2] = xvt.x[2];

			return true;
		}
		catch (Exception& e) {

			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0;

			return false;
		}
	}

	bool SimpleNavSolver::get_satClock(gpstk::CommonTime &gpstime, int satId, double &clockbias) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);
			clockbias = xvt.getClockBias();

			return true;
		}
		catch (Exception& e) {

			//cerr << e << endl;

			return false;
		}

	}

	bool SimpleNavSolver::get_satRelCorr(gpstk::CommonTime &gpstime, int satId, double &relcorr) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);
			relcorr = xvt.relcorr;

			return true;
		}
		catch (Exception& e) {

			//cerr << e << endl;

			return false;
		}
	}

#pragma endregion

};