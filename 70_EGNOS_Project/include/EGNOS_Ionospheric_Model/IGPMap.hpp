#pragma once

#include <stdlib.h>
#include "IonosphericGridPoint.hpp"
#include <iterator> 
#include <map> 

namespace EGNOS {

	
	typedef struct IGPCoordinate_s;
	typedef struct IGPCoordinate_s{
		int lat;
		int lon;

			

		bool const operator<(const IGPCoordinate_s &o) const {
			return lat < o.lat || (lat == o.lat && lon < o.lon);
		}
	}IGPCoordinate;

	class IGPMap
	{
		public:

			IGPMap(void);
			~IGPMap(void);

			void setIGPCandidates(const std::vector<IonosphericGridPoint> & const candidateIGPs);
			void updateIGPCandidate( const IonosphericGridPointMasksMessageParser  & const IGPMessageParser);
			void updateMap(void);

			friend std::ostream &operator<<(std::ostream &os, IGPMap const &imap);

		private:

			std::vector<IonosphericGridPoint> candidateIGPs;
			std::map<IGPCoordinate, IonosphericGridPoint>  Map;
	};

};