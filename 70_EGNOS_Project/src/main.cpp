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
#include "IonoCorrection.hpp"

#include "GINAConfig.h"

using namespace std;

int main(int argc, char **argv) {

	EGNOS::IGPMap IonoMap;

	EGNOS::IonosphericGridPoint igp1;
	igp1.lat = 55;
	igp1.lon = 20;
	igp1.IGPVerticalDelayEstimate = 100;
	igp1.valid = true;

	EGNOS::IonosphericGridPoint igp2;
	igp2.lat = 55;
	igp2.lon = 15;
	igp2.IGPVerticalDelayEstimate = 100;
	igp2.valid = true;

	EGNOS::IonosphericGridPoint igp3;
	igp3.lat = 50;
	igp3.lon = 15;
	igp3.IGPVerticalDelayEstimate = 100;
	igp3.valid = true;

	EGNOS::IonosphericGridPoint igp4;
	igp4.lat = 50;
	igp4.lon = 20;
	igp4.IGPVerticalDelayEstimate = 100;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	EGNOS::VerticalIonoDelayInterpolator interPol(&IonoMap);
	//cout << IonoMap;

	EGNOS::IonosphericGridPoint pp;
	pp.lat = 52;
	pp.lon = 17;

	interPol.setPP(pp);
	std::cout << interPol.gridPointSelectionCriteria() << std::endl;;
	

	return 0;
}