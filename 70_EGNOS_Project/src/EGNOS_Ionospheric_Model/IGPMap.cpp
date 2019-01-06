#pragma once


#include "IGPMap.hpp"
#include "IonosphericMaskBand.hpp"

namespace EGNOS {

	IGPMap::IGPMap(void) {

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

	std::ostream &operator<<(std::ostream &os, IGPMap const &imap) {
	
		// Old display
		/*os << "Element of IGPs candidates" << std::endl;

		for (std::vector<IonosphericGridPoint>::const_iterator it = imap.candidateIGPs.begin(); it != imap.candidateIGPs.end(); ++it) {
			os << *it;
		}*/

		// New display
		os << "Size of the Iono Map is " << imap.Map.size() << std::endl;
		os << "Element of IGPs candidates" << std::endl;

		std::map<IGPCoordinate, IonosphericGridPoint>::const_iterator it;

		for (it = imap.Map.begin(); it != imap.Map.end(); it++){

			os << it->second << std::endl;
		}

		return os;
	}

}