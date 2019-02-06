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

#define NO_EXCEPTED_RESULT 9999

typedef enum {
	Failed,
	Passed
}testStatus;

typedef struct {
	string nameOfTestCase;
	testStatus givenResult;
	testStatus expectedResult;
	double interPolationExpectedResult;
}testCaseDescriptor;

int numberOfFailedTests = 0;
std::vector<testCaseDescriptor> FailedTestCases;

void Test(EGNOS::VerticalIonoDelayInterpolator &interPol, EGNOS::IGPMap &IonoMap, EGNOS::IonosphericGridPoint pp, testCaseDescriptor testCase) {

	double interPolationTestResult = -999;
	testCase.givenResult = testStatus::Passed;
	try
	{
		interPolationTestResult = interPol.interpolate(IonoMap, pp);

		if (testCase.interPolationExpectedResult == NO_EXCEPTED_RESULT) {
			return;
		}

		if (interPolationTestResult != testCase.interPolationExpectedResult) {
			testCase.givenResult = testStatus::Failed;
			numberOfFailedTests++;
		}

		//cout << interPolationTestResult  << endl;
	}
	catch (exception &e)
	{


		if (testCase.expectedResult == testStatus::Passed) {
			testCase.givenResult = testStatus::Failed;
			std::cout << e.what() << endl;
			//std::cout << "Interpolation was not success and it was unexpected" << std::endl;
			numberOfFailedTests++;
		}
		else {
			//std::cout << "Interpolation was not success but it was expected" << std::endl;
		}
	}

	FailedTestCases.push_back(testCase);

}

void WriteOutTotalTestResults(bool withExtraInfo) {
	if (numberOfFailedTests > 0) {
		std::cout << "Test is failed" << std::endl;
		std::cout << std::endl << "Number of failed test cases are: " << numberOfFailedTests << std::endl;
		for (size_t i = 0; i < FailedTestCases.size(); i++)
		{
			if (withExtraInfo == true) {
				switch (FailedTestCases[i].givenResult)
				{
				case testStatus::Failed:
					cout << "Test case " << FailedTestCases[i].nameOfTestCase << " FAILED" << endl;
					break;
				case testStatus::Passed:
					cout << "Test case " << FailedTestCases[i].nameOfTestCase << " PASSED" << endl;
					break;
				default:
					throw("Unexpected error");
					break;
				}
			}

		}
	}
	else {
		std::cout << std::endl << "All the Tests are PASSED" << std::endl;

	}


}

int main(int argc, char **argv) {


#pragma region Initailaze Test

	testCaseDescriptor testCase;

	EGNOS::IGPMap IonoMap;
	EGNOS::VerticalIonoDelayInterpolator interPol;

	EGNOS::IonosphericGridPoint igp1;
	EGNOS::IonosphericGridPoint igp2;
	EGNOS::IonosphericGridPoint igp3;
	EGNOS::IonosphericGridPoint igp4;
	EGNOS::IonosphericGridPoint igp5;
	EGNOS::IonosphericGridPoint igp6;
	EGNOS::IonosphericGridPoint igp7;
	EGNOS::IonosphericGridPoint igp8;
	EGNOS::IonosphericGridPoint igp9;
	EGNOS::IonosphericGridPoint igp10;
	EGNOS::IonosphericGridPoint igp11;
	EGNOS::IonosphericGridPoint igp12;

	EGNOS::IonosphericGridPoint pp;
	EGNOS::IonosphericGridPoint out;


	igp1.lat = 85;
	igp1.lon = -170;
	igp1.setIonoDelayinMeter(10);
	igp1.valid = true;

	igp2.lat = 85;
	igp2.lon = -140;
	igp2.setIonoDelayinMeter(20);
	igp2.valid = false;

	igp3.lat = 85;
	igp3.lon = -110;
	igp3.setIonoDelayinMeter(30);
	igp3.valid = false;

	igp4.lat = 85;
	igp4.lon = -80;
	igp4.setIonoDelayinMeter(40);
	igp4.valid = true;

	igp5.lat = 85;
	igp5.lon = -50;
	igp5.setIonoDelayinMeter(50);
	igp5.valid = false;

	igp6.lat = 85;
	igp6.lon = -20;
	igp6.setIonoDelayinMeter(60);
	igp6.valid = false;

	igp7.lat = 85;
	igp7.lon = 10;
	igp7.setIonoDelayinMeter(70);
	igp7.valid = true;

	igp8.lat = 85;
	igp8.lon = 40;
	igp8.setIonoDelayinMeter(80);
	igp8.valid = false;

	igp9.lat = 85;
	igp9.lon = 70;
	igp9.setIonoDelayinMeter(90);
	igp9.valid = false;

	igp10.lat = 85;
	igp10.lon = 100;
	igp10.setIonoDelayinMeter(100);
	igp10.valid = true;

	igp11.lat = 85;
	igp11.lon = 130;
	igp11.setIonoDelayinMeter(110);
	igp11.valid = false;

	igp12.lat = 85;
	igp12.lon = 160;
	igp12.setIonoDelayinMeter(120);
	igp12.valid = false;

#pragma endregion


	/*Test 1 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);
	IonoMap.addIGPforDebugging(igp5);
	IonoMap.addIGPforDebugging(igp6);
	IonoMap.addIGPforDebugging(igp7);
	IonoMap.addIGPforDebugging(igp8);
	IonoMap.addIGPforDebugging(igp9);
	IonoMap.addIGPforDebugging(igp10);
	IonoMap.addIGPforDebugging(igp11);
	IonoMap.addIGPforDebugging(igp12);


	pp.lat = 85;
	

	for (size_t i = 0; i < 143; i++)	{

		pp.lon = -355.0 + 5.0 * i;
		out = interPol.getHorizontallyInterpolatedVertices(IonoMap, pp.lat, pp.lon, 5);
		if (out.valid == false) {
			cout << "Invalid IGP ";
		}
		cout << "Lon: " << pp.lon << " Corr: " << out.getIonoCorr() << endl;
	}
	

	return 0;
}