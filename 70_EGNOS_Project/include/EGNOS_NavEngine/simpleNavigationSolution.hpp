#ifndef EGNOS_SIMPLE_NAVIGATION_SOLUTION_HPP
#define EGNOS_SIMPLE_NAVIGATION_SOLUTION_HPP

#include <Eigen/Dense>

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

// Class for handling RAIM
#include "PRSolution2.hpp"

// Class defining GPS system constants
#include "GNSSconstants.hpp"

#include "IonoCorrection.hpp"

namespace EGNOS {
	namespace EGNOS_UTILITY {

		using namespace std;
		using namespace gpstk;


#define EGNOS_UTILITY_CONVERGENCE_LIMIT 1E-5

		class SimpleNavSolver
		{
		public:

			SimpleNavSolver(void) {};
			~SimpleNavSolver(void) {};

			std::vector<double> SimpleNavSolver::get_Result(void);
			void print_Result(void);
			bool calculatePosition(gpstk::CommonTime &time, vector<SatID> &vid, vector<double> &prv, gpstk::TropModel *pTropModel, IonoModel *ionoModel);

			void set_bcestore(GPSEphemerisStore &in_bcestore);
			void setSimpleNaviagtionCalculator(gpstk::CommonTime &time, vector<SatID> &vid, vector<double> &prv, gpstk::TropModel *tropModel, IonoModel *ionoModel);

		private:
			IonoModel *pIonoModel = NULL;
			gpstk::TropModel *pTropModel = NULL;
			GPSEphemerisStore bcestore;
			gpstk::CommonTime gpsTime;

			vector<int> gpsSatIds;
			vector<double> gpsPrs;
			std::array<double, 4> roverPos = { 6300000,0,0,0 };


			static const double wie_e;
			static const double c_mps;

			double SimpleNavSolver::calculateTropoDelay(gpstk::CommonTime &time, gpstk::Xvt &SV, gpstk::Xvt  &RX);
			double SimpleNavSolver::calculateIonoDelay(gpstk::CommonTime &time, gpstk::Xvt &SV, gpstk::Xvt  &RX);

			gpstk::Xvt getSatXvt(gpstk::CommonTime &gpstime, int satId);

			void reset(void);
			double updatePosition(int iterNumber);
			void correctwSagnacEffect(double deltat, gpstk::Xvt &old_pos, gpstk::Xvt &new_pos);
			double calculateDistance(std::array<double, 4> &rover, gpstk::Xvt &sat);
			double Norm(Eigen::VectorXd x);
		};


	};
};

#endif