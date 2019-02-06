
#include "IonoCorrection.hpp"


#define MAX_VALUE 9999
#define INVALID_VALUE MAX_VALUE

#define MAX_HEIGHT_OF_IONO_LAYER 300
#define MIN_HEIGHT_OF_IONO_LAYER 300
#define HEIGHT_DISTANCE 0
#define MAX_LAT 85
#define MIN_LAT -85
#define MAX_LON 180
#define MIN_LON -180
#define LAT_DISTANCE -5
#define LON_DISTANCE 5
#define	BASE_RADIUS 6371.4
#define	MAPDIMS 2
#define	EXPONENT -1
#define	VERSION 1.0
#define FILETYPE "IONOSPHERE MAPS"
#define	SYTEM "GPS" // TODO What shall be there instead of 'MIX'?
#define	FILEPROGRAM "GINA 1.0"
#define	FILEAGENCY "GINA Project"
#define DESCRIPTION_PART1 "Infrastructure: Robert Bosch Kft., Budapest"
#define DESCRIPTION_PART2 "Creator: Balazs Lupsic"
#define DESCRIPTION_PART3 "Contact adress: balazs.lupsic@hu.bosch.com"
#define DESCRIPTION_PART4 "Contact adress: balazs.lupsic@gmail.com"
#define	COMMENT_PART1 "TEC values in 0.1 [m] units;  9999, if no value available"
#define	MAPPINGFUNCTION "TBD" // TODO - to be discussed
#define	ELEVATION 0
#define	OBSERVABLEUSED "TEC is obtained from EGNOS"
#define NUMSTATIONS 0
#define NUMSVS 0


#define MAX_RANGE_FOR_INTERPOLATION 90

using namespace std;

namespace EGNOS {

	const double SlantIonoDelay::Re = 6378136.3;
	const double SlantIonoDelay::hI = 350000;

#pragma region SlantIonoDelay

	void SlantIonoDelay::setazimuthOfSatId(double az, double el) {

		this->azimuthOfSatId = az;
		this->elevationOfSatId = el;
	}

	void SlantIonoDelay::calculatePP(void) {

		double centralAngle;

		centralAngle = M_PI - this->elevationOfSatId - asin(((Re + this->rheight) / (Re + hI)) * cos(this->elevationOfSatId));

		this->ppLat = asin(sin(this->rlat) * cos(centralAngle) + cos(this->rlat) * sin(centralAngle) * cos(this->azimuthOfSatId));


		if (
			(this->rlat > 70 * M_PI / 360 && tan(centralAngle) * cos(this->azimuthOfSatId) > tan(M_PI / 2 - this->rlat)) ||
			(this->rlat > -70 * M_PI / 360 && -tan(centralAngle) * cos(this->azimuthOfSatId) > tan(M_PI / 2 - this->rlat))
			) {
			ppLon = this->rlon + M_PI - asin(sin(centralAngle) * sin(this->azimuthOfSatId) / cos(this->ppLat));
		}
		else {
			ppLon = this->rlon + asin(sin(centralAngle) * sin(this->azimuthOfSatId) / cos(this->ppLat));
		}
	}
#pragma endregion

#pragma region VerticalIonoDelayInterpolator

	VerticalIonoDelayInterpolator::VerticalIonoDelayInterpolator(void) {
	
	}

	VerticalIonoDelayInterpolator::~VerticalIonoDelayInterpolator(void) {
	
	}

