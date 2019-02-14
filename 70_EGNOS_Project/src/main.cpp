
#include "Runnables.hpp"

using namespace std;

#include "IonexStream.hpp"
#include "IonexHeader.hpp"
#include "IonexData.hpp"
#include "IonexStore.hpp"

int main(int argc, char **argv) {
	
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
	

	/*std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15.ems";
	std::string Output_IonexFileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_2.5x2.5_IGPMap.19i";
	std::string Output_IonexFileNamewPath_Detailed = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\Ionex_from_h15_Europe\\Grid2.5x2.5\\ionex_Europe.19i";
	*/

	//std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15.ems";
	//std::string Output_IonexFileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_1x1_IGPMap.19i";
	//std::string Output_IonexFileNamewPath_Detailed = "";//ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\Ionex_from_h15_Europe\\Grid1x1\\ionex_Europe.19i";

	std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15.ems";
	std::string Output_IonexFileNamewPath = "";//ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_5x5_IGPMap_All.19i";
	std::string Output_IonexFileNamewPathLast = ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\h15ems_ionex_Europe_5x5_IGPMap_Last.19i";
	std::string Output_IonexFileNamewPath_Detailed = "";//ROOT "\\70_EGNOS_Project\\files\\EMS_136_2019_02_11_15\\Ionex_from_h17_Europe\\ionex_files\\ionex_Europe.19i";
	
	EGNOS::EGNOSMapType mapType = EGNOS::europe5x5_tec;
	
	EGNOS::EGNOS_RUNNABLE::processEMS(	EDAS_FileNamewPath,
										Output_IonexFileNamewPath,
										Output_IonexFileNamewPathLast,										
										Output_IonexFileNamewPath_Detailed,
										mapType,
										false);
	

	//std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\EMS_124_2009_01_06_17\\h17.ems";
	//
	//EGNOS_EMS_Parser::EGNOS_EMS_Stream exampleStreamIn(EDAS_FileNamewPath.c_str());

	//EGNOS_EMS_Parser::EGNOS_EMS_Data EData;

	//EGNOS::IonosphericDelayCorrectionsMessageParser IonoGridPointParser;
	//EGNOS::IonosphericGridPointMasksMessageParser IonoMaskParser;

	//gpstk::CommonTime CurrentDataTime;

	//EGNOS::IGPMap IonoMap;
	//EGNOS::VerticalIonoDelayInterpolator egnosInterPol;
	//EGNOS::IonexCreator ionexWriter;
	//ionexWriter.headerType = EGNOS::europe5x5_tec; // europe5x5_tec | europe2_5x2_5_tec |europe1x1_tec
	//EGNOS::IGPMediator IgpMediator;

	//ionexWriter.setInterpolator(egnosInterPol);

	//bool weHad18 = false;
	//bool weHad26 = false;

	//int updateIndex = 0;
	//while (exampleStreamIn >> EData) {

	//	CurrentDataTime = EData.messageTime;

	//	if (EData.messageId == 18) {

	//		IonoMaskParser += EData.message;
	//		cout << IonoMaskParser << endl;
	//		weHad18 = true;
	//	}

	//	if (EData.messageId == 26) {

	//		IonoGridPointParser += EData.message;
	//		cout << IonoGridPointParser << endl;
	//		weHad26 = true;
	//	}

	//	if (weHad18 || weHad26) {

	//		bool newData = false;

	//		IgpMediator.updateTime(CurrentDataTime);
	//		IgpMediator.setIGPCandidates(IonoGridPointParser.getIonosphericGridPoint());
	//		IgpMediator.updateIGPCandidate(IonoMaskParser);
	//		
	//		newData = IonoMap.updateMap(IgpMediator);

	//		/*if (newData == true) {
	//			updateIndex++;
	//			string ems_out_file_index = createStrFileIndex(updateIndex);
	//			std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\EMS_124_2009_01_06_17\\Ionex_from_h17_Europe\\h17ems_ionex_Europe_" + ems_out_file_index + ".18i";
	//			std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\EMS_124_2009_01_06_17\\Ionex_from_h17_Europe_Interpolated\\h17ems_ionex_Europe_2.5_" + ems_out_file_index + ".18i";
	//			
	//			ionexWriter.setIonexData(IonoMap);
	//			ionexWriter.writeIGPMap2file(ionexFile_Out);
	//			cout << "IGP Map updated and file created" << endl;
	//		}*/
	//		cout << IonoMap;

	//		weHad26 = false;
	//		weHad18 = false;
	//	}
	//}

	//cout << IonoMap;
	//std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\EMS_124_2009_01_06_17\\h17ems_ionex_Europe_5x5_IGPMap.18i";
	//std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\EMS_124_2009_01_06_17\\h17ems_ionex_Europe_2.5x2.5_IGPMap.18i";
	//std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\EMS_124_2009_01_06_17\\h17ems_ionex_Europe_5x5_InterPol.18i";
	//std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\EMS_124_2009_01_06_17\\h17ems_ionex_Europe_2.5x2.5_InterPol.18i";
	//std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\EMS_124_2009_01_06_17\\h17ems_ionex_Europe_1x1_InterPol.18i";

	//ionexWriter.setIonexData(IonoMap);
	//ionexWriter.writeIGPMap2file(ionexFile_Out);

	//exampleStreamIn.close();

	return 0;
}

