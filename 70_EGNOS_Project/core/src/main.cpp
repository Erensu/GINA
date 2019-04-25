
#include "Runnables.hpp"

using namespace std;

#include "IonexStream.hpp"
#include "IonexHeader.hpp"
#include "IonexData.hpp"
#include "IonexStore.hpp"

#include "RTKPOST_Data.hpp"
#include "RTKPOST_Stream.hpp"

#include "ProtectionLevel_Main.hpp"
#include "ProtectionLevel_Data.hpp"
#include "ProtectionLevel_Stream.hpp"

int main(int argc, char **argv) {

	//ProtectionLevel Parser
	//////////////////////////////////////////

	string PL_FilewPath_In		= ROOT "\\70_EGNOS_Project\\core\\files\\DebugFiles\\Pos_lat49lon40h0_deltaT3600_el0_y2019_d019_136_brdc0190.pl";
	string PL_FilewPath_Out		= ROOT "\\70_EGNOS_Project\\core\\files\\DebugFiles\\Pos_lat49lon40h0_deltaT3600_el0_y2019_d019_136_brdc0190_Out.pl";
	string PL_FilewPath_Out2	= ROOT "\\70_EGNOS_Project\\core\\files\\DebugFiles\\Pos_lat49lon40h0_deltaT3600_el0_y2019_d019_136_brdc0190_Out_Out.pl";

	ProtectionLevel_Parser::ProtectionLevel_Stream pl_strm_in(PL_FilewPath_In.c_str(), std::ios::in);
	ProtectionLevel_Parser::ProtectionLevel_Stream pl_strm_in2(PL_FilewPath_Out.c_str(), std::ios::in);
	ProtectionLevel_Parser::ProtectionLevel_Stream pl_strm_out(PL_FilewPath_Out.c_str(), std::ios::out);
	ProtectionLevel_Parser::ProtectionLevel_Stream pl_strm_out2(PL_FilewPath_Out2.c_str(), std::ios::out);

	ProtectionLevel_Parser::ProtectionLevel_Data pl_data01;
	ProtectionLevel_Parser::ProtectionLevel_Data pl_data02;
	ProtectionLevel_Parser::ProtectionLevel_Data pl_data03;
	ProtectionLevel_Parser::ProtectionLevel_Data pl_data11;
	ProtectionLevel_Parser::ProtectionLevel_Data pl_data12;
	ProtectionLevel_Parser::ProtectionLevel_Data pl_data13;

	pl_strm_in >> pl_data01;
	pl_strm_in >> pl_data02;

	pl_strm_out << pl_data01;
	pl_strm_out << pl_data02;
	pl_strm_out << pl_data03;

	pl_strm_in2 >> pl_data11;
	pl_strm_in2 >> pl_data12;
	pl_strm_in2 >> pl_data13;

	pl_strm_out2 << pl_data11;
	pl_strm_out2 << pl_data12;
	pl_strm_out2 << pl_data13;

	pl_strm_in.close();
	pl_strm_out.close();
	pl_strm_in2.close();
	pl_strm_out2.close();

	std::cout << "Job is finished" << std::endl;
	//////////////////////////////////////////

	//ProtectionLevel Calculation
	//////////////////////////////////////////

	/*
	string ephFile = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\brdc0190.19n";
	string ionexFile = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\CODG0190.19I";
	string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136.ems";
	string PLwPath_out = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136_brdc0190.pl";
	double elevationMask = 10;
	EGNOS::ProtectionLevel::IonoType ionoType = EGNOS::ProtectionLevel::IonoType::egnos;

	double latgeodetic = 45;
	double lon = 15;
	double height = 0;
	int intervallBetweenEpochsinSecs = 3600;
	double probability_of_inner_circle = 0.99;

	EGNOS::ProtectionLevel::run_PL(ephFile, ionexFile, EDAS_FileNamewPath, PLwPath_out, elevationMask, ionoType, latgeodetic, lon, height, intervallBetweenEpochsinSecs, probability_of_inner_circle);
	*/
	//////////////////////////////////////////

	// Run RTKPOST parser
	//////////////////////////////////////////
	/*std::string EDAS_FileNamewPath_in		= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\bute0190_w_CODE.pos";
	std::string EDAS_FileNamewPath_out		= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\bute0190_w_CODE_out.pos";
	std::string EDAS_FileNamewPath_out_out	= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\bute0190_w_CODE_out_out.pos";
	
	EGNOS::EGNOS_RUNNABLE::runRTKPOSTParser(EDAS_FileNamewPath_in, EDAS_FileNamewPath_out, EDAS_FileNamewPath_out_out);
	*/
	//////////////////////////////////////////

	// Run EGNOSIonoCorrectionModel class and its features
	//////////////////////////////////////////

	/*std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15.ems";
	EGNOS::EGNOS_RUNNABLE::runEGNOSIonoCorrectionModel(EDAS_FileNamewPath);*/
	//////////////////////////////////////////

	// Run mainNavigationSolution
	//////////////////////////////////////////

	/*string obsFile					= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\bute0190.19o";
	string ephFile					= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\brdc0190.19n";
	string ionexFile				= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\CODG0190.19I";
	string EDAS_FileNamewPath		= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136.ems";
	string RTKPOST_out_gpstk		= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\bute0190_GPSTK_EGNOSIono_NoTropo_elev10_out.pos";
	string RTKPOST_out_navEngine	= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\bute0190_GINA_EGNOSIono_NoTropo_elev10_out.pos";
	string error_log				= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\ErrorLog.txt";
	double elevationMask			= 10;
	IonoType ionoType				= IonoType::egnos;

	mainNavigationSolution(	obsFile, 
							ephFile, 
							ionexFile, 
							EDAS_FileNamewPath, 
							RTKPOST_out_gpstk, 
							RTKPOST_out_navEngine, 
							error_log, 
							elevationMask, 
							ionoType);*/

	//////////////////////////////////////////

	// Compare Ionex Files
	//////////////////////////////////////////

	//std::string IonexFileNamewPath_1		= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\ionex_Europe00100.19i";
	//std::string IonexFileNamewPath_2		= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\ionex_Europe00200.19i";

	//std::string IonexFileNamewPath_1		= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_5x5_IGPMap_All.19i";
	//std::string IonexFileNamewPath_2		= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_5x5_IGPMap_All.19i";
		
	//std::string IonexFileNamewPath_1		= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\CODG0190.19I";
	//std::string IonexFileNamewPath_2		= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_All_with_fixedIntervals.19i";
	//std::string IonexFileNamewPath_Out	= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\Difference_IONEX_CODG0190-y2019_d019_136ems_ionex_Europe_5x5_IGPMap_All_with_fixedIntervals.19i";
	
	//EGNOS::EGNOS_RUNNABLE::compareIonexFiles(	IonexFileNamewPath_1, 
	//											IonexFileNamewPath_2, 
	//											IonexFileNamewPath_Out,
	//											60);
	
	//////////////////////////////////////////

	// Process EMS file
	//////////////////////////////////////////

	//std::string EDAS_FileNamewPath					= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15.ems";
	//std::string Output_IonexFileNamewPath				= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_2.5x2.5_IGPMap.19i";
	//std::string Output_IonexFileNamewPath_Detailed	= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\Ionex_from_h15_Europe\\Grid2.5x2.5\\ionex_Europe.19i";
	
	//std::string EDAS_FileNamewPath					= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15.ems";
	//std::string Output_IonexFileNamewPath				= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_1x1_IGPMap.19i";
	//std::string Output_IonexFileNamewPath_Detailed	= "";//ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\Ionex_from_h15_Europe\\Grid1x1\\ionex_Europe.19i";

	//std::string EDAS_FileNamewPath					= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15.ems";
	//std::string Output_IonexFileNamewPath				= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_World_5x5_IGPMap_All.19i";
	//std::string Output_IonexFileNamewPathLast			= ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_World_5x5_IGPMap_Last.19i";
	//std::string Output_IonexFileNamewPath_Detailed	= "";//ROOT "\\70_EGNOS_Project\\core\\files\\EMS_136_2019_02_11_15\\Ionex_from_h17_Europe\\ionex_files\\ionex_Europe.19i";


	/*std::string EDAS_FileNamewPath					= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136.ems";
    std::string Output_IonexFileNamewPath			= ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_All_with_fixedIntervals.19i";
	std::string Output_IonexFileNamewPathLast		= ""; //ROOT "\\70_EGNOS_Project\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_Last.19i";
	std::string Output_IonexFileNamewPath_Detailed	= "";

	EGNOS::EGNOSMapType mapType = EGNOS::europe5x5_tec; // world5x5_tec | europe5x5_tec | EGNOS::europe2_5x2_5_tec | EGNOS::europe1x1_tec

	gpstk::CivilTime firstUpdate;
	gpstk::CivilTime lastUpdate;

	EGNOS::EGNOS_RUNNABLE::processEMS(	EDAS_FileNamewPath,
										Output_IonexFileNamewPath,
										Output_IonexFileNamewPathLast,										
										Output_IonexFileNamewPath_Detailed,
										mapType,
										false,
										firstUpdate,
										lastUpdate,
										3600);
	*/
	//////////////////////////////////////////

	return 0;
}

