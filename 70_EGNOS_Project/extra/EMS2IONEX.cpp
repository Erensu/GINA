
#include "Runnables.hpp"

using namespace std;


int main(int argc, char **argv) {

	if (argc < 3 || argc > 5) {
		std::cout << " Invalid argument. \n Expected arguments: \n #1 EMS filename with absolute path -input \n #2 Ionex filename with absolute path - output " << std::endl;
		std::cout << " #2 Ionex filename with absolute path - output " << std::endl;
		std::cout << " #3 MapType - optional - default value is 0 - input" << std::endl;
		std::cout << "\t MapType value 0 - 5x5 Europe" << std::endl;
		std::cout << "\t MapType value 1 - 2.5x2.5 Europe - values are interpolated" << std::endl;
		std::cout << "\t MapType value 2 - 1x1 Europe - values are interpolated" << std::endl;
		std::cout << " #4 Update Interval in second - optional - default value is 3600 - input" << std::endl;
		std::cout << "\t When 'Update Interval' value is 0, it means everytime the parser gets an iono update from the ems, creates a map" << std::endl;
		exit(1);
	}

	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";


	// "C:\\Users\\lus2bp\\Source\\Repos\\GINA\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136.ems";
	// "C:\\Users\\lus2bp\\Source\\Repos\\GINA\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_All_with_fixedIntervals.19i";
	// #1 argument
	std::string EDAS_FileNamewPath = argv[1];
	// #2 argument
	std::string Output_IonexFileNamewPath = argv[2];

	// Make it empty
	std::string Output_IonexFileNamewPathLast = ""; //ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_Last.19i";
	std::string Output_IonexFileNamewPath_Detailed = "";

	// #3 argument
	EGNOS::EGNOSMapType mapType = EGNOS::europe5x5_tec; // world5x5_tec | europe5x5_tec | EGNOS::europe2_5x2_5_tec | EGNOS::europe1x1_tec
	bool doInterpolate = false;
	if (argc >= 4) {

		switch (stoi(argv[3]))
		{
		case 0:
			mapType = EGNOS::europe5x5_tec;
			doInterpolate = false;
			break;
		case 1:
			mapType = EGNOS::europe2_5x2_5_tec;
			doInterpolate = true;
			break;
		case 2:
			mapType = EGNOS::europe1x1_tec;
			doInterpolate = true;
			break;
		default:
			mapType = EGNOS::europe5x5_tec;
			doInterpolate = false;
			break;
		}
	}
	else {

		mapType = EGNOS::europe5x5_tec;
		doInterpolate = false;
	}

	// #4 argument
	double updateIntervalinSeconds = 3600;

	if (argc == 5) {

		updateIntervalinSeconds = stod(argv[4]);
	}
	else {
		updateIntervalinSeconds = 3600;
	}
	
	// Make it default
	gpstk::CivilTime firstUpdate;
	gpstk::CivilTime lastUpdate;

	// Call function
	EGNOS::EGNOS_RUNNABLE::processEMS(EDAS_FileNamewPath,
		Output_IonexFileNamewPath,
		Output_IonexFileNamewPathLast,
		Output_IonexFileNamewPath_Detailed,
		mapType,
		doInterpolate,
		firstUpdate,
		lastUpdate,
		updateIntervalinSeconds);

	std::cout << " EMS to Ionex converter succesfully finished the job" << std::endl;

	return 0;
}

