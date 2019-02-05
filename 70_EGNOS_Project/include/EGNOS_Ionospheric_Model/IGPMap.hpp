#pragma once

#include <stdlib.h>
#include <iterator> 
#include <map> 

#include "CivilTime.hpp"

#include "IonosphericGridPoint.hpp"

namespace EGNOS {
	
	typedef struct IGPCoordinate_s;
	typedef struct IGPCoordinate_s{
		double lat;
		double lon;
		bool const operator<(const IGPCoordinate_s &o) const {
			return lat < o.lat || (abs(lat - o.lat) < 10e-5 && lon < o.lon);
		}
	}IGPCoordinate;

	class IonexCompatible {
	public:
		virtual ~IonexCompatible() {};
		virtual double getTEC(gpstk::CivilTime epoch, double lat, double lon) const = 0;
		virtual double getRMS(gpstk::CivilTime epoch, double lat, double lon) const = 0;
		virtual std::vector<gpstk::CivilTime> getEpochTimes(void) const = 0;
		virtual IonexCompatible* clone() const = 0;
	};

	class IGPMapBase {
	public:

		virtual IonosphericGridPoint getIGP(double lat, double lon) const = 0;
	};

	class IGPMap:public IGPMapBase, public IonexCompatible
	{
		public:

			IGPMap(void);
			IGPMap(IGPMap* original);
			~IGPMap(void);

			double getTEC(gpstk::CivilTime epoch, double lat, double lon) const;
			double getRMS(gpstk::CivilTime epoch, double lat, double lon) const;
			std::vector<gpstk::CivilTime> getEpochTimes(void) const;
			IonexCompatible* clone() const { return new IGPMap(*this); }

			void setIGPCandidates(const std::vector<IonosphericGridPoint> & const candidateIGPs);
			void updateIGPCandidate( const IonosphericGridPointMasksMessageParser  & const IGPMessageParser);
			void updateMap(void);
			
			void addIGPforDebugging(IonosphericGridPoint newIGP);
			void restrictLonginDegree(double &indegree) const;
			void reset(void);
			IonosphericGridPoint getIGP(double lat, double lon) const;

			friend std::ostream &operator<<(std::ostream &os, IGPMap const &imap);
			 
		private:
			
			std::vector<IonosphericGridPoint> candidateIGPs;
			std::map<IGPCoordinate, IonosphericGridPoint>  Map;
	};
};