#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "PRSolution.hpp"

#include "EGNOS_EMS_Data.hpp"
#include "EGNOS_EMS_Stream.hpp"

#include "IGPMap.hpp"
#include "IonosphericMaskBand.hpp"
#include "IonosphericGridPoint.hpp"

#include "GINAConfig.h"
#include "CommonTime.hpp"
#include "CivilTime.hpp"

#include "IonexStream.hpp"

#include <ctime>  

using namespace std;


int main(int argc, char **argv) {


	std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\Test_Ionex_Header_out.18i";
	gpstk::IonexStream strm_out(ionexFile_Out.c_str(), std::ios::out);

	gpstk::IonexHeader header_costume;


	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	std::string date(buffer);

	gpstk::CivilTime firstEpoch;
	firstEpoch.year = 2019;
	firstEpoch.month = 1;
	firstEpoch.day = 22;
	firstEpoch.hour = 20;
	firstEpoch.minute = 57;
	firstEpoch.second = 15;

	header_costume.version = 1.0;
	header_costume.fileType = "IONOSPHERE MAPS";
	header_costume.system = "GPS";
	header_costume.fileProgram = "GINA 1.0";
	header_costume.fileAgency = "GINA Project";
	header_costume.date = date;

	header_costume.descriptionList.clear();
	header_costume.descriptionList.push_back("Infrastructure: Robert Bosch Kft., Budapest");
	header_costume.descriptionList.push_back("Creator: Balazs Lupsic");
	header_costume.descriptionList.push_back("Contact adress: balazs.lupsic@hu.bosch.com");
	header_costume.descriptionList.push_back("Contact adress: balazs.lupsic@gmail.com");

	header_costume.commentList.push_back("TEC values in [m] units;  9999, if no value available");

	header_costume.firstEpoch = firstEpoch;
	header_costume.lastEpoch = firstEpoch;
	header_costume.interval = 0;
	header_costume.numMaps = 1;
	header_costume.mappingFunction = "TBD";
	header_costume.elevation = 0;
	header_costume.observablesUsed = "TEC is obtained from EGNOS";
	header_costume.numStations = 0;
	header_costume.numSVs = 0;
	header_costume.baseRadius = 6371.4;
	header_costume.mapDims = 2;

	header_costume.hgt[0] = 300;
	header_costume.hgt[1] = 300;
	header_costume.hgt[2] = 0;

	header_costume.lat[0] = 85;
	header_costume.lat[1] = -85;
	header_costume.lat[2] = 5;

	header_costume.lon[0] = -180;
	header_costume.lon[1] = 180;
	header_costume.lon[2] = 5;

	header_costume.exponent = 0;
	header_costume.auxData = "";

	header_costume.svsmap.clear();
	header_costume.auxDataFlag = false;
	
	header_costume.valid = true;
	
	strm_out << header_costume;

	strm_out.close();

	return 0;
}