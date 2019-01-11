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

	double VerticalIonoDelayInterpolator::interpolation4point(	double xpp, double ypp,
																double ionoDelay1,
																double ionoDelay2,
																double ionoDelay3,
																double ionoDelay4) {
		return			xpp			* ypp		* ionoDelay1 +
						(1 - xpp)	* ypp		* ionoDelay2 +
						(1 - xpp)	* (1 - ypp)	* ionoDelay3 +
						xpp			* (1 - ypp)	* ionoDelay4;
	}

	double VerticalIonoDelayInterpolator::interpolation3point(	double xpp, double ypp,
																double ionoDelay1, 
																double ionoDelay2, 
																double ionoDelay3) {
		return			ypp				* ionoDelay1 +
						(1 - xpp - ypp)	* ionoDelay2 +
						xpp				* ionoDelay3;
	}

	void VerticalIonoDelayInterpolator::calculate_xpp_and_ypp(	double &xpp,	double &ypp, 
																double &lat1,	double &lat2, 
																double &lon1,	double &lon2) {

		if (ionoPP.lat >= 85.0 || ionoPP.lat <= -85.0) {
			ypp = (abs(ionoPP.lat) - 85.0) / 10.0;
			xpp = ( (ionoPP.lon - lon1) / 90.0 ) * (1 - 2*ypp) + ypp;
		}
		else {
			xpp = ionoPP.lon - lon1 / (lon2 - lon1);
			ypp = ionoPP.lat - lat1 / (lat2 - lat1);
		}
	}

	void VerticalIonoDelayInterpolator::setPP(IonosphericGridPoint newPP) {
		this->ionoPP = newPP;
	}

	double VerticalIonoDelayInterpolator::gridPointSelectionCriteria(void) {
		double rtv = -255;
		if (abs(this->ionoPP.lat) <= 55.0 ) {
			rtv = this->getIGPwhenPPbetweenS55N55();
		}
		else {
		
		}

		return rtv;
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

	double VerticalIonoDelayInterpolator::absDistanceOfLongitude(double lon1, double lon2) {

		double result = abs(lon2 - lon1);

		if (result > 180) {
			return 360 - result;
		}
		else {
			return result;
		}

	}

	double VerticalIonoDelayInterpolator::getIGPwhenPPbetweenS55N55(void) {
	
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
	
		
		IonosphericGridPoint igp1, igp2, igp3, igp4;

		int numberOfValidIGP = 0;

		try
		{
			igp1 = Map->getIGP(lat2, lon2);
			numberOfValidIGP++;
		}
		catch (const std::exception&)
		{
			igp1.valid = false;
		}

		try
		{
			igp2 = Map->getIGP(lat2, lon1);
			numberOfValidIGP++;
		}
		catch (const std::exception&)
		{
			igp2.valid = false;
		}
		try
		{
			igp3 = Map->getIGP(lat1, lon1);
			numberOfValidIGP++;
		}
		catch (const std::exception&)
		{
			igp3.valid = false;
		}

		try
		{
			igp4 = Map->getIGP(lat1, lon2);
			numberOfValidIGP++;
		}
		catch (const std::exception&)
		{
			igp4.valid = false;
		}

		if (numberOfValidIGP < 3) {
			double corr = -255;
			return corr;
		}
		
		if (igp1.valid && igp2.valid && igp3.valid && igp4.valid) {

			double xpp = absDistanceOfLongitude(ionoPP.lon, igp3.lon) / 5;
			double ypp = abs(ionoPP.lat - igp3.lat) / 5;

			double corr =  interpolation4point(xpp, ypp, igp1.getIonoCorr(), igp2.getIonoCorr(), igp3.getIonoCorr(), igp4.getIonoCorr());
			return corr;
		}


		if (igp1.valid == false) {
			if ( abs(ionoPP.lat - igp3.lat) <= 5.0 - absDistanceOfLongitude(igp3.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, igp3.lon) / 5;
				double ypp = abs(ionoPP.lat - igp3.lat) / 5;

				double corr = interpolation3point(xpp, ypp, igp2.getIonoCorr(), igp3.getIonoCorr(), igp4.getIonoCorr());
				return corr;
			}
			else {
				double corr = -255;
				return corr; // TODO - here we switch to 10 x 10 grids
			}
		}
		else if (igp2.valid == false) {
			if (abs(ionoPP.lat - igp4.lat) <= 5.0 - absDistanceOfLongitude(igp4.lon, this->ionoPP.lon)) {
				
				double xpp = absDistanceOfLongitude(ionoPP.lon, igp4.lon) / 5;
				double ypp = abs(ionoPP.lat - igp4.lat) / 5;

				double corr = interpolation3point(xpp, ypp, igp1.getIonoCorr(), igp4.getIonoCorr(), igp3.getIonoCorr());
				return corr;
			}
			else {
				double corr = -255;
				return corr; // TODO - here we switch to 10 x 10 grids
			}
		}
		else if (igp3.valid == false) {
			if ( abs(ionoPP.lat - igp4.lat) >= 5.0 - absDistanceOfLongitude(igp2.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, igp1.lon) / 5;
				double ypp = abs(ionoPP.lat - igp1.lat) / 5;

				double corr = interpolation3point(xpp, ypp, igp4.getIonoCorr(), igp1.getIonoCorr(), igp2.getIonoCorr());
				return corr;
			}
			else {
				double corr = -255;
				return corr; // TODO - here we switch to 10 x 10 grids
			}
		}
		else if (igp4.valid == false) {
			if (abs(ionoPP.lat - igp3.lat) >= absDistanceOfLongitude(igp3.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, igp2.lon) / 5;
				double ypp = abs(ionoPP.lat - igp2.lat) / 5;

				double corr = interpolation3point(xpp, ypp, igp3.getIonoCorr(), igp2.getIonoCorr(), igp1.getIonoCorr());
				return corr;
			}
			else {
				double corr = -255;
				return corr; // TODO - here we switch to 10 x 10 grids
			}
		}
		
		
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