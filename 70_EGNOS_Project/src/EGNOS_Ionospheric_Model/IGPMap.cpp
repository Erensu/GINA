#pragma once

#include "IGPMap.hpp"
#include <algorithm>

namespace EGNOS {

#pragma region IGPMediator

	void IGPMediator::updateIGPCandidate(const IonosphericGridPointMasksMessageParser & const IGPMessageParser) {

		for (int i = 0; i < candidateIGPs.size(); i++) {

			IGPMessageParser.updateIGP(candidateIGPs[i]);
			candidateIGPs[i].setReferenceTime(this->currentDataTime);
		}
	}

	void IGPMediator::setIGPCandidates(const std::vector<IonosphericGridPoint> & const candidateIGPs) { 

		this->candidateIGPs = candidateIGPs; 
	}

	std::vector<IonosphericGridPoint> IGPMediator::getIGPCandidates(void) {

		return this->candidateIGPs;
	}

	void IGPMediator::updateTime(gpstk::CommonTime &time) {

		currentDataTime = time;
	}

#pragma endregion

#pragma region IGPMap

	IGPMap::IGPMap(IGPMap* original) {

		this->Map = original->Map;
	}

	IGPMap::~IGPMap(void) {

		this->Map.clear();
	}

	bool IGPMap::updateMap(IGPMediator &mediator) {

		bool didWeHaveNewData = false;
		this->referenceTime = mediator.getReferencetime();

		std::vector<IonosphericGridPoint> candidateIGPs = mediator.getIGPCandidates();

		for (std::vector<IonosphericGridPoint>::const_iterator it = candidateIGPs.begin(); it != candidateIGPs.end(); ++it) {
			
			if (it->valid != true) {
				continue;
			}

			IGPCoordinate coor = { it->lat , it->lon};

			if (this->Map[coor] == *it) {
				// Do nothing
			}
			else {
				didWeHaveNewData = true;
				this->Map[coor] = *it;
			}
		}

		return didWeHaveNewData;
	}

	void IGPMap::addIGPforDebugging(IonosphericGridPoint newIGP) {

		restrictLonginDegree(newIGP.lon);

		IGPCoordinate coor = { newIGP.lat , newIGP.lon };
		this->Map[coor] = newIGP;
	}

	void IGPMap::restrictLonginDegree(double &indegree) const {

		if (indegree >= 180) {
			indegree = fmod(indegree, 180);
			indegree -= 180;
			return;
		}

		if (indegree < -180) {
			indegree = fmod(indegree, 180);
			indegree = 180 - fabs(indegree);
			return;
		}
	}

	void IGPMap::reset(void) {

		this->Map.clear();
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

		IGPCoordinate keyword;
		keyword.lat = lat;
		keyword.lon = lon;

		restrictLonginDegree(keyword.lon);

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

	double IGPMap::getTEC(gpstk::CommonTime epoch, double lat, double lon) const {

		double tec;

		try	{

			IonosphericGridPoint igp = getIGP(lat, lon);
			tec = igp.getIonoCorr();
		}
		catch (const std::exception& e)	{

			throw(e);
		}
		
		return tec;
	}

	double IGPMap::getRMS(gpstk::CommonTime epoch, double lat, double lon) const {

		double rms;

		try {

			IonosphericGridPoint igp = getIGP(lat, lon);
			rms = igp.getIonoCorrVariance();
		}
		catch (const std::exception& e) {

			throw(e);
		}

		rms = sqrt(rms);
		return rms;
	}

	std::vector<gpstk::CommonTime> IGPMap::getEpochTimes(void) const {

		std::vector<gpstk::CommonTime> epochs;
		epochs.push_back(this->referenceTime);

		return epochs;
	}

#pragma endregion

}