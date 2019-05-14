#ifndef EGNOS_RUNNABLES_HPP
#define EGNOS_RUNNABLES_HPP

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "statslib.hpp"

#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "PRSolution.hpp"

#include "EGNOS_EMS_Data.hpp"
#include "EGNOS_EMS_Stream.hpp"

#include "IGPMap.hpp"
#include "IonosphericMaskBand.hpp"
#include "IonosphericGridPoint.hpp"

#include "GINAConfig.h"

#include "IonexStore.hpp"
#include "IonexData.hpp"

#include "IGPMap.hpp"
#include "IonoCorrection.hpp"
#include "EGNOS_Ionex_Converter.hpp"

#include "navMain.hpp"

using namespace std;
namespace EGNOS 
{
	namespace EGNOS_RUNNABLE
	{
		void runRTKPOSTParser(std::string &EDAS_FileNamewPath_in, std::string &EDAS_FileNamewPath_out, std::string &EDAS_FileNamewPath_out_out);

		void runEGNOSIonoCorrectionModel(std::string EDAS_FileNamewPath);

		void compareIonexFiles(	std::string& ReferenceIonexFileNamewPath,
								std::string& TargetIonexFileNamewPath,
								std::string& IonexFileNamewPath_Out,
								double matchingIntervall = 100);

		void processEMS(std::string EDAS_FileNamewPath = "",
			std::string Output_IonexFileNamewPath = "",
			std::string Output_IonexFileNamewPathLast = "",
			std::string Output_IonexFileNamewPath_Detailed = "",
			EGNOSMapType mapType = EGNOS::europe5x5_tec,
			bool interPolationOn = true,
			gpstk::CommonTime firstUpdate = gpstk::CommonTime(),
			gpstk::CommonTime lastUpdate = gpstk::CommonTime(),
			double updateIntervalinSeconds = 0,
			bool fullHourStart = false);
	};
		
	namespace EGNOS_RUNNABLE_UTILITY
	{

		std::vector<gpstk::IonexData> createDifferenceDataBlock(gpstk::IonexHeader &header,
																gpstk::IonexStore &refreceStore,
																gpstk::IonexStore &targetStore,
																gpstk::CommonTime &epoch,
																int mapID);

		string createStrFileIndex(int index);

		bool checkArgs(std::string EDAS_FileNamewPath,
			std::string Output_IonexFileNamewPath,
			std::string Output_IonexFileNamewPathLast,
			std::string Output_IonexFileNamewPath_Detailed,
			EGNOS::EGNOSMapType mapType,
			bool interPolationOn,
			unsigned int updateIntervalinSeconds);

		double calcMaximumLikeliHood(double mean1, double mean2, double std1, double std2);
		double calcChi2Probability(double mean1, double std1, double mean2, double std2);
		double probChi2_2D(double x);

		std::string EGNOSMapType2String(EGNOS::EGNOSMapType mapType);
		int calcDim(int lat1, int lat2, double dlat);
	};

};

#endif