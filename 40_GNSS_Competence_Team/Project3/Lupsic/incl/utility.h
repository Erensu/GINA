#pragma once

// First, let's include Standard Template Library classes
#include <string>
#include <vector>

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

// Class for handling tropospheric models
#include "TropModel.hpp"

// Class for storing "broadcast-type" ephemerides
#include "GPSEphemerisStore.hpp"

// Class for handling RAIM
#include "PRSolution2.hpp"

// Class defining GPS system constants
#include "GNSSconstants.hpp"

using namespace std;
using namespace gpstk;

namespace utility_firstDGNSS_code {
	void set_bcestore(GPSEphemerisStore);

	void setDGNSSNaviagtionCalculator_Rover(CommonTime, vector<SatID>, vector<double>);

	void setDGNSSNaviagtionCalculator_Base(CommonTime, vector<SatID>, vector<double>, double[3]);

	int get_satPos(int, double, int, double*);

	int get_satPos(double[3], int, double, int, double*);

	int get_satClock(int, double, int, double&);

	int get_satRelCorr(int, double, int, double&);
}

namespace utility_secondDGNSS_code {
	void set_bcestore(GPSEphemerisStore);

	void setDGNSSNaviagtionCalculator_Rover(CommonTime, vector<SatID>, vector<double>);

	void setDGNSSNaviagtionCalculator_Base(CommonTime, vector<SatID>, vector<double>, double[3]);

	bool get_satPos(int, double, int, double[3]);

	bool get_satPos(double[3], int, double, int, double[3], double&);

	int get_satClock(int, double, int, double&);

	int get_satRelCorr(int, double, int, double&);
}