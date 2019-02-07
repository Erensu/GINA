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

	class IGPMapBase {
	public:

		virtual IonosphericGridPoint getIGP(double lat, double lon) const = 0;
	};

	class IonexCompatibleMap : public IGPMapBase {
	public:
		virtual ~IonexCompatibleMap() {};
		virtual double getTEC(gpstk::CommonTime epoch, double lat, double lon) const = 0;
		virtual double getRMS(gpstk::CommonTime epoch, double lat, double lon) const = 0;
		virtual std::vector<gpstk::CommonTime> getEpochTimes(void) const = 0;
		virtual IonexCompatibleMap* clone() const = 0;
	};

	class IonexCompatibleInterPolator {
	public:
		virtual ~IonexCompatibleInterPolator() {};
		
		virtual double getTEC(IonexCompatibleMap *Map, gpstk::CommonTime epoch, double lat, double lon) = 0;
		virtual double getRMS(IonexCompatibleMap *Map, gpstk::CommonTime epoch, double lat, double lon) = 0;
		virtual IonexCompatibleInterPolator* clone() const = 0;
	};

	class IGPMediator 	
	{
		public:

			void updateTime(gpstk::CommonTime &time);
			std::vector<IonosphericGridPoint> IGPMediator::getIGPCandidates(void);
			void IGPMediator::setIGPCandidates(const std::vector<IonosphericGridPoint> & const candidateIGPs);
			void IGPMediator::updateIGPCandidate(const IonosphericGridPointMasksMessageParser & const IGPMessageParser);

			gpstk::CommonTime getReferencetime(void) {return currentDataTime;};

		private:
			std::vector<IonosphericGridPoint> candidateIGPs;
			gpstk::CommonTime currentDataTime;
	};

	

	class IGPMap:public IonexCompatibleMap
	{
		public:

			IGPMap(void) {};
			IGPMap(IGPMap* original);
			~IGPMap(void);

			double getTEC(gpstk::CommonTime epoch, double lat, double lon) const;
			double getRMS(gpstk::CommonTime epoch, double lat, double lon) const;
			std::vector<gpstk::CommonTime> getEpochTimes(void) const;
			IonexCompatibleMap* clone() const { return new IGPMap(*this); }

			bool updateMap(IGPMediator &mediator);
			
			void addIGPforDebugging(IonosphericGridPoint newIGP);
			void restrictLonginDegree(double &indegree) const;
			void reset(void);
			IonosphericGridPoint getIGP(double lat, double lon) const;

			friend std::ostream &operator<<(std::ostream &os, IGPMap const &imap);
			 
		private:
			gpstk::CommonTime referenceTime;
			std::map<IGPCoordinate, IonosphericGridPoint>  Map;
	};
};