#pragma once
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

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
		VerticalIonoDelayInterpolator(IGPMap * const linkedMap);

		void VerticalIonoDelayInterpolator::gridPointSelectionCriteria(void);
		void interpolation4point(void);
		void setPP(IonosphericGridPoint newPP);
		IonosphericGridPoint getIGP(double lat, double lon);

		std::vector<IonosphericGridPoint> IGPs;
		IonosphericGridPoint ionoPP;

		int closestNumberFromLow(int n, int m);
		int closestNumberFromHigh(int n, int m);
		int secondclosestNumberFromLow(int n, int m);
		int secondclosestNumberFromHigh(int n, int m);

	private:
		IGPMap * Map;
		void registerIGPMap(IGPMap * const link2Map);
		void calculate_xpp_and_ypp(double &xpp, double &ypp);
		
	};
};