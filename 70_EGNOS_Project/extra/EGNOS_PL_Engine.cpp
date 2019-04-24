
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

	string ephFile = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\brdc0190.19n";
	string ionexFile = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\CODG0190.19I";
	string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136.ems";
	string PLwPath_out = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\Pos_lat49lon30h0_deltaT3600_el0_CODG0190_136_brdc0190.pl";
	double elevationMask = 0;
	EGNOS::ProtectionLevel::IonoType ionoType = EGNOS::ProtectionLevel::IonoType::egnos;

	double latgeodetic = 49;
	double lon = 30;
	double height = 0;
	int intervallBetweenEpochsinSecs = 3600;
	double probability_of_inner_circle = 0.99;

	EGNOS::ProtectionLevel::run_PL(ephFile, ionexFile, EDAS_FileNamewPath, PLwPath_out, elevationMask, ionoType, latgeodetic, lon, height, intervallBetweenEpochsinSecs, probability_of_inner_circle);
	
	std::cout << "Job is finished" << std::endl;

	return 0;
}

