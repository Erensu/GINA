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

	EGNOS::IonosphericMaskBands example;

	int lat = 0, lon = 0;
	bool successFlag = false;

	for (size_t band = 0; band < 11; band++)
	{
		for (size_t bitpos = 0; bitpos < 202; bitpos++)
		{

			try {
				successFlag = example.getPosition((unsigned char)band, (unsigned char)bitpos, lat, lon);

			}
			catch (const char* msg) {
				cerr << msg << endl;
				successFlag = false;
			}
			catch (...) {
				cerr << "Unexpected error" << endl;
				successFlag = false;
			}

			if (successFlag) {

				cout << lat << " " << lon << endl;
			}
			else {
				cout << "Error in: band - " << band << " bitpos - " << bitpos << endl;
			}

		}
	}
	

	return 0;
}