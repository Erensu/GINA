#include "IGPMap.hpp"
#include "IonoCorrection.hpp"

using namespace std;

namespace EGNOS {

	const double SlantIonoDelay::Re = 6378136.3;
	const double SlantIonoDelay::hI = 350000;

	VerticalIonoDelayInterpolator::VerticalIonoDelayInterpolator(IGPMap * const linkedMap) {
		try
		{
			this->registerIGPMap(linkedMap);
		}
		catch (const std::exception&)
		{
			throw(std::exception("Invalid input IGPMap pointer during the instance creation \n"));
		}
		
	}

	void SlantIonoDelay::setRoverPosition(double lat, double lon, double height) {
	
		this->rlat = lat;
		this->rlon = lon;
		this->rheight = height;

		validRoverPosotion = true;
	}

	void SlantIonoDelay::setazimuthOfSatId(double az, double el) {

		this->azimuthOfSatId = az;
		this->elevationOfSatId = el;
	}

	void SlantIonoDelay::calculatePP(void) {
	
		double centralAngle;
		
		centralAngle = M_PI - this->elevationOfSatId - asin(((Re + this->rheight) / (Re + hI)) * cos(this->elevationOfSatId));

		this->ppLat = asin(sin(this->rlat) * cos(centralAngle) + cos(this->rlat) * sin(centralAngle) * cos(this->azimuthOfSatId));


		if (
			(this->rlat > 70 * M_PI/360 && tan(centralAngle) * cos(this->azimuthOfSatId) > tan(M_PI/2 - this->rlat)) ||
			(this->rlat > -70 * M_PI / 360 && -tan(centralAngle) * cos(this->azimuthOfSatId) > tan(M_PI / 2 - this->rlat))
			) {
			ppLon = this->rlon + M_PI - asin(sin(centralAngle) * sin(this->azimuthOfSatId) / cos(this->ppLat));
		}
		else {
			ppLon = this->rlon + asin(sin(centralAngle) * sin(this->azimuthOfSatId) / cos(this->ppLat));
		}
	}

	void VerticalIonoDelayInterpolator::interpolation4point(double xpp, double ypp) {
	
		double ionoDelay1, ionoDelay2, ionoDelay3, ionoDelay4;
		double ionoDelayPP;

		ionoDelayPP =	xpp			* ypp		* ionoDelay1 +
						(1 - xpp)	* ypp		* ionoDelay2 +
						(1 - xpp)	* (1 - ypp)	* ionoDelay3 +
						xpp			* (1 - ypp)	* ionoDelay4;
	}

	void VerticalIonoDelayInterpolator::interpolation3point(double xpp, double ypp) {

		double ionoDelay2, ionoDelay3, ionoDelay4;
		double ionoDelayPP;

		ionoDelayPP =	ypp				* ionoDelay2 +
						1 - xpp - ypp	* ionoDelay3 +
						xpp				* ionoDelay4;
	}

	void VerticalIonoDelayInterpolator::calculate_xpp_and_ypp(	double &xpp,	double &ypp, 
																double &lat1,	double &lat2, 
																double &lon1,	double &lon2) {

		double deltaLongPP = ionoPP.lon - lon1;
		double deltaLatPP = ionoPP.lat - lat1;

		if (ionoPP.lat >= 85.0 || ionoPP.lat <= -85.0) {
			ypp = (abs(ionoPP.lat) - 85.0) / 10.0;
			xpp = ( (ionoPP.lon - lon1) / 90.0 ) * (1 - 2*ypp) + ypp;
		}
		else {
			xpp = deltaLongPP / (lon2 - lon1);
			ypp = deltaLatPP / (lat2 - lat1);
		}
	}

	void VerticalIonoDelayInterpolator::setPP(IonosphericGridPoint newPP) {
		this->ionoPP = newPP;
	}


	void VerticalIonoDelayInterpolator::gridPointSelectionCriteria(void) {
	
		if (abs(this->ionoPP.lat) <= 55.0 ) {
			this->getIGPwhenPPbetweenS55N55();
		}
	
	}
	
