#pragma once

#include "IGPMap.hpp"
#include <algorithm>

namespace EGNOS {

	IGPMap::IGPMap(void) {

	}

	IGPMap::IGPMap(IGPMap* original) {

		this->candidateIGPs = original->candidateIGPs;
		this->Map = original->Map;
	}

	IGPMap::~IGPMap(void) {

		this->Map.clear();
		this->candidateIGPs.clear();
	}

	void IGPMap::setIGPCandidates(const std::vector<IonosphericGridPoint> & const candidateIGPs) {

		this->candidateIGPs = candidateIGPs;
	}

	void IGPMap::updateIGPCandidate(const IonosphericGridPointMasksMessageParser & const IGPMessageParser) {

		for (std::vector<IonosphericGridPoint>::iterator it = this->candidateIGPs.begin(); it != this->candidateIGPs.end(); ++it) {

			IGPMessageParser.updateIGP(*it);
		}
	}

	void IGPMap::updateMap(void) {

		for (std::vector<IonosphericGridPoint>::const_iterator it = this->candidateIGPs.begin(); it != this->candidateIGPs.end(); ++it) {
			
			if (it->valid != true) {
				continue;
			}

			IGPCoordinate coor = { it->lat , it->lon};
			this->Map[coor] = *it;
		}
	}

	void IGPMap::addIGPforDebugging(IonosphericGridPoint newIGP) {

		IGPCoordinate coor = { newIGP.lat , newIGP.lon };
		this->Map[coor] = newIGP;
	}

	void IGPMap::reset(void) {

		this->Map.clear();
		this->candidateIGPs.clear();
	}

	std::ostream &operator<<(std::ostream &os, IGPMap const &imap) {

		os << "Size of the Iono Map is " << imap.Map.size() << std::endl;
		os << "Element of IGPs candidates" << std::endl;

		std::map<IGPCoordinate, IonosphericGridPoint>::const_iterator it;

		for (it = imap.Map.begin(); it != imap.Map.end(); it++){

			os << it->second << std::endl;
		}

		return os;
	}

	IonosphericGridPoint IGPMap::getIGP(double lat, double lon) const{

		IGPCoordinate  keyword;
		keyword.lat = lat;
		keyword.lon = lon;

		std::map<IGPCoordinate, IonosphericGridPoint>::const_iterator it;

		it = this->Map.find(keyword);

		if (it != this->Map.end()) {
			if (it->second.valid == true) {
				return it->second;
			}
			else {
				throw std::domain_error("IGP is not valid");
			}

		}
		else {
			throw std::domain_error("IGP is not found");
		}
	}

	double IGPMap::getTEC(gpstk::CivilTime epoch, double lat, double lon) const {

		IonosphericGridPoint igp = getIGP(lat, lon);
		return igp.IGPVerticalDelayEstimate;

	}

	double IGPMap::getRMS(gpstk::CivilTime epoch, double lat, double lon) const {
		IonosphericGridPoint igp = getIGP(lat, lon);
		return igp.GIVEI;	// TODO write a function which can transform GIVEI to meter format.
	}

	// TODO - unfinished
	std::vector<gpstk::CivilTime> IGPMap::getEpochTimes(void) const {

		std::vector<gpstk::CivilTime> epochs;
		return epochs;
	}


}