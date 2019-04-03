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

template<std::size_t R, std::size_t L, std::size_t N>
std::bitset<N> project_range(std::bitset<N> b)
{
	static_assert(R <= L && L <= N, "invalid bitrange");
	b >>= R;            // drop R rightmost bits
	b <<= (N - L + R);  // drop L-1 leftmost bits
	b >>= (N - L);      // shift back into place
	return b;
}

int main(int argc, char **argv) {

	
	std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\h17.ems";

	EGNOS_EMS_Parser::EGNOS_EMS_Stream exampleStreamIn(EDAS_FileNamewPath.c_str());

	EGNOS_EMS_Parser::EGNOS_EMS_Data EData;

	EGNOS::IonosphericDelayCorrectionsMessageParser IonoGridPointParser;

	while (exampleStreamIn >> EData) {
		
		if (EData.messageId == 26) {
			//cout << "Message 26 read" << endl;
			IonoGridPointParser += EData.message;
			//cout << "Message 26 checked" << endl;

			cout << IonoGridPointParser << endl;
		}
	}

	exampleStreamIn.close();


	return 0;
}