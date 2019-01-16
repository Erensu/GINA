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

using namespace std;

// Class to solve the equation system using Least Mean Squares
#include "SolverLMS.hpp"

// Class to solve the equation system using Weighted-Least Mean Squares
#include "SolverWMS.hpp"
#include "ComputeIonoModel.hpp"

int main(int argc, char **argv) {

	gpstk::IonexStore ionoStore;
	
	gpstk::Position nominalPosition;
	nominalPosition.setGeodetic(44, 19, 100, new gpstk::WGS84Ellipsoid());

	std::string ionexFile_In = ROOT "\\70_EGNOS_Project\\files\\carg0310.18i";
	std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\carg0310_out.18i";
	

	ionoStore.loadFile(ionexFile_In.c_str());

	gpstk::IonexStream strm_in(ionexFile_In.c_str(), std::ios::in);
	gpstk::IonexStream strm_out(ionexFile_Out.c_str(), std::ios::out);

	gpstk::IonexHeader header;
	header = ionoStore.getHeader(ionexFile_In);

	
	//ionoStore.dump(strm,1);

	//ionoStore.dump(std::cout, 1);
	

	gpstk::IonexData iod_in;
	gpstk::IonexData iod_out;
	
	strm_out << header;
	while (strm_in >> iod_in && iod_in.isValid())
	{
		iod_out.data = iod_in.data;

		iod_out.dim[0] = iod_in.dim[0];
		iod_out.dim[1] = iod_in.dim[1];
		iod_out.dim[2] = iod_in.dim[2];

		iod_out.exponent = iod_in.exponent;
		iod_out.lat[0] = iod_in.lat[0];
		iod_out.lat[1] = iod_in.lat[1];
		iod_out.lat[2] = iod_in.lat[2];

		iod_out.lon[0] = iod_in.lon[0];
		iod_out.lon[1] = iod_in.lon[1];
		iod_out.lon[2] = iod_in.lon[2];

		iod_out.hgt[0] = iod_in.hgt[0];
		iod_out.hgt[1] = iod_in.hgt[1];
		iod_out.hgt[2] = iod_in.hgt[2];

		iod_out.valid = iod_in.valid;

		iod_out.type = iod_in.type;

		iod_out.time = iod_in.time;
		
		strm_out << iod_out;
	}
	
	

	strm_in.close();
	strm_out.close();

	
	return 0;
}