
#include "Runnables.hpp"

using namespace std;

#include "IonexStream.hpp"
#include "IonexHeader.hpp"
#include "IonexData.hpp"
#include "IonexStore.hpp"

#include "RTKPOST_Data.hpp"
#include "RTKPOST_Stream.hpp"

int main(int argc, char **argv) {

	std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136.ems";
	std::string Output_IonexFileNamewPath = ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_All_with_fixedIntervals.19i";
	std::string Output_IonexFileNamewPathLast = ""; //ROOT "\\70_EGNOS_Project\\core\\files\\PositionComparisonInputFiles\\bute0190\\y2019_d019_136ems_ionex_Europe_5x5_IGPMap_Last.19i";
	std::string Output_IonexFileNamewPath_Detailed = "";

	EGNOS::EGNOSMapType mapType = EGNOS::europe5x5_tec; // world5x5_tec | europe5x5_tec | EGNOS::europe2_5x2_5_tec | EGNOS::europe1x1_tec

	gpstk::CivilTime firstUpdate;
	gpstk::CivilTime lastUpdate;

	EGNOS::EGNOS_RUNNABLE::processEMS(EDAS_FileNamewPath,
		Output_IonexFileNamewPath,
		Output_IonexFileNamewPathLast,
		Output_IonexFileNamewPath_Detailed,
		mapType,
		false,
		firstUpdate,
		lastUpdate,
		3600);


	return 0;
}

