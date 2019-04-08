#include "Runnables.hpp"

using namespace std;
int main(int argc, char **argv) {

	if (argc < 3 || argc > 5) {
		std::cout << " Invalid argument. \n Expected arguments: \n #1 Ionex filename with absolute path -input \n #2 Ionex filename with absolute path - input " << std::endl;
		std::cout << " #3 Differentiated IONEX filename with absolute path - output " << std::endl;
		std::cout << " #4 Matching intervall - optional - default value is 60 - input" << std::endl;
		std::cout << "\t Matching intervall means that the runnable differentiate the available ionex maps from the first file to the closeset ionex map of the second file in the given matching interval." << std::endl;
		std::cout << "TEC values are calculated as: TEC1 - TEC2" << std::endl;
		std::cout << "RMS values calculated as:		sqrt(RMS1^2 + RMS2^2)" << std::endl;
		exit(1);
	}

	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";

	// "C:\\Users\\lus2bp\\Source\\Repos\\GINA\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\CODG0190.19I";
	// "C:\\Users\\lus2bp\\Source\\Repos\\GINA\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_All_with_fixedIntervals.19i";
	// "C:\\Users\\lus2bp\\Source\\Repos\\GINA\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\Difference_IONEX_CODG0190-y2019_d019_136ems_ionex_Europe_5x5_IGPMap_All_with_fixedIntervals.19i";

	// #1 argument
	std::string IonexFileNamewPath_1 = argv[1];
	// #2 argument
	std::string IonexFileNamewPath_2 = argv[2];
	// #3 argument
	std::string IonexFileNamewPath_Out = argv[3];

	
	// #4 argument
	double matchingIntervall = 60;

	if (argc == 5) {

		matchingIntervall = stod(argv[4]);
	}
	else {
		matchingIntervall = 60;
	}


	// Call function
	EGNOS::EGNOS_RUNNABLE::compareIonexFiles(		IonexFileNamewPath_1,
													IonexFileNamewPath_2, 
													IonexFileNamewPath_Out,
													matchingIntervall);

	std::cout << " Create of the new differental ionex is finished" << std::endl;

	return 0;
}

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
