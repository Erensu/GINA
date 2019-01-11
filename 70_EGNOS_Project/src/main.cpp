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

#pragma region Initailaze Test

	EGNOS::IGPMap IonoMap;
	EGNOS::VerticalIonoDelayInterpolator interPol(&IonoMap);

	EGNOS::IonosphericGridPoint igp1;
	EGNOS::IonosphericGridPoint igp2;
	EGNOS::IonosphericGridPoint igp3;
	EGNOS::IonosphericGridPoint igp4;

	EGNOS::IonosphericGridPoint pp;

	igp1.lat = 5;
	igp1.lon = 175;
	igp1.IGPVerticalDelayEstimate = 100;
	igp1.valid = true;

	igp2.lat = 5;
	igp2.lon = -180;
	igp2.IGPVerticalDelayEstimate = 100;
	igp2.valid = true;

	igp3.lat = 0;
	igp3.lon = -180;
	igp3.IGPVerticalDelayEstimate = 100;
	igp3.valid = true;

	igp4.lat = 0;
	igp4.lon = 175;
	igp4.IGPVerticalDelayEstimate = 100;
	igp4.valid = true;
#pragma endregion

#pragma region Four grid interpolation quarter
	pp.lat = 2;
	pp.lon = 177;
	interPol.setPP(pp);

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

#pragma endregion

#pragma region 1st quarter
	/*pp  1st quarter*/
	pp.lat = 4;
	pp.lon = 176;
	interPol.setPP(pp);

	/*Test 1 */
	igp1.valid = false;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 2 */
	igp1.valid = true;
	igp2.valid = false;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 3 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = false;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 4 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = false;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;
#pragma endregion

#pragma region 2nd quarter
	/*pp  2nd quarter*/
	pp.lat = 4;
	pp.lon = 179;
	interPol.setPP(pp);

	/*Test 1 */
	igp1.valid = false;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 2 */
	igp1.valid = true;
	igp2.valid = false;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 3 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = false;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 4 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = false;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;
#pragma endregion

#pragma region 3rd quarter
	/*pp  3rd quarter*/
	pp.lat = 1;
	pp.lon = 179;
	interPol.setPP(pp);

	/*Test 1 */

	igp1.valid = false;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 2 */
	igp1.valid = true;
	igp2.valid = false;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 3 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = false;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 4 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = false;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;
#pragma endregion

#pragma region 4th quarter
	/*pp  4th quarter*/
	pp.lat = 1;
	pp.lon = 176;
	interPol.setPP(pp);

	/*Test 1 */
	igp1.valid = false;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 2 */
	igp1.valid = true;
	igp2.valid = false;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 3 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = false;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;

	/*Test 4 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = false;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	std::cout << interPol.gridPointSelectionCriteria() << std::endl;
#pragma endregion

	return 0;
}