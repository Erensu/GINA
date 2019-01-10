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

	void VerticalIonoDelayInterpolator::interpolation4point(void) {
	
		double ionoDelay1, ionoDelay2, ionoDelay3, ionoDelay4;
		double ionoDelayPP;
		double xpp, ypp;

		calculate_xpp_and_ypp(xpp, ypp);

		ionoDelayPP =	xpp			* ypp		* ionoDelay1 +
						(1 - xpp)	* ypp		* ionoDelay2 +
						(1 - xpp)	* (1 - ypp)	* ionoDelay3 +
						xpp			* (1 - ypp)	* ionoDelay4;
	}

	void VerticalIonoDelayInterpolator::calculate_xpp_and_ypp(double &xpp, double &ypp) {

		double LongWest; // TODO
		double LongEast; // TODO
		double LongPP; // TODO
		double LatPP; // TODO
		double LatWest; // TODO
		double LatEast; // TODO

		double deltaLongPP = LongPP - LongWest;
		double deltaLatPP = LatPP - LatWest;
		if (LatPP >= 85.0 || LatPP <= -85.0) {
			ypp = (abs(LatPP) - 85.0) / 10.0;
			xpp = ( (LongPP - LongWest) / 90.0 ) * (1 - 2*ypp) + ypp;
		}
		else {
			xpp = deltaLongPP / (LongEast - LongWest);
			ypp = deltaLatPP / (LatEast - LatWest);
		}
		
	}

	void VerticalIonoDelayInterpolator::setPP(IonosphericGridPoint newPP) {
		this->ionoPP = newPP;
	}


	void VerticalIonoDelayInterpolator::gridPointSelectionCriteria(void) {
	
		if (abs(this->ionoPP.lat) <= 55.0 ) {
		
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

};