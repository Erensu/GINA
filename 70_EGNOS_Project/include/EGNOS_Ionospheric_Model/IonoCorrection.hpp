#pragma once
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "IGPMap.hpp"

namespace EGNOS {

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
		
		friend class IGPMap;

		VerticalIonoDelayInterpolator(IGPMap * const linkedMap);
		~VerticalIonoDelayInterpolator(void);

		double VerticalIonoDelayInterpolator::gridPointSelectionCriteria(void);

		double interpolation4point(	double xpp, double ypp,
									double ionoDelay1,
									double ionoDelay2,
									double ionoDelay3,
									double ionoDelay4);
		double interpolation3point(	double xpp, double ypp,
									double ionoDelay2,
									double ionoDelay3,
									double ionoDelay4);

		

		void setPP(IonosphericGridPoint newPP);
		IonosphericGridPoint getIGP(double lat, double lon);

		std::vector<IonosphericGridPoint> IGPs;
		IonosphericGridPoint ionoPP;

		int closestNumberFromLow(int n, int m);
		int closestNumberFromHigh(int n, int m);
		int secondClosestNumberFromLow(int n, int m);
		int secondClosestNumberFromHigh(int n, int m);

		
	private:

		IGPMap * Map;							

		double grid5x5Interpolator(void);
		double grid10x10Interpolator(void);

		void getVerticesOf5x5Square(VerticesOfSquare& table);
		void getVerticesOf10x10Square(VerticesOfSquare& table);
		double symmetricInterpolator(double gridDistance, VerticesOfSquare table);

		void getNearestLatLot(double &lat1, double &lat2, double &lon1, double &lon2);
		void registerIGPMap(IGPMap * link2Map);
		void calculate_xpp_and_ypp(	double &xpp,	double &ypp,
									double &lat1,	double &lat2,
									double &lon1,	double &lon2);
		void restrictLong(double *indegree);
		double restrictLong(double indegree);
		void restrictLonginDegree(double &indegree);

		double absDistanceOfLongitude(double lon1, double lon2);
		void deleteLink2Map(void) {	Map = NULL;	}
	};
};