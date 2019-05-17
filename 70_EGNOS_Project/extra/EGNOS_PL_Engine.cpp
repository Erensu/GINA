
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

	std::cout << "" << std::endl;
	std::cout << "WARNING: Vertical Protection calculation is not embedded to the PL engine. " << std::endl;
	std::cout << "Only calculates the VPL for 3 sigma, so the INTEGRITY PROBABITY is  always 99,73 % for the Vertical Protection Level." << std::endl;
	std::cout << "" << std::endl;

	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";
	
	if (argc < 14 || argc > 16) {

		std::cout << " Invalid arguments. Expected arguments: " << std::endl;

		std::cout << "#1 Rover Position - Geodatic Lattitude [degree] - input " << std::endl;
		std::cout << "#2 Rover Position - Longitude [degree]- input " << std::endl;
		std::cout << "#3 Rover Position - Height [m]- input" << std::endl;
		std::cout << "#4 Time matching intervall between reference and target [s] - input" << std::endl;
		std::cout << "#5 Time Interval between PL calculations [s] - input" << std::endl;
		std::cout << "#6 Integrity probability. Probability of the real position (horizontal in ENU) is within the given Horizontal Protection Level - 0 < p < 1- input" << std::endl;
		std::cout << "#7 Elevaion mask [degree] - input" << std::endl;
		std::cout << "#8 Shall we use Klobuchar when the target model is not available? 0 - NO | 1 - YES - input" << std::endl;
		std::cout << "#9 Protection Level File with absolute path - output" << std::endl;
		std::cout << "#10 RINEX NAVIGATION File with absolute path - GPS ephermis are required - input" << std::endl;
		std::cout << "#11 Type of Target ionospheric model. '-i' stands for IONEX Model '-e' stands for EGNOS model - input" << std::endl;
		std::cout << "#12 Ionospheric file with absolute path for Target. The previous argument could be '-i' or '-e' so this file shall be IONEX or EMS file respectivly  - input" << std::endl;
		std::cout << "#13 Type of Reference ionospheric model. '-i' stands for IONEX Model '-e' stands for EGNOS model - input" << std::endl;
		std::cout << "#14 Ionospheric file with absolute path for Reference. The previous argument could be '-i' or '-e' so this file shall be IONEX or EMS file respectivly  - input" << std::endl;

		exit(1);
	}

	std::string target_FileNamewPath;
	std::string reference_FileNamewPath;
	EGNOS::ProtectionLevel::IonoType referenceIonoType = EGNOS::ProtectionLevel::IonoType::zero;
	EGNOS::ProtectionLevel::IonoType targetIonoType = EGNOS::ProtectionLevel::IonoType::zero;

	double latgeodetic = stod(argv[1]);
	double lon = stod(argv[2]);
	double height = stod(argv[3]);

	int matchingIntervall_in_secs = stod(argv[4]);
	if (matchingIntervall_in_secs <= 0) {
		std::cout << "Error in arguments " << std::endl;
		std::cout << "Time matching between target and reference data. It should be greater than Zero " << std::endl;
		exit(1);
	}

	int timeIntervallofRefModellMap_in_secs = stod(argv[5]);
	if (timeIntervallofRefModellMap_in_secs <= 0) {
		std::cout << "Error in arguments " << std::endl;
		std::cout << "Time Intervall between reference map. It should be greater than Zero " << std::endl;
		exit(1);
	}

	double probability_of_inner_circle = stod(argv[6]);
	if (probability_of_inner_circle <= 0 || probability_of_inner_circle >= 1) {
		std::cout << "Error in arguments " << std::endl;
		std::cout << "Integrity probability shall be between 0 and 1. => 0 < p < 1 " << std::endl;
		exit(1);
	}
	
	double elevationMask = stod(argv[7]);

	bool useKlobucharasSecunderModel = false;
	if (stod(argv[8]) == 0) {
		useKlobucharasSecunderModel = false;
	}
	else {
		useKlobucharasSecunderModel = true;
	}
	

	std::string PLwPath_out = argv[9];
	std::string RINEX_NAVFileNamewPath = argv[10];

	if (std::string(argv[11]) == "-i") {
		targetIonoType = EGNOS::ProtectionLevel::IonoType::ionex;
		target_FileNamewPath = argv[12];
	}
	else if(std::string(argv[11]) == "-e"){
		targetIonoType = EGNOS::ProtectionLevel::IonoType::egnos;
		target_FileNamewPath = argv[12];
	}
	else {
		exit(1);
	}

	if (argc == 15) {
		if (std::string(argv[13]) == "-i") {
			referenceIonoType = EGNOS::ProtectionLevel::IonoType::ionex;
			reference_FileNamewPath = argv[14];
		}
		else if (std::string(argv[13]) == "-e") {
			referenceIonoType = EGNOS::ProtectionLevel::IonoType::egnos;
			reference_FileNamewPath = argv[14];
		}
		else {
			exit(1);
		}
	}
	
	/*string ephFile = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\brdc0190.19n";
	string ionexFile = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\CODG0190.19I";
	string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136.ems";
	string PLwPath_out = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\Pos_lat49lon40h0_deltaT3600_el0_Tar_y2019_d019_136_Ref_CODG0190_brdc0190.pl";
	
	EGNOS::ProtectionLevel::IonoType targetIonoType = EGNOS::ProtectionLevel::IonoType::ionex;
	EGNOS::ProtectionLevel::IonoType referenceIonoType = EGNOS::ProtectionLevel::IonoType::zero;
	
	double latgeodetic = 49;
	double lon = 40;
	double height = 0;
	int intervallBetweenEpochsinSecs = 3600;
	double probability_of_inner_circle = 0.99;
	double elevationMask = 0;*/

	EGNOS::ProtectionLevel::run_PL(	RINEX_NAVFileNamewPath, 
									target_FileNamewPath, 
									reference_FileNamewPath, 
									PLwPath_out, 
									elevationMask, 
									referenceIonoType, 
									targetIonoType, 
									useKlobucharasSecunderModel, 
									latgeodetic, lon, height, 
									matchingIntervall_in_secs,
									timeIntervallofRefModellMap_in_secs,
									probability_of_inner_circle);
	
	std::cout << "Job is finished" << std::endl;

	return 0;
}

