
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

	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";

	if (argc < 11 || argc > 13) {

		std::cout << " Invalid arguments. Expected arguments: " << std::endl;

		std::cout << "#1 Rover Position - Geodatic Lattitude [degree] - input " << std::endl;
		std::cout << "#2 Rover Position - Longitude [degree]- input " << std::endl;
		std::cout << "#3 Rover Position - Height [m]- input" << std::endl;
		std::cout << "#4 Time Interval between PL calculations [s] - input" << std::endl;
		std::cout << "#5 Integrity probability. Probability of the real position (horizontal in ENU) is within the given Horizontal Protection Level - 0 < p < 1- input" << std::endl;
		std::cout << "#6 Elevaion mask [degree] - input" << std::endl;
		std::cout << "#7 Protection Level File with absolute path - output" << std::endl;
		std::cout << "#8 RINEX NAVIGATION File with absolute path - GPS ephermis are required - input" << std::endl;
		std::cout << "#9 Type of Target ionospheric model. '-i' stands for IONEX Model '-e' stands for EGNOS model - input" << std::endl;
		std::cout << "#10 Ionospheric file with absolute path for Target. The previous argument could be '-i' or '-e' so this file shall be IONEX or EMS file respectivly  - input" << std::endl;
		std::cout << "#11 Type of Reference ionospheric model. '-i' stands for IONEX Model '-e' stands for EGNOS model - input" << std::endl;
		std::cout << "#12 Ionospheric file with absolute path for Reference. The previous argument could be '-i' or '-e' so this file shall be IONEX or EMS file respectivly  - input" << std::endl;

		exit(1);
	}

	std::string ionex_FileNamewPath;
	std::string EDAS_FileNamewPath;
	EGNOS::ProtectionLevel::IonoType referenceIonoType = EGNOS::ProtectionLevel::IonoType::zero;
	EGNOS::ProtectionLevel::IonoType targetIonoType = EGNOS::ProtectionLevel::IonoType::zero;

	double latgeodetic = stod(argv[1]);
	double lon = stod(argv[2]);
	double height = stod(argv[3]);

	int intervallBetweenEpochsinSecs = stod(argv[4]);
	if (intervallBetweenEpochsinSecs <= 0) {
		std::cout << "Error in arguments " << std::endl;
		std::cout << "Time Interval between PL calculations shall be greater than ZERO " << std::endl;
		exit(1);
	}
	double probability_of_inner_circle = stod(argv[5]);
	if (probability_of_inner_circle <= 0 || probability_of_inner_circle >= 1) {
		std::cout << "Error in arguments " << std::endl;
		std::cout << "Integrity probability shall be between 0 and 1. => 0 < p < 1 " << std::endl;
		exit(1);
	}
	
	double elevationMask = stod(argv[6]);

	std::string PLwPath_out = argv[7];
	std::string RINEX_NAVFileNamewPath = argv[8];

	if (std::string(argv[9]) == "-i") {
		targetIonoType = EGNOS::ProtectionLevel::IonoType::ionex;
		ionex_FileNamewPath = argv[10];
	}
	else if(std::string(argv[9]) == "-e"){
		targetIonoType = EGNOS::ProtectionLevel::IonoType::egnos;
		EDAS_FileNamewPath = argv[10];
	}
	else {
		exit(1);
	}

	if (argc == 13) {
		if (std::string(argv[11]) == "-i") {
			referenceIonoType = EGNOS::ProtectionLevel::IonoType::ionex;
			ionex_FileNamewPath = argv[12];
		}
		else if (std::string(argv[11]) == "-e") {
			referenceIonoType = EGNOS::ProtectionLevel::IonoType::egnos;
			EDAS_FileNamewPath = argv[12];
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

	EGNOS::ProtectionLevel::run_PL(RINEX_NAVFileNamewPath, ionex_FileNamewPath, EDAS_FileNamewPath, PLwPath_out, elevationMask, referenceIonoType, targetIonoType, latgeodetic, lon, height, intervallBetweenEpochsinSecs, probability_of_inner_circle);
	
	std::cout << "Job is finished" << std::endl;

	return 0;
}

