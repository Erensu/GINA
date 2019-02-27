#pragma once

#include <Eigen/Dense>

// First, let's include Standard Template Library classes
#include <string>
#include <vector>
#include <array>

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

// Class for handling tropospheric models
#include "TropModel.hpp"

// Class for storing "broadcast-type" ephemerides
#include "GPSEphemerisStore.hpp"

// Class for handling RAIM
#include "PRSolution2.hpp"

// Class defining GPS system constants
#include "GNSSconstants.hpp"

namespace EGNOS_UTILITY {

	using namespace std;
	using namespace gpstk;


#define EGNOS_UTILITY_CONVERGENCE_LIMIT 1E-5

	class IonoModel {
	public:

		virtual ~IonoModel() {};
		virtual double getIonoDelay(gpstk::CommonTime &epoch, gpstk::Position RX, double elevation) const = 0;
		virtual double clone(void) const = 0;
	};

	class SimpleNavSolver
	{
	public:

		SimpleNavSolver(void) {};
		~SimpleNavSolver(void) {};

		std::vector<double> SimpleNavSolver::get_Result(void);
		void print_Result(void);
		bool calculatePosition(gpstk::CommonTime &time, vector<SatID> &vid, vector<double> &prv);

		void set_bcestore(GPSEphemerisStore &in_bcestore);
		void setSimpleNaviagtionCalculator(gpstk::CommonTime &time, vector<SatID> &vid, vector<double> &prv);
		
	private:
		GPSEphemerisStore bcestore;
		gpstk::CommonTime gpsTime;

		vector<int> gpsSatIds;
		vector<double> gpsPrs;
		std::array<double, 4> roverPos = {0,0,0,0};
		

		static const double wie_e;
		static const double c_mps;

		bool get_satPos(gpstk::CommonTime &gpstime, int satId, std::array<double, 3> &pos);
		bool get_satClock(gpstk::CommonTime &gpstime, int satId, double &clockbias);
		bool get_satRelCorr(gpstk::CommonTime &gpstime, int satId, double &relcorr);

		void reset(void);
		double updatePosition(void);
		void correctwSagnacEffect(double deltat, std::array<double, 3> &old_pos, std::array<double, 3> &new_pos);
		double calculateDistance(std::array<double, 4> &rover, std::array<double, 3> &sat);
		double Norm(Eigen::VectorXd x);
	};

	
};