	void SlantIonoDelay::setRoverPosition(double lat, double lon, double height) {
	
		this->rlat = lat;
		this->rlon = lon;
		this->rheight = height;

		validRoverPosotion = true;
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

	double VerticalIonoDelayInterpolator::interpolate(IGPMapBase& Map, IonosphericGridPoint &newPP) {

		bool interPolFailed = false;
		double rtv;
		setPP(newPP);
		
		if (abs(this->ionoPP.lat) <= 75.0 ) {
			try
			{
				rtv = this->grid5x5Interpolator(Map);
				return rtv;
			}
			catch (exception &e)
			{
				try
				{
					rtv = this->grid5x10Interpolator(Map);
					return rtv;
				}
				catch (exception &e)
				{
					try
					{
						rtv = this->grid10x10Interpolator(Map);
					}
					catch (exception &e)
					{
						interPolFailed = true;
					}
				}
			}
		}

		if (abs(this->ionoPP.lat) == 75.0) {
			IonosphericGridPoint igp = getHorizontallyInterpolatedVertices(Map, this->ionoPP.lat, this->ionoPP.lon, 5);
			if (igp.valid == true) {
				rtv = igp.getIonoCorr();
				return rtv;
			}
		}

		if (75 <= abs(this->ionoPP.lat) && abs(this->ionoPP.lat) <= 85.0) {
		
			try
			{
				rtv = this->grid10x10InterpolatorwHorizontalInterpolation(Map);
				return rtv;
			}
			catch (exception &e)
			{
				interPolFailed = true;
			}
		}

		if (abs(this->ionoPP.lat) == 85.0) {
			IonosphericGridPoint igp = getHorizontallyInterpolatedVertices(Map, this->ionoPP.lat, this->ionoPP.lon, 10);
			if (igp.valid == true) {
				rtv = igp.getIonoCorr();
				return rtv;
			}
		}

		if (abs(this->ionoPP.lat) >= 85.0) {
			
			try
			{
				rtv = this->polarInterpolator(Map);
				return rtv;
			}
			catch (exception &e)
			{
				interPolFailed = true;
			}
		}
		
		if (interPolFailed == true) {
			throw std::domain_error("Interppolation is not possible"); 
		}

		return rtv;
	}
	
	IonosphericGridPoint VerticalIonoDelayInterpolator::getIGP(IGPMapBase &Map, double lat, double lon) {

		try
		{
			return Map.getIGP(lat, lon);
		}
		catch (std::exception& e)
		{
			throw std::domain_error("Interppolation in is not possible");
		}
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

	int VerticalIonoDelayInterpolator::secondClosestNumberFromLow(int n, int m)
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

	int VerticalIonoDelayInterpolator::secondClosestNumberFromHigh(int n, int m)
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

	
	double VerticalIonoDelayInterpolator::polarInterpolator(IGPMapBase& Map) {

		VerticesOfSquare table;
		getPolarVertices(table, Map);


		double numberOfValidIGP = int(table.first.valid) + int(table.second.valid) + int(table.third.valid) + int(table.fourth.valid);

		if (numberOfValidIGP < 3) {
			throw std::domain_error("Interppolation is not possible");
		}

		if (table.first.valid && table.second.valid && table.third.valid && table.fourth.valid) {

			double ypp = (abs(ionoPP.lat) - 85) / 10;
			double xpp = restrictLong(ionoPP.lon - table.third.lon) / 90 * (1 - 2*ypp) + ypp;

			double corr = interpolation4point(xpp, ypp, table.first.getIonoCorr(), table.second.getIonoCorr(), table.third.getIonoCorr(), table.fourth.getIonoCorr());
			return corr;
		}

		if (table.first.valid == false) {
			
			double ypp = (abs(ionoPP.lat) - 85) / 10;
			double xpp = restrictLong(ionoPP.lon - table.third.lon) / 90 * (1 - 2 * ypp) + ypp;

			double corr = interpolation3point(xpp, ypp, table.second.getIonoCorr(), table.third.getIonoCorr(), table.fourth.getIonoCorr());
			return corr;
		}
		else if (table.second.valid == false) {
			
			double ypp = (abs(ionoPP.lat) - 85) / 10;
			double xpp = restrictLong(ionoPP.lon - table.third.lon) / 90 * (1 - 2 * ypp) + ypp;

			double corr = interpolation3point(xpp, ypp, table.first.getIonoCorr(), table.fourth.getIonoCorr(), table.third.getIonoCorr());
			return corr;
		}
		
		throw std::domain_error("Interppolation is not possible");
		return 0;
	}

	double VerticalIonoDelayInterpolator::grid5x5Interpolator(IGPMapBase& Map) {
	
		
		double corr;
		double latDistance = 5;
		double lonDistance = 5;
		VerticesOfSquare table;
		getVerticesOf5x5Square(table, Map);

		try
		{
			corr = symmetricInterpolator(latDistance, lonDistance, table);
		}
		catch (const std::exception&)
		{
			throw std::domain_error("Interppolation in 5x5 grid is not possible");
		}

		return corr;
	}

	double VerticalIonoDelayInterpolator::grid5x10Interpolator(IGPMapBase& Map) {

		double corr;
		double lonDistance = 10;
		double latDistance = 5;
		VerticesOfSquare table;

		getVerticesOf5x10Square(table, Map);

		try
		{
			corr = symmetricInterpolator(latDistance, lonDistance, table);
		}
		catch (const std::exception&)
		{
			throw std::domain_error("Interppolation in 5x10 grid is not possible");
		}

		return corr;
	}

	double VerticalIonoDelayInterpolator::grid10x10InterpolatorwHorizontalInterpolation(IGPMapBase& Map) {

		double corr;
		double lonDistance = 10;
		double latDistance = 10;
		VerticesOfSquare table;

		getVerticesOf10x10SquarewHorizontalInterpolation(table, Map);

		try
		{
			corr = symmetricInterpolator(latDistance, lonDistance, table);
		}
		catch (const std::exception&)
		{
			throw std::domain_error("Interppolation in 10x10 grid is not possible");
		}

		return corr;
	}

	double VerticalIonoDelayInterpolator::grid10x10Interpolator(IGPMapBase& Map) {

		double corr;
		double lonDistance = 10;
		double latDistance = 10;
		VerticesOfSquare table;

		getVerticesOf10x10Square(table, Map);

		try
		{
			corr = symmetricInterpolator(latDistance, lonDistance, table);
		}
		catch (const std::exception&)
		{
			throw std::domain_error("Interppolation in 10x10 grid is not possible");
		}

		return corr;
	}

	double VerticalIonoDelayInterpolator::symmetricInterpolator(double latDistance, double lonDistance, VerticesOfSquare table) {

		double numberOfValidIGP = int(table.first.valid) + int(table.second.valid) + int(table.third.valid) + int(table.fourth.valid);

		if (numberOfValidIGP < 3) {
			throw std::domain_error("Interppolation is not possible");
		}

		if (table.first.valid && table.second.valid && table.third.valid && table.fourth.valid) {

			double xpp = absDistanceOfLongitude(ionoPP.lon, table.third.lon) / lonDistance;
			double ypp = abs(ionoPP.lat - table.third.lat) / latDistance;

			double corr = interpolation4point(xpp, ypp, table.first.getIonoCorr(), table.second.getIonoCorr(), table.third.getIonoCorr(), table.fourth.getIonoCorr());
			return corr;
		}


		if (table.first.valid == false) {
			if (abs(ionoPP.lat - table.third.lat) <= latDistance - latDistance / lonDistance * absDistanceOfLongitude(table.third.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, table.third.lon) / lonDistance;
				double ypp = abs(ionoPP.lat - table.third.lat) / latDistance;

				double corr = interpolation3point(xpp, ypp, table.second.getIonoCorr(), table.third.getIonoCorr(), table.fourth.getIonoCorr());
				return corr;
			}
			else {
				throw std::domain_error("Interppolation is not possible");
			}
		}
		else if (table.second.valid == false) {
			if (abs(ionoPP.lat - table.fourth.lat) <= latDistance - latDistance / lonDistance * absDistanceOfLongitude(table.fourth.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, table.fourth.lon) / lonDistance;
				double ypp = abs(ionoPP.lat - table.fourth.lat) / latDistance;

				double corr = interpolation3point(xpp, ypp, table.first.getIonoCorr(), table.fourth.getIonoCorr(), table.third.getIonoCorr());
				return corr;
			}
			else {
				throw std::domain_error("Interppolation is not possible");
			}
		}
		else if (table.third.valid == false) {
			if (abs(ionoPP.lat - table.fourth.lat) >= latDistance - latDistance / lonDistance * absDistanceOfLongitude(table.second.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, table.first.lon) / lonDistance;
				double ypp = abs(ionoPP.lat - table.first.lat) / latDistance;

				double corr = interpolation3point(xpp, ypp, table.fourth.getIonoCorr(), table.first.getIonoCorr(), table.second.getIonoCorr());
				return corr;
			}
			else {
				throw std::domain_error("Interppolation is not possible");
			}
		}
		else if (table.fourth.valid == false) {
			if (abs(ionoPP.lat - table.third.lat) >= latDistance / lonDistance * absDistanceOfLongitude(table.third.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, table.second.lon) / lonDistance;
				double ypp = abs(ionoPP.lat - table.second.lat) / latDistance;

				double corr = interpolation3point(xpp, ypp, table.third.getIonoCorr(), table.second.getIonoCorr(), table.first.getIonoCorr());
				return corr;
			}
			else {
				throw std::domain_error("Interppolation is not possible");
			}
		}

		throw std::domain_error("Interppolation is not possible");
		return 0;
	}

	void VerticalIonoDelayInterpolator::getPolarVertices(VerticesOfSquare &table, IGPMapBase &Map) {

		IonosphericGridPoint igp1, igp2, igp3, igp4;

		if (ionoPP.lat >= 85) {

			try {
				igp1 = getIGP(Map, 85, 0);
			}
			catch (const std::exception&) {}

			try {
				igp2 = getIGP(Map, 85, 90);
			}
			catch (const std::exception&) {}
			{

			}
			try {
				igp3 = getIGP(Map, 85, 180);
			}
			catch (const std::exception&) {}

			try {
				igp4 = getIGP(Map, 85, -90);
			}
			catch (const std::exception&) {}
		}
		else if (ionoPP.lat <= -85) {

			try {
				igp1 = getIGP(Map, -85, 0);
			}
			catch (const std::exception&) {}

			try {
				igp2 = getIGP(Map, -85, 90);
			}
			catch (const std::exception&) {}

			try {
				igp3 = getIGP(Map, -85, 180);
			}
			catch (const std::exception&) {}

			try {
				igp4 = getIGP(Map, -85, -90);
			}
			catch (const std::exception&) {}
		}

		if (0 <= ionoPP.lon && ionoPP.lon <= 90) {
			table.first = igp3;
			table.second = igp4;
			table.third = igp1;
			table.fourth = igp2;
		}
		 else if (90 <= ionoPP.lon && ionoPP.lon <= 180) {
			table.first = igp4;
			table.second = igp1;
			table.third = igp2;
			table.fourth = igp3;
		}
		else if (-180 <= ionoPP.lon && ionoPP.lon <= -90) {
			 table.first = igp1;
			 table.second = igp2;
			 table.third = igp3;
			 table.fourth = igp4;
		 }
		else if (-90 <= ionoPP.lon && ionoPP.lon <= 0) {
			table.first = igp2;
			table.second = igp3;
			table.third = igp4;
			table.fourth = igp1;
		}
		
	}

	void VerticalIonoDelayInterpolator::getVerticesOf5x5Square(VerticesOfSquare& table, IGPMapBase& Map) {
	
		double lat1, lat2, lon1, lon2;
		getNearestLatLot(lat1, lat2, lon1, lon2);
		
		IonosphericGridPoint igp1, igp2, igp3, igp4;

		int numberOfValidIGP = 0;

		try
		{
			igp1 = getIGP(Map, lat2, lon2);
			numberOfValidIGP++;
		}
		catch (const std::exception&)
		{
			igp1.valid = false;
		}

		try
		{
			igp2 = getIGP(Map, lat2, lon1);
			numberOfValidIGP++;
		}
		catch (const std::exception&)
		{
			igp2.valid = false;
		}

		try
		{
			igp3 = getIGP(Map, lat1, lon1);
			numberOfValidIGP++;
		}
		catch (const std::exception&)
		{
			igp3.valid = false;
		}
		table.first = igp1;
		try
		{
			igp4 = getIGP(Map, lat1, lon2);
			numberOfValidIGP++;
		}
		catch (const std::exception&)
		{
			igp4.valid = false;
		}


		table.first = igp1;
		table.second = igp2;
		table.third = igp3;
		table.fourth = igp4;
	}

	void VerticalIonoDelayInterpolator::getVerticesOf5x10Square(VerticesOfSquare& table, IGPMapBase& Map) {

		double latDistance = 5;
		double lonDistance = 10;
		double numberOfValidIGP = 0;

		double lat1, lat2, lon1, lon2;
		getNearestLatLot(lat1, lat2, lon1, lon2);

		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp1, igp21, igp31, igp4;

			try
			{
				igp1 = getIGP(Map, lat2, lon2);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp1.valid = false;
			}

			try
			{
				igp21 = getIGP(Map, lat2, restrictLong(lon1-5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp21.valid = false;
			}

			try
			{
				igp31 = getIGP(Map, lat1, restrictLong(lon1-5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp31.valid = false;
			}

			try
			{
				igp4 = getIGP(Map, lat1, lon2);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp4.valid = false;
			}

			table.first = igp1;
			table.second = igp21;
			table.third = igp31;
			table.fourth = igp4;

			if (numberOfValidIGP == 4) {
				return;
			}

			if (numberOfValidIGP == 3) {
				if (igp1.valid == false) {
					if (abs(ionoPP.lat - igp31.lat) <= latDistance - latDistance/ lonDistance * absDistanceOfLongitude(igp31.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp21.valid == false) {
					if (abs(ionoPP.lat - igp4.lat) <= latDistance - latDistance / lonDistance * absDistanceOfLongitude(igp4.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp31.valid == false) {
					if (abs(ionoPP.lat - igp4.lat) >= latDistance - latDistance / lonDistance * absDistanceOfLongitude(igp21.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp4.valid == false) {
					if (abs(ionoPP.lat - igp31.lat) >= latDistance / lonDistance * absDistanceOfLongitude(igp31.lon, ionoPP.lon)) {
						return;
					}
				}
			}

		}

		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp11, igp2, igp3, igp41;

			try
			{
				igp11 = getIGP(Map, lat2, restrictLong(lon2 + 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp11.valid = false;
			}

			try
			{
				igp2 = getIGP(Map, lat2, lon1);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp2.valid = false;
			}

			try
			{
				igp3 = getIGP(Map, lat1, lon1);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp3.valid = false;
			}

			try
			{
				igp41 = getIGP(Map, lat1, restrictLong(lon2 + 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp41.valid = false;
			}

			table.first = igp11;
			table.second = igp2;
			table.third = igp3;
			table.fourth = igp41;

			if (numberOfValidIGP == 4) {
				return;
			}

			if (numberOfValidIGP == 3) {
				if (igp11.valid == false) {
					if (abs(ionoPP.lat - igp3.lat) <= latDistance - latDistance / lonDistance * absDistanceOfLongitude(igp3.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp2.valid == false) {
					if (abs(ionoPP.lat - igp41.lat) <= latDistance - latDistance / lonDistance * absDistanceOfLongitude(igp41.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp3.valid == false) {
					if (abs(ionoPP.lat - igp41.lat) >= latDistance - latDistance / lonDistance * absDistanceOfLongitude(igp2.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp41.valid == false) {
					if (abs(ionoPP.lat - igp3.lat) >= latDistance / lonDistance * absDistanceOfLongitude(igp3.lon, ionoPP.lon)) {
						return;
					}
				}
			}
		}

	}

	IonosphericGridPoint VerticalIonoDelayInterpolator::getHorizontallyInterpolatedVertices(IGPMapBase& Map, double lat, double lon, double increment) {

		IonosphericGridPoint interpolatedVert;
		IonosphericGridPoint interpolatedVertEast, interpolatedVertWest;

		lon = restrictLong(lon);
		increment = abs(increment);

		interpolatedVert.lat = lat;
		interpolatedVert.lon = lon;

		interpolatedVertEast = getHorizontallyNearestIGP(Map, lat, lon, lat, lon, increment);
		interpolatedVertWest = getHorizontallyNearestIGP(Map, lat, lon, lat, lon, -increment);

		if (interpolatedVertEast.valid == false || interpolatedVertWest.valid == false) {
			interpolatedVert.valid = false;
			return interpolatedVert;
		}

		double lonDistance = abs(restrictLong(interpolatedVertWest.lon - interpolatedVertEast.lon));
		if (lonDistance < 10e-3) {
			interpolatedVert.setIonoDelayinMeter( interpolatedVertWest.getIonoCorr() );
			interpolatedVert.valid = true;
			return interpolatedVert;
		}

		if (lonDistance > 90) {
			interpolatedVert.valid = false;
			return interpolatedVert;
		}

		double deltaIonoCorr = interpolatedVertEast.getIonoCorr() - interpolatedVertWest.getIonoCorr();
		double deltaLon = abs(restrictLong(lon - interpolatedVertWest.lon));

		double interpolatedIonoCorr = interpolatedVertWest.getIonoCorr() + deltaIonoCorr * deltaLon / lonDistance;
		interpolatedVert.setIonoDelayinMeter(interpolatedIonoCorr);

		
		interpolatedVert.valid = true;

		return interpolatedVert;
	}

	IonosphericGridPoint VerticalIonoDelayInterpolator::getHorizontallyNearestIGP(IGPMapBase& Map, double originalLat, double originalLon, double lat, double lon, double increment) {

		IonosphericGridPoint out;

		lon = restrictLong(lon);
		originalLon = restrictLong(originalLon);

		if (abs(restrictLong(originalLon - lon)) > MAX_RANGE_FOR_INTERPOLATION){
			return out;
		}

		try
		{
			out = getIGP(Map, lat, lon);
		}
		catch (const std::exception&)
		{
			out = getHorizontallyNearestIGP(Map, originalLat, originalLon, originalLat, lon + increment, increment);
		}

		return out;
	}

	void VerticalIonoDelayInterpolator::getVerticesOf10x10SquarewHorizontalInterpolation(VerticesOfSquare& table, IGPMapBase& Map) {

		double gridDistance = 10;
		double numberOfValidIGP = 0;

		double lat1, lat2, lon1, lon2;
		getNearestLatLot(lat1, lat2, lon1, lon2);


		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp1, igp21, igp33, igp42;

			try
			{
				if (abs(lat2 - 85) < 10e-3) {
					igp1 = getHorizontallyInterpolatedVertices(Map, lat2, lon2, 5);
				}
				else {
					igp1 = getIGP(Map, lat2, lon2);
				}

				if(igp1.valid == true)
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp1.valid = false;
			}

			try
			{
				if (abs(lat2 - 85) < 10e-3) {
					igp21 = getHorizontallyInterpolatedVertices(Map, lat2, restrictLong(lon1 - 5), 5);
				}
				else {
					igp21 = getIGP(Map, lat2, restrictLong(lon1 - 5));
				}

				if (igp21.valid == true)
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp21.valid = false;
			}

			try
			{
				igp33 = getIGP(Map, lat1 - 5, restrictLong(lon1 - 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp33.valid = false;
			}

			try
			{
				igp42 = getIGP(Map, lat1 - 5, lon2);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp42.valid = false;
			}

			table.first = igp1;
			table.second = igp21;
			table.third = igp33;
			table.fourth = igp42;

			if (numberOfValidIGP == 4) {
				return;
			}

			if (numberOfValidIGP == 3) {
				if (igp1.valid == false) {
					if (abs(ionoPP.lat - igp33.lat) <= gridDistance - absDistanceOfLongitude(igp33.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp21.valid == false) {
					if (abs(ionoPP.lat - igp42.lat) <= gridDistance - absDistanceOfLongitude(igp42.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp33.valid == false) {
					if (abs(ionoPP.lat - igp42.lat) >= gridDistance - absDistanceOfLongitude(igp21.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp42.valid == false) {
					if (abs(ionoPP.lat - igp33.lat) >= absDistanceOfLongitude(igp33.lon, ionoPP.lon)) {
						return;
					}
				}
			}

		}

		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp11, igp2, igp32, igp43;

			try
			{
				if (abs(lat2 - 85) < 10e-3) {
					igp11 = getHorizontallyInterpolatedVertices(Map, lat2, restrictLong(lon2 + 5), 5);
				}
				else {
					igp11 = getIGP(Map, lat2, restrictLong(lon2 + 5));
				}

				if (igp11.valid == true)
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp11.valid = false;
			}

			try
			{
				if (abs(lat2 - 85) < 10e-3) {
					igp2 = getHorizontallyInterpolatedVertices(Map, lat2, lon1, 5);
				}
				else {
					igp2 = getIGP(Map, lat2, lon1);
				}

				if (igp2.valid == true)
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp2.valid = false;
			}

			try
			{
				igp32 = getIGP(Map, lat1 - 5, lon1);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp32.valid = false;
			}

			try
			{
				igp43 = getIGP(Map, lat1 - 5, restrictLong(lon2 + 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp43.valid = false;
			}

			table.first = igp11;
			table.second = igp2;
			table.third = igp32;
			table.fourth = igp43;

			if (numberOfValidIGP == 4) {
				return;
			}

			if (numberOfValidIGP == 3) {
				if (igp11.valid == false) {
					if (abs(ionoPP.lat - igp32.lat) <= gridDistance - absDistanceOfLongitude(igp32.lon, ionoPP.lon), 5) {
						return;
					}
				}
				else if (igp2.valid == false) {
					if (abs(ionoPP.lat - igp43.lat) <= gridDistance - absDistanceOfLongitude(igp43.lon, ionoPP.lon), 5) {
						return;
					}
				}
				else if (igp32.valid == false) {
					if (abs(ionoPP.lat - igp43.lat) >= gridDistance - absDistanceOfLongitude(igp2.lon, ionoPP.lon), 5) {
						return;
					}
				}
				else if (igp43.valid == false) {
					if (abs(ionoPP.lat - igp32.lat) >= absDistanceOfLongitude(igp32.lon, ionoPP.lon)) {
						return;
					}
				}
			}
		}

		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp13, igp22, igp3, igp41;

			try
			{
				if (abs(lat2 + 5 - 85) < 10e-3) {
					igp13 = getHorizontallyInterpolatedVertices(Map, lat2 + 5, restrictLong(lon2 + 5), 5);
				}
				else {
					igp13 = getIGP(Map, lat2 + 5, restrictLong(lon2 + 5));
				}

				if (igp13.valid == true)
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp13.valid = false;
			}

			try
			{
				if (abs(lat2 + 5 - 85) < 10e-3) {
					igp22 = getHorizontallyInterpolatedVertices(Map, lat2 + 5, lon1, 5);
				}
				else {
					igp22 = getHorizontallyInterpolatedVertices(Map, lat2 + 5, lon1, 5);
				}

				if (igp22.valid == true)
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp22.valid = false;
			}

			try
			{
				igp3 = getIGP(Map, lat1, lon1);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp3.valid = false;
			}

			try
			{
				igp41 = getIGP(Map, lat1, restrictLong(lon2 + 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp41.valid = false;
			}

			table.first = igp13;
			table.second = igp22;
			table.third = igp3;
			table.fourth = igp41;

			if (numberOfValidIGP == 4) {
				return;
			}

			if (numberOfValidIGP == 3) {
				if (igp13.valid == false) {
					if (abs(ionoPP.lat - igp3.lat) <= gridDistance - absDistanceOfLongitude(igp3.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp22.valid == false) {
					if (abs(ionoPP.lat - igp41.lat) <= gridDistance - absDistanceOfLongitude(igp41.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp3.valid == false) {
					if (abs(ionoPP.lat - igp41.lat) >= gridDistance - absDistanceOfLongitude(igp22.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp41.valid == false) {
					if (abs(ionoPP.lat - igp3.lat) >= absDistanceOfLongitude(igp3.lon, ionoPP.lon)) {
						return;
					}
				}
			}
		}

		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp12, igp23, igp31, igp4;

			try
			{
				if (abs(lat2 + 5 - 85) < 10e-3) {
					igp12 = getHorizontallyInterpolatedVertices(Map, lat2 + 5, lon2, 5);
				}
				else {
					igp12 = getIGP(Map, lat2 + 5, lon2);
				}

				if (igp12.valid == true)
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp12.valid = false;
			}

			try
			{
				if (abs(lat2 + 5 - 85) < 10e-3) {
					igp23 = getHorizontallyInterpolatedVertices(Map, lat2 + 5, restrictLong(lon1 - 5), 5);
				}
				else {
					igp23 = getIGP(Map, lat2 + 5, restrictLong(lon1 - 5));
				}

				if (igp23.valid == true)
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp23.valid = false;
			}

			try
			{
				igp31 = getIGP(Map, lat1, restrictLong(lon1 - 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp31.valid = false;
			}

			try
			{
				igp4 = getIGP(Map, lat1, lon2);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp4.valid = false;
			}

			table.first = igp12;
			table.second = igp23;
			table.third = igp31;
			table.fourth = igp4;

			if (numberOfValidIGP == 4) {
				return;
			}

			if (numberOfValidIGP == 3) {
				if (igp12.valid == false) {
					if (abs(ionoPP.lat - igp31.lat) <= gridDistance - absDistanceOfLongitude(igp31.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp23.valid == false) {
					if (abs(ionoPP.lat - igp4.lat) <= gridDistance - absDistanceOfLongitude(igp4.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp31.valid == false) {
					if (abs(ionoPP.lat - igp4.lat) >= gridDistance - absDistanceOfLongitude(igp23.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp4.valid == false) {
					if (abs(ionoPP.lat - igp31.lat) >= absDistanceOfLongitude(igp31.lon, ionoPP.lon)) {
						return;
					}
				}
			}
		}

	}

	void VerticalIonoDelayInterpolator::getVerticesOf10x10Square(VerticesOfSquare& table, IGPMapBase& Map) {

		double gridDistance = 10;
		double numberOfValidIGP = 0;

		double lat1, lat2, lon1, lon2;
		getNearestLatLot(lat1, lat2, lon1, lon2);

		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp1, igp21, igp33, igp42;

			try
			{
				igp1 = getIGP(Map, lat2, lon2);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp1.valid = false;
			}

			try
			{
				double temp = restrictLong(lon1 - 5);
				igp21 = getIGP(Map, lat2, temp);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp21.valid = false;
			}

			try
			{
				igp33 = getIGP(Map, lat1 - 5, restrictLong(lon1 - 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp33.valid = false;
			}

			try
			{
				igp42 = getIGP(Map, lat1 - 5, lon2);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp42.valid = false;
			}

			table.first = igp1;
			table.second = igp21;
			table.third = igp33;
			table.fourth = igp42;

			if (numberOfValidIGP == 4) {
				return; 
			}

			if (numberOfValidIGP == 3) {
				if (igp1.valid == false) {
					if (abs(ionoPP.lat - igp33.lat) <= gridDistance - absDistanceOfLongitude(igp33.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp21.valid == false) {
					if (abs(ionoPP.lat - igp42.lat) <= gridDistance - absDistanceOfLongitude(igp42.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp33.valid == false) {
					if (abs(ionoPP.lat - igp42.lat) >= gridDistance - absDistanceOfLongitude(igp21.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp42.valid == false) {
					if (abs(ionoPP.lat - igp33.lat) >= absDistanceOfLongitude(igp33.lon, ionoPP.lon)) {
						return;
					}
				}
			}

		}

		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp11, igp2, igp32, igp43;
		
			try
			{
				igp11 = getIGP(Map, lat2, restrictLong(lon2 + 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp11.valid = false;
			}

			try
			{
				igp2 = getIGP(Map, lat2, lon1);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp2.valid = false;
			}

			try
			{
				igp32 = getIGP(Map, lat1 - 5, lon1);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp32.valid = false;
			}

			try
			{
				igp43 = getIGP(Map, lat1 - 5, restrictLong(lon2 + 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp43.valid = false;
			}

			table.first = igp11;
			table.second = igp2;
			table.third = igp32;
			table.fourth = igp43;

			if (numberOfValidIGP == 4) {
				return;
			}

			if (numberOfValidIGP == 3) {
				if (igp11.valid == false) {
					if (abs(ionoPP.lat - igp32.lat) <= gridDistance - absDistanceOfLongitude(igp32.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp2.valid == false) {
					if (abs(ionoPP.lat - igp43.lat) <= gridDistance - absDistanceOfLongitude(igp43.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp32.valid == false) {
					if (abs(ionoPP.lat - igp43.lat) >= gridDistance - absDistanceOfLongitude(igp2.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp43.valid == false) {
					if (abs(ionoPP.lat - igp32.lat) >= absDistanceOfLongitude(igp32.lon, ionoPP.lon)) {
						return;
					}
				}
			}
		}

		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp13, igp22, igp3, igp41;

			try
			{
				igp13 = getIGP(Map, lat2 + 5, restrictLong(lon2 + 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp13.valid = false;
			}

			try
			{
				igp22 = getIGP(Map, lat2 + 5, lon1);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp22.valid = false;
			}

			try
			{
				igp3 = getIGP(Map, lat1, lon1);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp3.valid = false;
			}

			try
			{
				igp41 = getIGP(Map, lat1, restrictLong(lon2 + 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp41.valid = false;
			}

			table.first = igp13;
			table.second = igp22;
			table.third = igp3;
			table.fourth = igp41;

			if (numberOfValidIGP == 4) {
				return;
			}

			if (numberOfValidIGP == 3) {
				if (igp13.valid == false) {
					if (abs(ionoPP.lat - igp3.lat) <= gridDistance - absDistanceOfLongitude(igp3.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp22.valid == false) {
					if (abs(ionoPP.lat - igp41.lat) <= gridDistance - absDistanceOfLongitude(igp41.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp3.valid == false) {
					if (abs(ionoPP.lat - igp41.lat) >= gridDistance - absDistanceOfLongitude(igp22.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp41.valid == false) {
					if (abs(ionoPP.lat - igp3.lat) >= absDistanceOfLongitude(igp3.lon, ionoPP.lon)) {
						return;
					}
				}
			}
		}

		numberOfValidIGP = 0;
		{
			IonosphericGridPoint igp12, igp23, igp31, igp4;

			try
			{
				igp12 = getIGP(Map, lat2 + 5, lon2);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp12.valid = false;
			}

			try
			{
				igp23 = getIGP(Map, lat2 + 5, restrictLong(lon1 - 5) );
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp23.valid = false;
			}

			try
			{
				igp31 = getIGP(Map, lat1, restrictLong(lon1 - 5));
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp31.valid = false;
			}

			try
			{
				igp4 = getIGP(Map, lat1, lon2);
				numberOfValidIGP++;
			}
			catch (const std::exception&)
			{
				igp4.valid = false;
			}

			table.first = igp12;
			table.second = igp23;
			table.third = igp31;
			table.fourth = igp4;

			if (numberOfValidIGP == 4) {
				return;
			}

			if (numberOfValidIGP == 3) {
				if (igp12.valid == false) {
					if (abs(ionoPP.lat - igp31.lat) <= gridDistance - absDistanceOfLongitude(igp31.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp23.valid == false) {
					if (abs(ionoPP.lat - igp4.lat) <= gridDistance - absDistanceOfLongitude(igp4.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp31.valid == false) {
					if (abs(ionoPP.lat - igp4.lat) >= gridDistance - absDistanceOfLongitude(igp23.lon, ionoPP.lon)) {
						return;
					}
				}
				else if (igp4.valid == false) {
					if (abs(ionoPP.lat - igp31.lat) >= absDistanceOfLongitude(igp31.lon, ionoPP.lon)) {
						return;
					}
				}
			}
		}

	}
	
	void VerticalIonoDelayInterpolator::getNearestLatLot(double &lat1, double &lat2, double &lon1, double &lon2) {
	
		lat1 = closestNumberFromLow(ionoPP.lat, 5);
		lat2 = closestNumberFromHigh(ionoPP.lat, 5);
	
		lon1 = closestNumberFromLow(ionoPP.lon, 5);
		lon2 = closestNumberFromHigh(ionoPP.lon, 5);
		
		restrictLong(&lon1);
		restrictLong(&lon2);
	}

	void VerticalIonoDelayInterpolator::restrictLong(double *indegree) {

		restrictLonginDegree(*indegree);
	}

	double VerticalIonoDelayInterpolator::restrictLong(double indegree) {

		restrictLonginDegree(indegree);
		return indegree;
	}

	void VerticalIonoDelayInterpolator::restrictLonginDegree(double &indegree) {

		if (indegree >= 180) {
			indegree = fmod(indegree, 180);
			indegree -= 180;
			return;
		}

		if (indegree < -180) {
			indegree = fmod(indegree, 180);
			indegree = 180 - fabs(indegree);
			return;
		}
	}

#pragma endregion

#pragma region IonexCreator

	IonexCreator::~IonexCreator(void){
		delete ionoData; 
	};

	bool IonexCreator::write2file(std::string newIonexFile) {

		bool validityFlag = false;
		ionexFile = newIonexFile;
		validityFlag = getMapEpochs();

		if (validityFlag == false) {
			return false;
		}

		openFile();

		createHeader();
		writeHeader(header);

		gpstk::IonexData tecData;
		gpstk::IonexData rmsData;
		int mapId = 0;
		for (size_t i = 0; i < epochs.size(); i++)
		{
			mapId = i;
			tecData = createDataBlock(epochs[i], mapId, TEC);
			writeData(tecData);

			rmsData = createDataBlock(epochs[i], mapId, RMS);
			writeData(rmsData);
		}

		closeFile();

	}

	void IonexCreator::setIonexData(IonexCompatible &Ionex) {

		ionoData = Ionex.clone();
	}

	void IonexCreator::openFile(void) {

		strm.open(ionexFile.c_str(), std::ios::out);
	}

	void IonexCreator::closeFile(void) {

		strm.close();
	}

	int IonexCreator::calcDim(int lat1, int lat2, double dlat) {

		if (dlat == 0) {
			return 1;
		}
		else {
			return int(abs((int(lat2) - int(lat1)) / dlat) + 1);
		}
	}

	void IonexCreator::createHeader(void) {

		gpstk::CivilTime firstEpoch= epochs[0];
		gpstk::CivilTime lastEpoch = epochs[epochs.size()-1];
		
		double intervalBetweenEpochinSec;
		try
		{
			intervalBetweenEpochinSec = calculateIntervalinSec(firstEpoch, lastEpoch);
		}
		catch (const std::exception&)
		{
			intervalBetweenEpochinSec = 0;
		}
		
		header.version = VERSION;
		header.fileType = FILETYPE;
		header.system = SYTEM; 
		header.fileProgram = FILEPROGRAM;
		header.fileAgency = FILEAGENCY;
		header.date = getCurrentTimeinStr();

		header.descriptionList.clear();
		header.descriptionList.push_back(DESCRIPTION_PART1);
		header.descriptionList.push_back(DESCRIPTION_PART2);
		header.descriptionList.push_back(DESCRIPTION_PART3);
		header.descriptionList.push_back(DESCRIPTION_PART4);

		header.commentList.push_back(COMMENT_PART1);

		header.firstEpoch = firstEpoch;
		header.lastEpoch = lastEpoch;
		header.interval = intervalBetweenEpochinSec;
		header.numMaps = epochs.size(); // TODO - if the recieved maps are fewer than the epoch number, the number of maps won't match with the declared number in header.
		header.mappingFunction = MAPPINGFUNCTION; // TODO - to be discussed
		header.elevation = ELEVATION;
		header.observablesUsed = OBSERVABLEUSED;
		header.numStations = NUMSTATIONS;
		header.numSVs = NUMSVS;
		header.baseRadius = BASE_RADIUS;
		header.mapDims = MAPDIMS;

		header.hgt[0] = MIN_HEIGHT_OF_IONO_LAYER;
		header.hgt[1] = MAX_HEIGHT_OF_IONO_LAYER;
		header.hgt[2] = HEIGHT_DISTANCE;

		header.lat[0] = MAX_LAT;
		header.lat[1] = MIN_LAT;
		header.lat[2] = LAT_DISTANCE;

		header.lon[0] = MIN_LON;
		header.lon[1] = MAX_LON;
		header.lon[2] = LON_DISTANCE;

		header.exponent = EXPONENT;
		header.auxData = "";

		header.svsmap.clear();
		header.auxDataFlag = false;

		header.valid = true;
	}

	std::string IonexCreator::getCurrentTimeinStr(void) {
	
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
		 std::string date(buffer);
		 return date;
	}

	gpstk::IonexData IonexCreator::createDataBlock(gpstk::CivilTime currentEpoch, int mapID, dataType type) {

		gpstk::IonexData iod;
		double lat1, lat2, dlat;
		double lon1, lon2, dlon;
		double hgt1, hgt2, dhgt;
		int dimlat, dimlon, dimhgt;

		lat1 = header.lat[0];
		lat2 = header.lat[1];
		dlat = header.lat[2];

		lon1 = header.lon[0];
		lon2 = header.lon[1];
		dlon = header.lon[2];

		hgt1 = header.hgt[0];
		hgt2 = header.hgt[1];
		dhgt = header.hgt[2];

		dimlat = calcDim(lat1, lat2, dlat);
		dimlon = calcDim(lon1, lon2, dlon);
		dimhgt = calcDim(hgt1, hgt2, dhgt);

		double numberOfValues = dimlat * dimlon;

		gpstk::Vector<double> values(numberOfValues);
		int counter = 0;

		double currLat = lat1;
		double currLon = lon1;

		while (counter < numberOfValues)
		{	
			values(counter) = getData(currentEpoch, currLat, currLon, type);

			if (abs(currLon - lon2) < dlon) {
				currLon = lon1 - dlon;
				currLat += dlat;
			}
			currLon += dlon;
			counter++;
		}

		iod.data = values;
		iod.mapID = mapID;

		iod.dim[0] = dimlat;
		iod.dim[1] = dimlon;
		iod.dim[2] = dimhgt;

		iod.exponent = header.exponent;
		iod.lat[0] = lat1;
		iod.lat[1] = lat2;
		iod.lat[2] = dlat;

		iod.lon[0] = lon1;
		iod.lon[1] = lon2;
		iod.lon[2] = dlon;

		iod.hgt[0] = hgt1;
		iod.hgt[1] = hgt2;
		iod.hgt[2] = dhgt;

		iod.valid = true;

		try
		{
			iod.type.type = typeString(type);
		}
		catch (const std::exception&)
		{
			iod.type.type = "UN";
		}
		
		iod.type.units = "10e" + std::to_string(header.exponent) + " meter";
		iod.type.description = "Total Electron Content map";

		iod.time = currentEpoch;

		return iod;
	}

	double IonexCreator::getData(gpstk::CivilTime currentEpoch, double currLat, double currLon, dataType type) {

		double rtv;
		switch (type)
		{
			case TEC:
				try	{
					rtv = ionoData->getTEC(currentEpoch, currLat, currLon);
				}
				catch (const std::exception&){
					rtv = INVALID_VALUE * std::pow(10, header.exponent);
				}
				break;

			case RMS:
				try	{
					rtv = ionoData->getRMS(currentEpoch, currLat, currLon);
				}
				catch (const std::exception&){
					rtv = INVALID_VALUE * std::pow(10, header.exponent);
				}
				break;

			default:
				rtv = INVALID_VALUE * std::pow(10, header.exponent);
				break;
		}
		return rtv;
	}
	void IonexCreator::writeHeader(gpstk::IonexHeader &header) {
		strm << header;
	}

	void IonexCreator::writeData(gpstk::IonexData &data) {
		strm << data;
	}

	bool IonexCreator::getMapEpochs(void) {

		bool validEpochs = false;
		try
		{
			epochs = ionoData->getEpochTimes();
			validEpochs = true;
		}
		catch (const std::exception&)
		{
			validEpochs = false;
		}

		return validEpochs;
	}

	double IonexCreator::calculateIntervalinSec(gpstk::CivilTime firstEpoch, gpstk::CivilTime secondEpoch) {

		double intervalBetweenEpochinSec;
		if (epochs.size() > 1) {
			if ((epochs[1].year == epochs[0].year) && (epochs[1].month == epochs[0].month) && (epochs[1].day == epochs[0].day)) {

				intervalBetweenEpochinSec = (epochs[1].hour - epochs[0].hour) * 3600 +
					(epochs[1].minute - epochs[0].minute) * 60 +
					(epochs[1].second - epochs[0].second);
			}
			else {
				intervalBetweenEpochinSec = 0;
			}
		}
		else {
			intervalBetweenEpochinSec = 0;
		}

		if (intervalBetweenEpochinSec == 0) {
			throw std::domain_error("Interval is 0 or noncalculable!");
		}

		return intervalBetweenEpochinSec;
	}

	std::string IonexCreator::typeString(dataType type) {

		std::string typestr;
		switch (type)
		{
			case TEC:
				typestr = "TEC";
				break;
			case RMS:
				typestr = "RMS";
				break;
			default:
				throw std::domain_error("OuT of dataType domain");
				break;
		}

		return typestr;
	}
#pragma endregion

};