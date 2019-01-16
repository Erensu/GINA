#pragma once

#include <stdlib.h>
#include "IonosphericGridPoint.hpp"
#include "IonoCorrection.hpp"
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

			friend class VerticalIonoDelayInterpolator;

			IGPMap(void);
			~IGPMap(void);

			void setIGPCandidates(const std::vector<IonosphericGridPoint> & const candidateIGPs);
			void updateIGPCandidate( const IonosphericGridPointMasksMessageParser  & const IGPMessageParser);
			void updateMap(void);
			
			void addIGPforDebugging(IonosphericGridPoint newIGP);
			void reset(void);
			IonosphericGridPoint getIGP(double lat, double lon) const;

			friend std::ostream &operator<<(std::ostream &os, IGPMap const &imap);
			 
			
		private:
			
			std::vector<VerticalIonoDelayInterpolator*> interPolList;
			
			std::vector<IonosphericGridPoint> candidateIGPs;
			std::map<IGPCoordinate, IonosphericGridPoint>  Map;
			void ereaseInterPolElementLink(VerticalIonoDelayInterpolator* obsoleteElement);
			void deleteAllInterPolElementLink(void);
			void deleteInterPolElementLink(VerticalIonoDelayInterpolator* obsolateElement);
			
	};

	
};