	IonosphericGridPoint VerticalIonoDelayInterpolator::getIGP(double lat, double lon) {

		try
		{
			return this->Map->getIGP(lat, lon);
		}
		catch (const std::exception& e)
		{
			throw std::exception("IGP is not found \n");
		}
		
	}

	void VerticalIonoDelayInterpolator::registerIGPMap(IGPMap * const link2Map) {
		if (link2Map == NULL) {
			throw(std::exception("Invalid IGPMap \n"));
		}
		this->Map = link2Map;
	}

	int VerticalIonoDelayInterpolator::closestNumberFromLow(int n, int m)
	{
		// find the quotient 
		int q = n / m;

		// 1st possible closest number 
		int n1 = m * q;

		// 2nd possible closest number 
		int n2 = (n * m) > 0 ? (m * (q + 1)) : (m * (q - 1));

		// if true, then n1 is the required closest number 
		if ((n - n1) > (n - n2))
			return n1;

		// else n2 is the required closest number     
		return n2;
	}

	int VerticalIonoDelayInterpolator::closestNumberFromHigh(int n, int m) {
		// find the quotient 
		int q = n / m;

		// 1st possible closest number 
		int n1 = m * q;

		// 2nd possible closest number 
		int n2 = (n * m) > 0 ? (m * (q + 1)) : (m * (q - 1));

		// if true, then n1 is the required closest number 
		if ((n - n1) < (n - n2))
			return n1;

		// else n2 is the required closest number     
		return n2;
	}

	int VerticalIonoDelayInterpolator::secondclosestNumberFromLow(int n, int m)
	{
		// find the quotient 
		int q = n / m;

		// 1st possible closest number 
		int n1 = m * (q - 1);

		// 2nd possible closest number 
		int n2 = (n * m) > 0 ? (m * (q + 2)) : (m * (q - 2));

		// if true, then n1 is the required closest number 
		if ((n - n1) > (n - n2))
			return n1;

		// else n2 is the required closest number     
		return n2;
	
	}

	int VerticalIonoDelayInterpolator::secondclosestNumberFromHigh(int n, int m)
	{
		// find the quotient 
		int q = n / m;

		// 1st possible closest number 
		//int n1 = m * (q - 1);
		int n1 = (n * m) < 0 ? (m * (q + 1)) : (m * (q - 1));

		// 2nd possible closest number 
		int n2 = (n * m) > 0 ? (m * (q + 2)) : (m * (q - 2));

		// if true, then n1 is the required closest number 
		if ((n - n1) < (n - n2))
			return n1;

		// else n2 is the required closest number     
		return n2;

	}

	void VerticalIonoDelayInterpolator::getIGPwhenPPbetweenS55N55(void) {
	
		double lat1, lat2, lon1, lon2;
		// try with 5x5 grid
		if (ionoPP.lat >= 0.0) {
			lat1 = closestNumberFromLow(ionoPP.lat, 5);
			lat2 = closestNumberFromHigh(ionoPP.lat, 5);
		}
		else {
			lat2 = closestNumberFromLow(ionoPP.lat, 5);
			lat1 = closestNumberFromHigh(ionoPP.lat, 5);
		}
		
		if (ionoPP.lon >= 0.0) {
			lon1 = closestNumberFromLow(ionoPP.lon, 5);
			lon2 = closestNumberFromHigh(ionoPP.lon, 5);
		}
		else {
			lon2 = closestNumberFromLow(ionoPP.lon, 5);
			lon1 = closestNumberFromHigh(ionoPP.lon, 5);
		}

		modulo180(lon1);
		modulo180(lon2);
	
		
		
		
		//Map->getIGP
	}

	void VerticalIonoDelayInterpolator::modulo180(double &indegree) {
	
		if (indegree > 180) {
			indegree = fmod(indegree, 180);
			return;
		}

		if (indegree > -180) {
			indegree = fmod(indegree, 180);
			return;
		}
	}
};