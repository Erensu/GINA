#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "PRSolution.hpp"

#include "EGNOS_EMS_Data.hpp"
#include "EGNOS_EMS_Stream.hpp"

#include "IonosphericMaskBand.hpp"
#include "IonosphericGridPoint.hpp"

#include "GINAConfig.h"

using namespace std;

int main(int argc, char **argv) {

	EGNOS::IonosphericGridPoint example1;

	EGNOS::IonosphericGridPoint example2;

	example1.lat = 15;
	example1.lon = 30;

	example2 = example1;
	//example2.copy(example1);

	EGNOS::IonosphericGridPoint example3 = example2;

	return 0;
}