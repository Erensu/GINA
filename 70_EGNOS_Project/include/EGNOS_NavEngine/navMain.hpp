#ifndef EGNOS_NAV_MAIN_HPP
#define EGNOS_NAV_MAIN_HPP

#include "simpleNavigationSolution.hpp"

int mainNavigationSolution(std::string& obsData, std::string &ephData, std::string& EMSData, std::string& rtkpost_out_gpstk, std::string& rtkpost_out_navEngine, std::string& errorFile, double elevationMask);

static void applyElevationMask(double elevationMask, gpstk::Position &R, vector<double> &rangeVec, vector<gpstk::SatID> &prnVec, gpstk::GPSEphemerisStore &bcestore, gpstk::CommonTime &gpstime);

static void applyIonoCorrection(gpstk::CommonTime &time, gpstk::GPSEphemerisStore &bcestore, vector<gpstk::SatID> &id, vector<double> &rangeVec, EGNOS::IonoModel &pIonoModel, gpstk::Position Rx);

#endif