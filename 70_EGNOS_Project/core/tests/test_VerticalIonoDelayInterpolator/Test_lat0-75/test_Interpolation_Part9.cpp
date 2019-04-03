#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "PRSolution.hpp"

#include "EGNOS_EMS_Data.hpp"
#include "EGNOS_EMS_Stream.hpp"

#include "IonosphericMaskBand.hpp"

#include "GINAConfig.h"

using namespace std;

int main(int argc, char **argv) {

	std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\h17.ems";
	std::string EDAS_FileNamewPath_out = ROOT "\\70_EGNOS_Project\\files\\h17_out.ems";

	EGNOS_EMS_Parser::EGNOS_EMS_Stream exampleStreamIn(EDAS_FileNamewPath.c_str());
	EGNOS_EMS_Parser::EGNOS_EMS_Stream exampleStreamOut(EDAS_FileNamewPath_out.c_str(), std::ios::out);

	EGNOS_EMS_Parser::EGNOS_EMS_Data EData;

	while (exampleStreamIn >> EData) {
		exampleStreamOut << EData;
	}

	exampleStreamIn.close();
	exampleStreamOut.close();


	return 0;
}