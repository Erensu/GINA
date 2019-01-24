#pragma once
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "IGPMap.hpp"

#include "IonexStream.hpp"
#include "IonexHeader.hpp"
#include "CivilTime.hpp"

namespace EGNOS {

	class IonexCompatible {
	public:

		virtual double getTEC(gpstk::CivilTime epoch, double lat, double lon) = 0;
		virtual double getRMS(gpstk::CivilTime epoch, double lat, double lon) = 0;
		virtual std::vector<gpstk::CivilTime> getEpochTimes(void) = 0;
		virtual void copy(IonexCompatible *target) = 0;
	};

	class SlantIonoDelay
	{
	public:

		static const double SlantIonoDelay::Re;
		static const double SlantIonoDelay::hI;

		bool validRoverPosotion = false;

		double rlat;
		double rlon;
		double rheight;

		double azimuthOfSatId;
		double elevationOfSatId;

		double ppLat, ppLon;

		void setRoverPosition(double lat, double lon, double height);
		void setazimuthOfSatId(double az, double el);

		void calculatePP(void);
		
	private:

	};

	class VerticalIonoDelayInterpolator	
	{

	public:
		typedef struct VerticesOfSquare {
			IonosphericGridPoint first;
			IonosphericGridPoint second;
			IonosphericGridPoint third;
			IonosphericGridPoint fourth;
		};

		VerticalIonoDelayInterpolator(void);
		~VerticalIonoDelayInterpolator(void);

		double VerticalIonoDelayInterpolator::interpolate(IGPMapBase& Map, IonosphericGridPoint& newPP);

	private:
		IonosphericGridPoint ionoPP;

		IonosphericGridPoint getIGP(IGPMapBase &Map, double lat, double lon);

		double grid5x5Interpolator(IGPMapBase& Map);
		double grid10x10Interpolator(IGPMapBase& Map);
		void getVerticesOf5x5Square(VerticesOfSquare& table, IGPMapBase& Map);
		void getVerticesOf10x10Square(VerticesOfSquare& table, IGPMapBase& Map);

		void setPP(IonosphericGridPoint newPP);
		double interpolation4point(double xpp, double ypp,
			double ionoDelay1,
			double ionoDelay2,
			double ionoDelay3,
			double ionoDelay4);
		double interpolation3point(double xpp, double ypp,
			double ionoDelay2,
			double ionoDelay3,
			double ionoDelay4);
		
		double symmetricInterpolator(double gridDistance, VerticesOfSquare table);	// Symmetric means that the grid distance is constant. It can be square and triangle as well.

		void calculate_xpp_and_ypp(	double &xpp,	double &ypp,
									double &lat1,	double &lat2,
									double &lon1,	double &lon2);

		void getNearestLatLot(double &lat1, double &lat2, double &lon1, double &lon2);
		void restrictLong(double *indegree);
		double restrictLong(double indegree);
		void restrictLonginDegree(double &indegree);
		double absDistanceOfLongitude(double lon1, double lon2);
		int closestNumberFromLow(int n, int m);
		int closestNumberFromHigh(int n, int m);
		int secondClosestNumberFromLow(int n, int m);
		int secondClosestNumberFromHigh(int n, int m);
	};

	class IonexCreator
	{
		typedef enum{
			TEC,
			RMS
		}dataType;

		public:
			IonexCreator(void);
			~IonexCreator(void);

			void setIonexData(IonexCompatible &Ionex);
			bool write2file(std::string newIonexFile);

		private:

			std::string ionexFile;
			gpstk::IonexHeader header;
			gpstk::IonexStream strm;
			std::vector<gpstk::CivilTime> epochs;
			IonexCompatible *ionoData;

			bool getMapEpochs(void);
			void createHeader(void);
			gpstk::IonexData createDataBlock(gpstk::CivilTime currentEpoch, int mapID, dataType type);
			double getData(gpstk::CivilTime currentEpoch, double currLat, double currLon, dataType type);

			void writeHeader(gpstk::IonexHeader &header);
			void writeData(gpstk::IonexData &data);

			std::string getCurrentTimeinStr(void);
			void openFile(void);
			void closeFile(void);

			int calcDim(int lat1, int lat2, double dlat);
			double calculateIntervalinSec(gpstk::CivilTime firstEpoch, gpstk::CivilTime secondEpoch);
			std::string typeString(dataType type);
	};
};