#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "PRSolution.hpp"

#include "EGNOS_EMS_Data.hpp"
#include "EGNOS_EMS_Stream.hpp"

#include "IGPMap.hpp"
#include "IonosphericMaskBand.hpp"
#include "IonosphericGridPoint.hpp"

#include "GINAConfig.h"
#include "CommonTime.hpp"
#include "CivilTime.hpp"

#include "IonexStream.hpp"

#include <ctime>  

using namespace std;

// Class to solve the equation system using Least Mean Squares
#include "SolverLMS.hpp"

// Class to solve the equation system using Weighted-Least Mean Squares
#include "SolverWMS.hpp"
#include "ComputeIonoModel.hpp"

#include "IGPMap.hpp"
#include "IonoCorrection.hpp"

int main(int argc, char **argv) {

	EGNOS::IGPMap dummyMap;
	EGNOS::IonexCreator ionexWriter;
	std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\test_ionex_out.18i";

	ionexWriter.setIonexData(dummyMap);
	ionexWriter.write2file(ionexFile_Out);

	return 0;
}