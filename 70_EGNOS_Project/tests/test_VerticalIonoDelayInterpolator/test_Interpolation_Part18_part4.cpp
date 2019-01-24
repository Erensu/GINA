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

	EGNOS::IonosphericGridPoint pp;

	igp1.lat = -10;
	igp1.lon = -170;
	igp1.setIonoDelayinMeter(10);
	igp1.valid = true;

	igp2.lat = -10;
	igp2.lon = -180;
	igp2.setIonoDelayinMeter(20);
	igp2.valid = true;

	igp3.lat = -20;
	igp3.lon = -180;
	igp3.setIonoDelayinMeter(30);
	igp3.valid = true;

	igp4.lat = -20;
	igp4.lon = -170;
	igp4.setIonoDelayinMeter(40);
	igp4.valid = true;
#pragma endregion

#pragma region All 4 is fine


	/*  All 4 is fine*/
	pp.lat = -15;
	pp.lon = -175;
	

	/*Test 1 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 25;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'All 4 is fine in the middle'";
	Test(interPol, IonoMap, pp, testCase);

	pp.lat = -12.5;
	pp.lon = -172.5;
	

	testCase.interPolationExpectedResult = 18.75;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'All 4 is fine in 1st corner symmetric'";
	Test(interPol, IonoMap, pp, testCase);

	pp.lat = -12.5;
	pp.lon = -177.5;
	

	testCase.interPolationExpectedResult = 21.25;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'All 4 is fine in 2nd corner symmetric'";
	Test(interPol, IonoMap, pp, testCase);

	pp.lat = -17.5;
	pp.lon = -177.5;
	

	testCase.interPolationExpectedResult = 28.75;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'All 4 is fine in 3rd corner symmetric'";
	Test(interPol, IonoMap, pp, testCase);

	pp.lat = -17.5;
	pp.lon = -172.5;
	

	testCase.interPolationExpectedResult = 31.25;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'All 4 is fine in 4th corner symmetric'";
	Test(interPol, IonoMap, pp, testCase);

#pragma endregion

#pragma region 1st quarter
	/*pp  1st quarter*/
	pp.lat = -12.5;
	pp.lon = -172.5;
	

	/*Test 1 */
	igp1.valid = false;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.expectedResult = testStatus::Failed;
	testCase.nameOfTestCase = "'PP is in 1st quarter. 1st grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 2 */
	igp1.valid = true;
	igp2.valid = false;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 15;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 1st quarter. 2nd grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 3 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = false;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 20;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 1st quarter. 3rd grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 4 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = false;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 15;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 1st quarter. 4th grid is out'";
	Test(interPol, IonoMap, pp, testCase);
#pragma endregion

#pragma region 2nd quarter
	/*pp  2nd quarter*/
	pp.lat = -12.5;
	pp.lon = -177.5;
	

	/*Test 1 */
	igp1.valid = false;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 25;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 2nd quarter. 1st grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 2 */
	igp1.valid = true;
	igp2.valid = false;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.expectedResult = testStatus::Failed;
	testCase.nameOfTestCase = "'PP is in 2nd quarter. 2nd grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 3 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = false;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 25;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 2nd quarter. 3rd grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 4 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = false;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 20;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 2nd quarter. 4th grid is out'";
	Test(interPol, IonoMap, pp, testCase);

#pragma endregion

#pragma region 3rd quarter
	/*pp  3rd quarter*/
	pp.lat = -17.5;
	pp.lon = -177.5;
	

	/*Test 1 */

	igp1.valid = false;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 30;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 3rd quarter. 1st grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 2 */
	igp1.valid = true;
	igp2.valid = false;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 25;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 3rd quarter. 2nd grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 3 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = false;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.expectedResult = testStatus::Failed;
	testCase.nameOfTestCase = "'PP is in 3rd quarter. 3rd grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 4 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = false;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 25;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 3rd quarter. 4th grid is out'";
	Test(interPol, IonoMap, pp, testCase);

#pragma endregion

#pragma region 4th quarter
	/*pp  4th quarter*/
	pp.lat = -17.5;
	pp.lon = -172.5;
	

	/*Test 1 */
	igp1.valid = false;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 35;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 4th quarter. 1st grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 2 */
	igp1.valid = true;
	igp2.valid = false;
	igp3.valid = true;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 30;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 4th quarter. 2nd grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 3 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = false;
	igp4.valid = true;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.interPolationExpectedResult = 35;
	testCase.expectedResult = testStatus::Passed;
	testCase.nameOfTestCase = "'PP is in 4th quarter. 3rd grid is out'";
	Test(interPol, IonoMap, pp, testCase);

	/*Test 4 */
	igp1.valid = true;
	igp2.valid = true;
	igp3.valid = true;
	igp4.valid = false;

	IonoMap.addIGPforDebugging(igp1);
	IonoMap.addIGPforDebugging(igp2);
	IonoMap.addIGPforDebugging(igp3);
	IonoMap.addIGPforDebugging(igp4);

	testCase.expectedResult = testStatus::Failed;
	testCase.nameOfTestCase = "'PP is in 4th quarter. 4th grid is out'";
	Test(interPol, IonoMap, pp, testCase);
#pragma endregion

	WriteOutTotalTestResults(true);
	return 0;
}