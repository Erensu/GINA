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

	int n, m;
	
	EGNOS::IGPMap IonoMap;
	EGNOS::IGPMap  * const link2Map = &IonoMap;

	EGNOS::VerticalIonoDelayInterpolator interPol(link2Map);

	n = 76;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -76;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 75;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 76;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -76;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 75;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -75;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 6;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 2;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -2;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 2;
	m = 10;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -2;
	m = 10;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;


	n = 82;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 78;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 80;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -82;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -78;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -80;
	m = 5;

	cout << "number: " << n << " divisible " << m << endl;
	cout << "second closest Number From High: " << interPol.secondclosestNumberFromHigh(n, m) << endl;
	cout << "second closest Number From Low: " << interPol.secondclosestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 76;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n =-76;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 82;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -82;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;


	n = 80;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -80;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 0;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n = 2;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	n = -2;
	m = 5;
	cout << "number: " << n << " divisible " << m << endl;
	cout << "closest Number From High: " << interPol.closestNumberFromHigh(n, m) << endl;
	cout << "closest Number From Low: " << interPol.closestNumberFromLow(n, m) << endl;
	cout << endl;

	return 0;
}