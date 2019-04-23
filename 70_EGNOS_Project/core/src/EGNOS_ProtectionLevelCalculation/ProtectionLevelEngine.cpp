#include "ProtectionLevelEngine.hpp"



namespace EGNOS {
	namespace ProtectionLevel {

		double EGNOS_PL::calculatePL(gpstk::CommonTime &time, gpstk::Position &Rover, vector<gpstk::SatID> &prnVec, Eigen::MatrixXd &CovMatrix, double probabilityThatWeAreinTheCircle) {

			setParameters(time, Rover, prnVec, CovMatrix);
			calcDesignMatrix();
			calcCovarianceMatrix();
			calcHorizontalCovariance();
			calcEigenValues();
			calcScaleFactor2Gaussian2d(probabilityThatWeAreinTheCircle);

			return K*semiMajorAxis;
		}
		void EGNOS_PL::calcScaleFactor2Gaussian2d(double probabilityThatWeAreinTheCircle) {
		
			K = std::sqrt(-2*std::log( 1- probabilityThatWeAreinTheCircle));
		}

		void EGNOS_PL::calcEigenValues(void) {
		
			typedef typename Eigen::Matrix2d::Index Index;
			typedef typename Eigen::Matrix2d::Scalar Scalar;
			typedef std::complex<Scalar> ComplexScalar;
			typedef Eigen::Matrix<ComplexScalar, 2, 1> VectorType;

			VectorType eigenV = horizontalCov.eigenvalues();

			if (eigenV(0).real() > eigenV(1).real()) {
				semiMajorAxis = std::sqrt(eigenV(0).real());
			}
			else {
				semiMajorAxis = std::sqrt(eigenV(1).real());
			}
		}

		void EGNOS_PL::calcHorizontalCovariance(void) {

			Eigen::MatrixXd ecef2enu = getECEF2ENUMatrix(Rover.geodeticLatitude() * M_PI / 180 , Rover.longitude() * M_PI / 180 );
			horizontalCov = PosCovMatrix.block(0, 0, 2, 2);
			Eigen::Matrix3d	PosCovMatrix_from_ecef = ecef2enu*PosCovMatrix_ecef.block(0, 0, 3, 3)*ecef2enu.transpose();
			horizontalCov_from_ecef = PosCovMatrix_from_ecef.block(0,0,2,2);
		}

		void EGNOS_PL::calcCovarianceMatrix(void) {
		
			Eigen::MatrixXd S		=	(designMatrix_from_ElAz	.transpose()	* WeightMatrix * designMatrix_from_ElAz	).inverse() * designMatrix_from_ElAz.transpose()	* WeightMatrix;
			Eigen::MatrixXd S_ecef	=	(designMatrix_ecef		.transpose()	* WeightMatrix * designMatrix_ecef		).inverse() * designMatrix_ecef		.transpose()	* WeightMatrix;

			PosCovMatrix		= S			* MesCovMatrix * S		.transpose();
			PosCovMatrix_ecef	= S_ecef	* MesCovMatrix * S_ecef	.transpose();
		}

		void EGNOS_PL::calcDesignMatrix(void)	{

			Eigen::MatrixXd designMatrix_ecef(prnVec.size(), 4);
			Eigen::MatrixXd designMatrix_from_ElAz_local(prnVec.size(), 4);
			gpstk::Xvt satPos;
			double rho = 1;
			double el;
			double az;

			int NumberOfCalculatedSats = 0;

			for (int i = 0; i < prnVec.size(); i++)	{

				try
				{
					satPos = getSatXvt(time, prnVec[i]);
					NumberOfCalculatedSats++;
				}
				catch (const std::exception& e)
				{
					continue;
				}

				gpstk::Xvt RX;
				RX.x[0] = Rover.X();
				RX.x[1] = Rover.Y();
				RX.x[2] = Rover.Z();

				gpstk::Position S(satPos);
				gpstk::Position R(Rover);

				rho = calculateDistance(Rover, S);
				el = R.elevationGeodetic(S);
				az = R.azimuthGeodetic(S);

				Eigen::MatrixXd ecef2enu = getECEF2ENUMatrix(Rover.geodeticLatitude() * M_PI / 180, Rover.longitude() * M_PI / 180);

				designMatrix_from_ElAz_local(i, 0) = -cos(el * M_PI / 180)*sin(az * M_PI / 180);
				designMatrix_from_ElAz_local(i, 1) = -cos(el * M_PI / 180)*cos(az * M_PI / 180);
				designMatrix_from_ElAz_local(i, 2) = -sin(el * M_PI / 180);
				designMatrix_from_ElAz_local(i, 3) = 1;

				designMatrix_ecef(i, 0) = (Rover.X() - S.X()) / rho;
				designMatrix_ecef(i, 1) = (Rover.Y() - S.Y()) / rho;
				designMatrix_ecef(i, 2) = (Rover.Z() - S.Z()) / rho;
				designMatrix_ecef(i, 3) = 1;
			}

			if (NumberOfCalculatedSats != prnVec.size()) {

				throw domain_error("NumberOfCalculatedSats != prnVec.size()");
			}
			else {
				this->designMatrix_from_ElAz = designMatrix_from_ElAz_local;
				this->designMatrix_ecef = designMatrix_ecef;
			}
		}

		Eigen::MatrixXd EGNOS_PL::getECEF2ENUMatrix(double lat, double lon)	 {

			Eigen::MatrixXd ecef2enu = Eigen::MatrixXd(3, 3);

			double sLat = sin(lat);
			double cLat = cos(lat);
			double sLon = sin(lon);
			double cLon = cos(lon);

			ecef2enu << -sLat, cLat, 0,
						-cLat*sLon, -sLat*sLon, cLon,
						cLat*cLon, sLat*cLon, sLon;
			return ecef2enu;
		}

		double EGNOS_PL::calculateDistance(gpstk::Position &rover, gpstk::Position &sat) {

			double dist = 0;
			
			dist += pow((rover.X() - sat.X()), 2);
			dist += pow((rover.Y() - sat.Y()), 2);
			dist += pow((rover.Z() - sat.Z()), 2);

			return sqrt(dist);
		}

		void EGNOS_PL::setParameters(gpstk::CommonTime &time, gpstk::Position &Rover, vector<gpstk::SatID> &prnVec, Eigen::MatrixXd &MesCovMatrix) {
			
			this->Rover = Rover; 
			this->prnVec = prnVec; 
			this->time = time; 
			this->MesCovMatrix = MesCovMatrix;
			this->WeightMatrix = MesCovMatrix.inverse();
		};

		gpstk::Xvt EGNOS_PL::getSatXvt(gpstk::CommonTime &time, gpstk::SatID &id) {

		
			gpstk::Xvt xvt;

			try {

				switch (id.system)
				{
				case gpstk::SatID::SatelliteSystem::systemGPS:

					xvt = bcestoreGps.getXvt(id, time);
					break;

				case gpstk::SatID::SatelliteSystem::systemGalileo:
					
					xvt = bcestoreGal.getXvt(id, time);
					break;

				case gpstk::SatID::SatelliteSystem::systemGlonass:
					xvt = bcestoreGlo.getXvt(id, time);
					break;

				default:
					throw domain_error("Sat position cannot be calculated bc sat system not defined");
					break;
				}
				if (id.system == gpstk::SatID::SatelliteSystem::systemGPS) {
					xvt = bcestoreGps.getXvt(id, time);
				}

				return xvt;
			}
			catch (gpstk::Exception& e) {
				throw domain_error("Sat position cannot be calculated");
			}
		}

	};
};