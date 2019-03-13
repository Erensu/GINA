#ifndef EGNOS_NAV_MAIN_HPP
#define EGNOS_NAV_MAIN_HPP

#include "simpleNavigationSolution.hpp"

int mainNavigationSolution(std::string& obsData, std::string &ephData, std::string& EMSData, std::string& rtkpost_out_gpstk, std::string& rtkpost_out_navEngine, std::string& errorFile, double elevationMask);

void applyElevationMask(double elevationMask, 
	double x, 
	double y, 
	double z, 
	vector<double> &rangeVec, vector<gpstk::SatID> &prnVec, gpstk::GPSEphemerisStore &bcestore, gpstk::CommonTime &gpstime);

#endif