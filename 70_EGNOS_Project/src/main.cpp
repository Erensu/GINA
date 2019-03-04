
#include "Runnables.hpp"

using namespace std;

#include "IonexStream.hpp"
#include "IonexHeader.hpp"
#include "IonexData.hpp"
#include "IonexStore.hpp"

#include "RTKPOST_Data.hpp"
#include "RTKPOST_Stream.hpp"

int main(int argc, char **argv) {

	// Run RTKPOST parser
	//////////////////////////////////////////
	std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\bute0190_w_CODE.pos";

	//////////////////////////////////////////

	// Run EGNOSIonoCorrectionModel class and its features
	//////////////////////////////////////////

	/*std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15.ems";
	EGNOS::EGNOS_RUNNABLE::runEGNOSIonoCorrectionModel(EDAS_FileNamewPath);*/
	//////////////////////////////////////////

	// Run mainNavigationSolution
	//////////////////////////////////////////

	/*string obsFile = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\bute0190.19o";
	string ephFile = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\brdc0190.19n";
	string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136.ems";
	mainNavigationSolution(obsFile, ephFile, EDAS_FileNamewPath);*/
	//////////////////////////////////////////

	// This part is under construction
	//////////////////////////////////////////
	//td::string ionexFile1 = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\CODG0190.19I";
	//std::string ionexFile1 = ROOT "\\70_EGNOS_Project\\files\\EMS_124_2009_01_06_17\\h17ems_ionex_Europe_2.5x2.5_IGPMap.18i";
	//std::string ionexFile1 = ROOT "\\70_EGNOS_Project\\files\\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_5x5_IGPMap.19i";
	//std::string ionexFile2 = ROOT "\\70_EGNOS_Project\\files\\\EMS_136_2019_02_11_15\\Ionex_from_h15_Europe\\Grid5x5\\ionex_Europe00217.19i";
	
	/*std::string ionexFile1 = ROOT "\\70_EGNOS_Project\\files\\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_5x5_InterPol_All.19i";

	gpstk::IonexStore ionoStore1;
	gpstk::IonexStore ionoStore2;
	gpstk::IonexHeader ionoHeader1;

	ionoStore1.loadFile(ionexFile1);
	
	ionoHeader1 = ionoStore1.getHeader(ionexFile1);
	

	cout << gpstk::CivilTime(ionoHeader1.firstEpoch).year << " " << gpstk::CivilTime(ionoHeader1.firstEpoch).month << " " << gpstk::CivilTime(ionoHeader1.firstEpoch).day << " " << gpstk::CivilTime(ionoHeader1.firstEpoch).hour << " " << gpstk::CivilTime(ionoHeader1.firstEpoch).minute << " " << gpstk::CivilTime(ionoHeader1.firstEpoch).second << endl;
	
	gpstk::CommonTime initTime = ionoStore1.getInitialTime();
	gpstk::CommonTime finalTime = ionoStore1.getFinalTime();
	
	ionoHeader1.dump(cout);
	cout << endl;
	cout << gpstk::CivilTime(initTime).year << " " << gpstk::CivilTime(initTime).month << " " << gpstk::CivilTime(initTime).day << " " << gpstk::CivilTime(initTime).hour << " " << gpstk::CivilTime(initTime).minute << " " << gpstk::CivilTime(initTime).second << endl;
	cout << gpstk::CivilTime(finalTime).year << " " << gpstk::CivilTime(finalTime).month << " " << gpstk::CivilTime(finalTime).day << " " << gpstk::CivilTime(finalTime).hour << " " << gpstk::CivilTime(finalTime).minute << " " << gpstk::CivilTime(finalTime).second << endl;

	gpstk::Position RX;
	RX.setGeocentric(32.3, 25.4, 0);
	
	try
	{

		gpstk::Triple rtv = ionoStore1.getIonexValue(initTime, RX, 1);
		cout << rtv << endl;

	}
	catch (...)
	{
		std::cout << "ad" << std::endl;
	}*/
	

	//gpstk::IonexStream ionoStream;
	//gpstk::IonexHeader ionoHeader;
	//gpstk::IonexData ionoData;

	//ionoStream.open(ionexFile1.c_str(), std::ios::in);
	////ionoStream >> ionoHeader;

	//ionoStream >> ionoData;
	//ionoStream >> ionoData;

	//cout << "done" << endl;
	//////////////////////////////////////////



	// Compare Ionex Files
	//////////////////////////////////////////

	//std::string IonexFileNamewPath_1 = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\ionex_Europe00100.19i";
	//std::string IonexFileNamewPath_2 = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\ionex_Europe00200.19i";

	//std::string IonexFileNamewPath_1 = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_5x5_IGPMap_All.19i";
	//std::string IonexFileNamewPath_2 = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_5x5_IGPMap_All.19i";
										  //"\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\Ionex_from_h15_Europe\\Grid5x5\\ionex_files"
	//std::string IonexFileNamewPath_1 = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_All_with_fixedIntervals.19i";
	//std::string IonexFileNamewPath_2 = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\CODG0190.19I";
	//std::string IonexFileNamewPath_Out = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\TestIonexStoreOut.19i";
	
	/*
	EGNOS::EGNOS_RUNNABLE::compareIonexFiles(IonexFileNamewPath_1, IonexFileNamewPath_2, IonexFileNamewPath_Out);
	*/

	//////////////////////////////////////////

	// Process EMS file
	//////////////////////////////////////////

	//std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15.ems";
	//std::string Output_IonexFileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_2.5x2.5_IGPMap.19i";
	//std::string Output_IonexFileNamewPath_Detailed = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\Ionex_from_h15_Europe\\Grid2.5x2.5\\ionex_Europe.19i";
	
	//std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15.ems";
	//std::string Output_IonexFileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_1x1_IGPMap.19i";
	//std::string Output_IonexFileNamewPath_Detailed = "";//ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\Ionex_from_h15_Europe\\Grid1x1\\ionex_Europe.19i";

	//std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15.ems";
	//std::string Output_IonexFileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_World_5x5_IGPMap_All.19i";
	//std::string Output_IonexFileNamewPathLast = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_World_5x5_IGPMap_Last.19i";
	//std::string Output_IonexFileNamewPath_Detailed = "";//ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\Ionex_from_h17_Europe\\ionex_files\\ionex_Europe.19i";


	//std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136.ems";
	//std::string Output_IonexFileNamewPath = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_All_with_fixedIntervals.19i";
	//std::string Output_IonexFileNamewPathLast = ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_Last.19i";
	//std::string Output_IonexFileNamewPath_Detailed = "";//ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\Ionex_from_h17_Europe\\ionex_files\\ionex_Europe.19i";

	//EGNOS::EGNOSMapType mapType = EGNOS::europe5x5_tec; // world5x5_tec | europe5x5_tec | EGNOS::europe2_5x2_5_tec | EGNOS::europe1x1_tec

	/*EGNOS::EGNOS_RUNNABLE::processEMS(	EDAS_FileNamewPath,
										Output_IonexFileNamewPath,
										Output_IonexFileNamewPathLast,										
										Output_IonexFileNamewPath_Detailed,
										mapType,
										false,
										1800);*/

	//////////////////////////////////////////

	return 0;
}

