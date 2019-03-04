#ifndef EGNOS_IGPMAP_HPP
#define EGNOS_IGPMAP_HPP

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

		virtual IonosphericGridPoint getIGP(gpstk::CommonTime &epoch, double lat, double lon) const = 0;// { IonosphericGridPoint dummy; return dummy; };
	};

	class IonexCompatibleMap  {
	public:
		virtual ~IonexCompatibleMap() {};
		virtual double getTEC(gpstk::CommonTime &epoch, double lat, double lon) const = 0;
		virtual double getRMS(gpstk::CommonTime &epoch, double lat, double lon) const = 0;
		virtual std::vector<gpstk::CommonTime> getEpochTimes(void) const = 0;
		virtual IonexCompatibleMap* clone() const = 0;
	};

	class IonexCompatibleInterPolator {
	public:
		virtual ~IonexCompatibleInterPolator() {};
		
		virtual double getTEC(IonexCompatibleMap *Map, gpstk::CommonTime &epoch, double lat, double lon) = 0;
		virtual double getRMS(IonexCompatibleMap *Map, gpstk::CommonTime &epoch, double lat, double lon) = 0;
		virtual IonexCompatibleInterPolator* clone() const = 0;
	};

	class IGPMediator 	
	{
		public:

			void updateTime(gpstk::CommonTime &time);
			std::vector<IonosphericGridPoint> getIGPCandidates(void);
			void setIGPCandidates(const std::vector<IonosphericGridPoint> & candidateIGPs);
			void updateIGPCandidate(const IonosphericGridPointMasksMessageParser & IGPMessageParser);

			gpstk::CommonTime getReferencetime(void) {return currentDataTime;};

		private:
			std::vector<IonosphericGridPoint> candidateIGPs;
			gpstk::CommonTime currentDataTime;
	};

	

	class IGPMap:public IonexCompatibleMap , public IGPMapBase
	{
		public:

			IGPMap(void) {};
			IGPMap(IGPMap* original);
			~IGPMap(void);

			double getTEC(gpstk::CommonTime &epoch, double lat, double lon) const;
			double getRMS(gpstk::CommonTime &epoch, double lat, double lon) const;
			std::vector<gpstk::CommonTime> getEpochTimes(void) const;
			IonexCompatibleMap* clone() const { return new IGPMap(*this); }

			bool updateMap(IGPMediator &mediator);
			
			void addIGPforDebugging(IonosphericGridPoint newIGP);
			void restrictLonginDegree(double &indegree) const;
			void reset(void);
			IonosphericGridPoint getIGP(gpstk::CommonTime &epoch, double lat, double lon) const;

			friend std::ostream &operator<<(std::ostream &os, IGPMap const &imap);
			 
		private:

			gpstk::CommonTime referenceTime;
			std::map<IGPCoordinate, IonosphericGridPoint>  Map;
	};

	class IGPMapStore: public IonexCompatibleMap, public IGPMapBase {

		public:
			IGPMapStore(void) {};
			IGPMapStore(IGPMapStore* original);
			~IGPMapStore(void) {};

			double getTEC(gpstk::CommonTime &epoch, double lat, double lon) const;
			double getRMS(gpstk::CommonTime &epoch, double lat, double lon) const;
			std::vector<gpstk::CommonTime> getEpochTimes(void) const;
			IonosphericGridPoint getIGP(gpstk::CommonTime &epoch, double lat, double lon) const;
			IonexCompatibleMap* clone() const { return new IGPMapStore(*this); };

			IGPMapStore* copy() const { return new IGPMapStore(*this); };

			void addMap(gpstk::CommonTime &time, IGPMap &igp);
			
			IGPMap getIGPMap(gpstk::CommonTime &epoch);

		private:
			int numberOfMap = 0;

			void updateTime(gpstk::CommonTime &t);
			gpstk::CommonTime initialTime, finalTime;

			/// The key to this map is the time
			typedef std::map<gpstk::CommonTime, IGPMap> IGPMapMap;

			/// Map of IGP maps
			IGPMapMap igpMaps;
	};
};

#endif