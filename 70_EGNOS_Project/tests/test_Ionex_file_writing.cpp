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

// Class to solve the equation system using Least Mean Squares
#include "SolverLMS.hpp"

// Class to solve the equation system using Weighted-Least Mean Squares
#include "SolverWMS.hpp"
#include "ComputeIonoModel.hpp"


double dummyTECProvider(double lat, double lon) {
	return 19;
}
double dummyTECRMSProvider(double lat, double lon) {
	return 0;
}

int main(int argc, char **argv) {

	gpstk::IonexStore ionoStore;

	gpstk::Position nominalPosition;
	nominalPosition.setGeodetic(44, 19, 100, new gpstk::WGS84Ellipsoid());

	std::string ionexFile_In = ROOT "\\70_EGNOS_Project\\files\\carg0310.18i";
	std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\test_out.18i";


	ionoStore.loadFile(ionexFile_In.c_str());

	gpstk::IonexStream strm_in(ionexFile_In.c_str(), std::ios::in);
	gpstk::IonexStream strm_out(ionexFile_Out.c_str(), std::ios::out);

	gpstk::IonexHeader header;
	gpstk::IonexHeader header_costume;
	header = ionoStore.getHeader(ionexFile_In);

	//ionoStore.dump(strm,1);

	//ionoStore.dump(std::cout, 1);

	gpstk::IonexData iod_in;
	gpstk::IonexData iod_out;

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
	header_costume.system = "GPS"; // TODO What shall be there instead of 'MIX'?
	header_costume.fileProgram = "GINA 1.0";
	header_costume.fileAgency = "GINA Project";
	header_costume.date = date;

	header_costume.descriptionList.clear();
	header_costume.descriptionList.push_back("Infrastructure: Robert Bosch Kft., Budapest");
	header_costume.descriptionList.push_back("Creator: Balazs Lupsic");
	header_costume.descriptionList.push_back("Contact adress: balazs.lupsic@hu.bosch.com");
	header_costume.descriptionList.push_back("Contact adress: balazs.lupsic@gmail.com");

	header_costume.commentList.push_back("TEC values in 0.1 [m] units;  9999, if no value available");

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

	header_costume.exponent = -1;
	header_costume.auxData = "";

	header_costume.svsmap.clear();
	header_costume.auxDataFlag = false;
	
	header_costume.valid = true;
	
	strm_out << header_costume;



	//while (strm_in >> iod_in && iod_in.isValid())
	{

		double lat1, lat2, dlat;
		double lon1, lon2, dlon;
		double hgt1, hgt2, dhgt;
		int dimlat, dimlon, dimheight;

		lat1 = 85;
		lat2 = -85;
		dlat = -15;

		lon1 = -180;
		lon2 = 180;
		dlon = 25;

		hgt1 = 300;
		hgt2 = 300;
		dhgt = 0;

		// TODO - make it nicer
		if (dlat == 0) {
			dimlat = 1;
		}
		else {
			dimlat = abs((int(lat2) - int(lat1)) / dlat) + 1;
		}

		if (dlon == 0) {
			dimlon = 1;
		}
		else {
			dimlon = abs((int(lon2) - int(lon1)) / dlon) + 1;
		}

		if (dhgt == 0) {
			dimheight = 1;
		}
		else {
			dimheight = 1;
		}
		
		

		
		double numberOfValues = dimlat * dimlon;

		gpstk::Vector<double> values(numberOfValues);
		int counter = 0;

		double currLat = lat1;
		double currLon = lon1;

		while (counter < numberOfValues)
		{
			cout << currLat <<" " << currLon << endl;
			
			
			values[counter] = dummyTECProvider(currLat, currLon);

			
			if (abs(currLon - lon2) < dlon) {
				currLon = lon1- dlon;
				currLat += dlat;
			}
			currLon += dlon;
			counter++;
		}

		iod_out.data = values;

		iod_out.mapID = 0;

		iod_out.dim[0] = dimlat;
		iod_out.dim[1] = dimlon;
		iod_out.dim[2] = dimheight;

		iod_out.exponent = 0;
		iod_out.lat[0] = lat1;
		iod_out.lat[1] = lat2;
		iod_out.lat[2] = dlat;

		iod_out.lon[0] = lon1;
		iod_out.lon[1] = lon2;
		iod_out.lon[2] = dlon;

		iod_out.hgt[0] = hgt1;
		iod_out.hgt[1] = hgt2;
		iod_out.hgt[2] = dhgt;

		iod_out.valid = true;

		iod_out.type.type = "TEC"; 
		iod_out.type.units = "meter"; 
		iod_out.type.description = "Total Electron Content map";

		iod_out.time = firstEpoch;

		strm_out << iod_out;

	}

	{

		double lat1, lat2, dlat;
		double lon1, lon2, dlon;
		double hgt1, hgt2, dhgt;
		int dimlat, dimlon, dimheight;

		lat1 = 85;
		lat2 = -85;
		dlat = -15;

		lon1 = -180;
		lon2 = 180;
		dlon = 25;

		hgt1 = 300;
		hgt2 = 300;
		dhgt = 0;

		// TODO - make it nicer
		if (dlat == 0) {
			dimlat = 1;
		}
		else {
			dimlat = abs((int(lat2) - int(lat1)) / dlat) + 1;
		}

		if (dlon == 0) {
			dimlon = 1;
		}
		else {
			dimlon = abs((int(lon2) - int(lon1)) / dlon) + 1;
		}

		if (dhgt == 0) {
			dimheight = 1;
		}
		else {
			dimheight = 1;
		}




		double numberOfValues = dimlat * dimlon;

		gpstk::Vector<double> values(numberOfValues);
		int counter = 0;

		double currLat = lat1;
		double currLon = lon1;

		while (counter < numberOfValues)
		{
			//cout << currLat << " " << currLon << endl;


			values[counter] = dummyTECProvider(currLat, currLon);


			if (abs(currLon - lon2) < dlon) {
				currLon = lon1 - dlon;
				currLat += dlat;
			}
			currLon += dlon;
			counter++;
		}

		iod_out.data = values;

		iod_out.mapID = 0;

		iod_out.dim[0] = dimlat;
		iod_out.dim[1] = dimlon;
		iod_out.dim[2] = dimheight;

		iod_out.exponent = 0;
		iod_out.lat[0] = lat1;
		iod_out.lat[1] = lat2;
		iod_out.lat[2] = dlat;

		iod_out.lon[0] = lon1;
		iod_out.lon[1] = lon2;
		iod_out.lon[2] = dlon;

		iod_out.hgt[0] = hgt1;
		iod_out.hgt[1] = hgt2;
		iod_out.hgt[2] = dhgt;

		iod_out.valid = true;

		iod_out.type.type = "RMS";
		iod_out.type.units = "meter";
		iod_out.type.description = "Total Electron Content map";

		iod_out.time = firstEpoch;
		
		strm_out << iod_out;

		/*iod_out.data = iod_in.data;

		iod_out.dim[0] = iod_in.dim[0];
		iod_out.dim[1] = iod_in.dim[1];
		iod_out.dim[2] = iod_in.dim[2];

		iod_out.exponent = iod_in.exponent;
		iod_out.lat[0] = iod_in.lat[0];
		iod_out.lat[1] = iod_in.lat[1];
		iod_out.lat[2] = iod_in.lat[2];

		iod_out.lon[0] = iod_in.lon[0];
		iod_out.lon[1] = iod_in.lon[1];
		iod_out.lon[2] = iod_in.lon[2];

		iod_out.hgt[0] = iod_in.hgt[0];
		iod_out.hgt[1] = iod_in.hgt[1];
		iod_out.hgt[2] = iod_in.hgt[2];

		iod_out.valid = iod_in.valid;

		iod_out.type = iod_in.type;

		iod_out.time = iod_in.time;*/

	}

	strm_in.close();
	strm_out.close();


	return 0;
}