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

	IonosphericGridPoint IGPMap::getIGP(gpstk::CommonTime &epoch, double lat, double lon) const{

		if (epoch != this->referenceTime) {
			throw std::domain_error("Time doesn't match");
		}

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

	double IGPMap::getTEC(gpstk::CommonTime &epoch, double lat, double lon) const {

		double tec;
		if (epoch != referenceTime) {
			throw std::domain_error("Time is not valid");
		}

		try	{

			IonosphericGridPoint igp = getIGP(epoch, lat, lon);
			tec = igp.getIonoCorr();
		}
		catch (const std::exception& e)	{

			throw(e);
		}
		
		return tec;
	}

	double IGPMap::getRMS(gpstk::CommonTime &epoch, double lat, double lon) const {

		double variance;

		try {

			IonosphericGridPoint igp = getIGP(epoch, lat, lon);
			variance = igp.getIonoCorrVariance();
		}
		catch (const std::exception& e) {

			throw(e);
		}

		return sqrt(variance);
	}

	std::vector<gpstk::CommonTime> IGPMap::getEpochTimes(void) const {

		std::vector<gpstk::CommonTime> epochs;
		epochs.push_back(this->referenceTime);

		return epochs;
	}


	

#pragma endregion

#pragma region IGPMapStore

	IGPMapStore::IGPMapStore(IGPMapStore* original) {
	
		this->numberOfMap = original->numberOfMap;
		this->initialTime = original->initialTime;
		this->finalTime = original->finalTime;
		this->igpMaps = original->igpMaps;
	}

	IGPMap IGPMapStore::getIGPMap(gpstk::CommonTime &epoch) {

		IGPMap iMap;
		IGPMapMap::const_iterator it2Map = igpMaps.find(epoch);
		if (it2Map == igpMaps.end()) {

			throw std::domain_error("IGPMap does not exist");
		}
		else {
			iMap = it2Map->second;
		}

		return iMap;
	}

	void IGPMapStore::updateTime(gpstk::CommonTime &t) {

		if (igpMaps.size() == 0) {
			initialTime = t;
			finalTime = t;
			return;
		}

		if (t < initialTime) {
			initialTime = t;
		}
		else if (t > finalTime) {
			finalTime = t;
		}
	}

	void IGPMapStore::addMap(gpstk::CommonTime &time, IGPMap &igp) {

		updateTime(time);
		IGPMap igpIn(igp);

		if (igpMaps.insert(std::pair<gpstk::CommonTime, IGPMap>(time, igpIn)).second == true) {
			numberOfMap++;
		}
	}

	double IGPMapStore::getTEC(gpstk::CommonTime &epoch, double lat, double lon) const {

		double rtv = 0;
		IGPMapMap::const_iterator it2Map = igpMaps.find(epoch);
		if (it2Map == igpMaps.end()) {

			throw std::domain_error("Time is not valid");
		}
		else {
			IGPMap iMap = it2Map->second;

			try
			{
				rtv = iMap.getTEC(epoch, lat, lon);
			}
			catch (const std::exception& e)	
			{
				throw(e);
			}

		}
		return rtv;
	}

	double IGPMapStore::getRMS(gpstk::CommonTime &epoch, double lat, double lon) const {

		double rtv = 0;
		IGPMapMap::const_iterator it2Map = igpMaps.find(epoch);
		if (it2Map == igpMaps.end()) {

			throw std::domain_error("Time is not valid");
		}
		else {
			IGPMap iMap = it2Map->second;

			try
			{
				rtv = iMap.getRMS(epoch, lat, lon);
			}
			catch (const std::exception& e)
			{
				throw(e);
			}

		}
		return rtv;
	}

	std::vector<gpstk::CommonTime> IGPMapStore::getEpochTimes(void) const{

		std::vector<gpstk::CommonTime> epochs;
		IGPMapMap::const_iterator it2Map;

		for (it2Map = igpMaps.begin(); it2Map != igpMaps.end(); it2Map++){

			epochs.push_back(it2Map->first);
		}
		
		return epochs;
	}


	IonosphericGridPoint IGPMapStore::getIGP(gpstk::CommonTime &epoch, double lat, double lon) const {

		IonosphericGridPoint igp;

		IGPMapMap::const_iterator it2Map = igpMaps.find(epoch);

		if (it2Map != this->igpMaps.end()) {
			try
			{
				igp = it2Map->second.getIGP(epoch, lat, lon);
			}
			catch (const std::exception& e)
			{
				throw(e);
			}
			
		}
		else {
			throw std::domain_error("Time doesn't match");
		}

		return igp;
	}



#pragma endregion
}