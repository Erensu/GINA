#ifndef PROTECTION_LEVEL_MAIN_HPP
#define PROTECTION_LEVEL_MAIN_HPP

// Primary Math Library
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
#include "SaasTropModel.hpp"
#include "NeillTropModel.hpp"

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

// Protection Level
#include "ProtectionLevelEngine.hpp"
#include "ProtectionLevel_Data.hpp"
#include "ProtectionLevel_Stream.hpp"

// MOPS weigh calculation
#include "ComputeMOPSWeights.hpp"

// GPSTK Iono models
#include <ComputeIonoModel.hpp>

namespace EGNOS {
	namespace ProtectionLevel {

		typedef enum
		{
			egnos = 0,
			ionex = 1,
			zero
		}IonoType;

		void run_PL(std::string &ephData, 
					std::string& targetFile,
					std::string& referenceFile,
					std::string& PLwPath_out, 
					double elevationMask, 
					IonoType referenceIonoType, 
					IonoType targetIonoType, 
					bool useKlobucharasSecunderModel, 
					double latgeodetic, 
					double lon, double height, 
					int matchingIntervall_in_secs, 
					int timeIntervallofRefModellMap_in_secs,
					double probability_of_inner_circle);
	};
};

#endif