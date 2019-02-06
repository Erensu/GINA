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
	EGNOS::VerticalIonoDelayInterpolator interPol(&IonoMap);

	EGNOS::IonosphericGridPoint igp1;
	EGNOS::IonosphericGridPoint igp2;
	EGNOS::IonosphericGridPoint igp3;
	EGNOS::IonosphericGridPoint igp4;

	EGNOS::IonosphericGridPoint pp;

	/*Test 1 */
	pp.lat = 52;
	pp.lon = 17;

	interPol.setPP(pp);

	igp1.lat = 55;
	igp1.lon = 20;
	igp1.IGPVerticalDelayEstimate = 100;
	igp1.valid = true;

	igp2.lat = 55;
	igp2.lon = 15;
	igp2.IGPVerticalDelayEstimate = 100;
	igp2.valid = true;
	
	igp3.lat = 50;
	igp3.lon = 15;
	igp3.IGPVerticalDelayEstimate = 100;
	igp3.valid = true;
	
	igp4.lat = 50;
	igp4.lon = 20;
	igp4.IGPVerticalDelayEstimate = 100;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 2 */
	pp.lat = 52;
	pp.lon = 17;

	interPol.setPP(pp);

	igp1.lat = 55;
	igp1.lon = 20;
	igp1.IGPVerticalDelayEstimate = 50;
	igp1.valid = true;

	igp2.lat = 55;
	igp2.lon = 15;
	igp2.IGPVerticalDelayEstimate = 50;
	igp2.valid = true;

	igp3.lat = 50;
	igp3.lon = 15;
	igp3.IGPVerticalDelayEstimate = 100;
	igp3.valid = true;

	igp4.lat = 50;
	igp4.lon = 20;
	igp4.IGPVerticalDelayEstimate = 100;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 3 */
	pp.lat = 53;
	pp.lon = 18;

	interPol.setPP(pp);

	igp1.lat = 55;
	igp1.lon = 20;
	igp1.IGPVerticalDelayEstimate = 50;
	igp1.valid = true;

	igp2.lat = 55;
	igp2.lon = 15;
	igp2.IGPVerticalDelayEstimate = 50;
	igp2.valid = true;

	igp3.lat = 50;
	igp3.lon = 15;
	igp3.IGPVerticalDelayEstimate = 100;
	igp3.valid = true;

	igp4.lat = 50;
	igp4.lon = 20;
	igp4.IGPVerticalDelayEstimate = 100;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 4 */
	pp.lat = 52.5;
	pp.lon = 17.5;

	interPol.setPP(pp);

	igp1.lat = 55;
	igp1.lon = 20;
	igp1.IGPVerticalDelayEstimate = 50;
	igp1.valid = true;

	igp2.lat = 55;
	igp2.lon = 15;
	igp2.IGPVerticalDelayEstimate = 50;
	igp2.valid = true;

	igp3.lat = 50;
	igp3.lon = 15;
	igp3.IGPVerticalDelayEstimate = 100;
	igp3.valid = true;

	igp4.lat = 50;
	igp4.lon = 20;
	igp4.IGPVerticalDelayEstimate = 100;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 5 */
	pp.lat = 52.5;
	pp.lon = 15;

	interPol.setPP(pp);

	igp1.lat = 55;
	igp1.lon = 20;
	igp1.IGPVerticalDelayEstimate = 50;
	igp1.valid = true;

	igp2.lat = 55;
	igp2.lon = 15;
	igp2.IGPVerticalDelayEstimate = 50;
	igp2.valid = true;

	igp3.lat = 50;
	igp3.lon = 15;
	igp3.IGPVerticalDelayEstimate = 100;
	igp3.valid = true;

	igp4.lat = 50;
	igp4.lon = 20;
	igp4.IGPVerticalDelayEstimate = 100;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;


	/*Test 6 */
	pp.lat = 51;
	pp.lon = 17.5;

	interPol.setPP(pp);

	igp1.lat = 55;
	igp1.lon = 20;
	igp1.IGPVerticalDelayEstimate = 50;
	igp1.valid = true;

	igp2.lat = 55;
	igp2.lon = 15;
	igp2.IGPVerticalDelayEstimate = 100;
	igp2.valid = true;

	igp3.lat = 50;
	igp3.lon = 15;
	igp3.IGPVerticalDelayEstimate = 100;
	igp3.valid = true;

	igp4.lat = 50;
	igp4.lon = 20;
	igp4.IGPVerticalDelayEstimate = 50;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;
	

	return 0;
}