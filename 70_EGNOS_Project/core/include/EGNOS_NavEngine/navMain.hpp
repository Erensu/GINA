#ifndef EGNOS_NAV_MAIN_HPP
#define EGNOS_NAV_MAIN_HPP

#include "simpleNavigationSolution.hpp"

typedef enum
{
	egnos = 0,
	ionex = 1,
	zero
}IonoType;

int mainNavigationSolution(std::string& obsData, std::string &ephData, string& ionexFile, std::string& EMSData, std::string& rtkpost_out_gpstk, std::string& rtkpost_out_navEngine, std::string& errorFile, double elevationMask, IonoType ionoType);

static void applyElevationMask(double elevationMask, gpstk::Position &R, vector<double> &rangeVec, vector<gpstk::SatID> &prnVec, gpstk::GPSEphemerisStore &bcestore, gpstk::CommonTime &gpstime);

static void applyIonoCorrection(gpstk::CommonTime &time, gpstk::GPSEphemerisStore &bcestore, vector<gpstk::SatID> &id, vector<double> &rangeVec, EGNOS::IonoModel &pIonoModel, gpstk::Position Rx);

#endif