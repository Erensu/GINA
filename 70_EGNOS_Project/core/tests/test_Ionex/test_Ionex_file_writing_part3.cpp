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


	std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\h17ems_ionex_out.18i";
	std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\h17.ems";

	EGNOS_EMS_Parser::EGNOS_EMS_Stream exampleStreamIn(EDAS_FileNamewPath.c_str());

	EGNOS_EMS_Parser::EGNOS_EMS_Data EData;
	EGNOS::IonosphericDelayCorrectionsMessageParser IonoGridPointParser;
	EGNOS::IonosphericGridPointMasksMessageParser IonoMaskParser;

	EGNOS::IGPMap IonoMap;
	EGNOS::IonexCreator ionexWriter;

	bool weHad18 = false;
	bool weHad26 = false;

	while (exampleStreamIn >> EData) {

		if (EData.messageId == 18) {

			IonoMaskParser += EData.message;
			//cout << IonoMaskParser << endl;
			weHad18 = true;
		}

		if (EData.messageId == 26) {

			IonoGridPointParser += EData.message;
			//cout << IonoGridPointParser << endl;
			weHad26 = true;
		}

		if (weHad18 || weHad26) {
			IonoMap.setIGPCandidates(IonoGridPointParser.getIonosphericGridPoint());
			IonoMap.updateIGPCandidate(IonoMaskParser);
			IonoMap.updateMap();

			//cout << IonoMap;

			weHad26 = false;
			weHad18 = false;
		}
	}

	//cout << IonoMap;

	exampleStreamIn.close();


	ionexWriter.setIonexData(IonoMap);
	ionexWriter.write2file(ionexFile_Out);

	return 0;
}