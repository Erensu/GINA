#ifndef EGNOS_NAV_MAIN_HPP
#define EGNOS_NAV_MAIN_HPP

#include "simpleNavigationSolution.hpp"

int mainNavigationSolution(std::string& obsData, std::string &ephData, std::string& EMSData, std::string& rtkpost_out_gpstk, std::string& rtkpost_out_navEngine, std::string& errorFile);

#endif