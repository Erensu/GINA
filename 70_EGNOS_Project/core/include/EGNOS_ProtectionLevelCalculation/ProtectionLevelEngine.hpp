#ifndef PROTECTION_LEVEL_ENGINE_HPP
#define PROTECTION_LEVEL_ENGINE_HPP

#define STATS_DONT_USE_OPENMP
#define STATS_USE_EIGEN

// Math Library
#include <Eigen/Dense>
//#include "statslib.hpp"

// First, let's include Standard Template Library classes
#include <string>
#include <vector>
#include <array>

// Classes for handling observations RINEX files (data)
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsData.hpp"
#include "Rinex3ObsStream.hpp"

// Classes for handling satellite navigation parameters RINEX
// files (ephemerides)
#include "Rinex3NavHeader.hpp"
#include "Rinex3NavData.hpp"
#include "Rinex3NavStream.hpp"

// Classes for handling RINEX files with meteorological parameters
#include "RinexMetBase.hpp"
#include "RinexMetData.hpp"
#include "RinexMetHeader.hpp"
#include "RinexMetStream.hpp"

// Class for handling tropospheric model
#include "GGTropModel.hpp"

// Class for storing "broadcast-type" ephemerides
#include "GPSEphemerisStore.hpp"
#include "GloEphemerisStore.hpp"
#include "GalEphemerisStore.hpp"

// Class for handling RAIM
#include "PRSolution2.hpp"

// Class defining GPS system constants
#include "GNSSconstants.hpp"

// EGNOS Iono Model
#include "IonoCorrection.hpp"

// RTKPOS interface
#include "RTKPOST_Data.hpp"

// GPSTK Iono models
#include <ComputeIonoModel.hpp>

namespace EGNOS {
	namespace ProtectionLevel {

		class EGNOS_PL	{

			public:
				
				EGNOS_PL(	gpstk::GPSEphemerisStore &bcestoreGps, 
							gpstk::GalEphemerisStore &bcestoreGal, 
							gpstk::GloEphemerisStore &bcestoreGlo ):
																		bcestoreGps(bcestoreGps), 
																		bcestoreGal(bcestoreGal) , 
																		bcestoreGlo(bcestoreGlo) {};
				~EGNOS_PL() {};

				gpstk::Position Rover;
				vector<gpstk::SatID> prnVec;
				gpstk::CommonTime time;

				Eigen::VectorXd prError;
				Eigen::MatrixXd designMatrix_ecef;
				Eigen::MatrixXd designMatrix_from_ElAz;
				Eigen::MatrixXd MesCovMatrix;
				Eigen::MatrixXd WeightMatrix;
				Eigen::MatrixXd PosCovMatrix;
				Eigen::MatrixXd PosCovMatrix_ecef;
				Eigen::Matrix2d	horizontalCov;
				Eigen::Matrix2d	horizontalCov_from_ecef;

				double semiMajorAxis;
				double K;

				double calculatePL(gpstk::CommonTime &time, gpstk::Position &Rover, vector<gpstk::SatID> &prnVec, Eigen::MatrixXd &WeightMatrix, double probabilityThatWeAreinTheCircle);
				double calculatePL(gpstk::CommonTime &time, gpstk::Position &Rover, vector<gpstk::SatID> &prnVec, Eigen::MatrixXd &CovMatrix, Eigen::MatrixXd &WeightMatrix, double probabilityThatWeAreinTheCircle);
				
				Eigen::Vector3d calculatePositionError_in_enu(gpstk::CommonTime &time, gpstk::Position &Rover, vector<gpstk::SatID> &prnVec, Eigen::VectorXd &prError, Eigen::MatrixXd &WeightMatrix);
				
				gpstk::GPSEphemerisStore bcestoreGps;
				gpstk::GalEphemerisStore bcestoreGal;
				gpstk::GloEphemerisStore bcestoreGlo;

			private:

				void setParameters(gpstk::CommonTime &time, gpstk::Position &Rover, vector<gpstk::SatID> &prnVec);
				void setParameters(gpstk::CommonTime &time, gpstk::Position &Rover, vector<gpstk::SatID> &prnVec, Eigen::MatrixXd &MesCovMatrix);
				void setParameters(gpstk::CommonTime &time, gpstk::Position &Rover, vector<gpstk::SatID> &prnVec, Eigen::MatrixXd &MesCovMatrix, Eigen::MatrixXd &WeightMatrix);
				gpstk::Xvt getSatXvt(gpstk::CommonTime &time, gpstk::SatID &id);
				void calcDesignMatrix(void);
				void calcCovarianceMatrix(void);
				void calcHorizontalCovariance(void);
				void calcEigenValues(void);
				void calcScaleFactor2Gaussian2d(double probabilityThatWeAreinTheCircle);

				Eigen::MatrixXd getECEF2ENUMatrix(double lat, double lon);
				double calculateDistance(gpstk::Position &rover, gpstk::Position &sat);
		};

	};
};

#endif