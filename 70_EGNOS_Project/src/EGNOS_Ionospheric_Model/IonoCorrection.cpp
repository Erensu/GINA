
#include "IonoCorrection.hpp"

#define TEC_IN_METER 0.162372
#define MAX_RANGE_FOR_INTERPOLATION 90



using namespace std;

namespace EGNOS {

	const double SlantIonoDelay::Re = 6378136.3;
	const double SlantIonoDelay::hI = 350000;

#pragma region SlantIonoDelay

	double SlantIonoDelay::getSlantFactor(SlantIonoDelay_Input data) {

		double F;
		setRoverPosition(data.RoverPos.rlat, data.RoverPos.rlon, data.RoverPos.rheight);
		setazimuthOfSatId(data.SatVisibility.azimuthOfSatId, data.SatVisibility.elevationOfSatId);

		calculatePP();
		calculateSlantFactor();

		return F;
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
			(this->rlat > 70 * M_PI / 360 && tan(centralAngle) * cos(this->azimuthOfSatId) > tan(M_PI / 2 - this->rlat)) ||
			(this->rlat > -70 * M_PI / 360 && -tan(centralAngle) * cos(this->azimuthOfSatId) > tan(M_PI / 2 - this->rlat))
			) {
			ppLon = this->rlon + M_PI - asin(sin(centralAngle) * sin(this->azimuthOfSatId) / cos(this->ppLat));
		}
		else {
			ppLon = this->rlon + asin(sin(centralAngle) * sin(this->azimuthOfSatId) / cos(this->ppLat));
		}
	}

	double SlantIonoDelay::calculateSlantFactor(void) {
	
		double F;

		F = 1 / ( sqrt( 1 - pow((( Re * cos(elevationOfSatId)) / (Re + hI)), 2)));

		return 0;
	}

#pragma endregion

#pragma region VerticalIonoDelayInterpolator

	VerticalIonoDelayInterpolator::VerticalIonoDelayInterpolator(VerticalIonoDelayInterpolator* original) {

		this->ionoPP = original->ionoPP;
		this->incrementOfSearchWindowforHorizontalInterPolator = incrementOfSearchWindowforHorizontalInterPolator;
	}

	void SlantIonoDelay::setRoverPosition(double lat, double lon, double height) {
	
		this->rlat = lat;
		this->rlon = lon;
		this->rheight = height;
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

	double VerticalIonoDelayInterpolator::getTEC(IonexCompatibleMap *Map, gpstk::CommonTime epoch, double lat, double lon) {

		IonCorrandVar interPolatedValues;
		try
		{
			IGPMapBase *downCastedMap = static_cast<IGPMapBase*> (Map);

			IonosphericGridPoint targetPP;
			targetPP.lat = lat;
			targetPP.lon = lon;

			interPolatedValues = interpolate(*downCastedMap, targetPP);
		}
		catch (const std::exception& e)
		{
			throw(e);
		}
		
		return interPolatedValues.CorrinMeter;
	};

	double VerticalIonoDelayInterpolator::getRMS(IonexCompatibleMap *Map, gpstk::CommonTime epoch, double lat, double lon) {

		IonCorrandVar interPolatedValues;
		try
		{
			IGPMapBase *downCastedMap = static_cast<IGPMapBase*> (Map);

			IonosphericGridPoint targetPP;
			targetPP.lat = lat;
			targetPP.lon = lon;

			interPolatedValues = interpolate(*downCastedMap, targetPP);
		}
		catch (const std::exception& e)
		{
			throw(e);
		}

		return sqrt(interPolatedValues.Variance);
	};

	IonCorrandVar VerticalIonoDelayInterpolator::interpolate(IGPMapBase& Map, IonosphericGridPoint &newPP) {

		bool interPolFailed = false;
		IonCorrandVar rtv;
		setPP(newPP);
		
		if (abs(this->ionoPP.lat) < 75.0 ) {

			if (this->ionoPP.lat == 50 && this->ionoPP.lon >= -20 ) {
				cout << endl;
			}

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
						return rtv;
					}
					catch (exception &e)
					{
						interPolFailed = true;
					}
				}
			}
		}

		if (abs(this->ionoPP.lat) == 75.0) {
			IonosphericGridPoint igp = getHorizontallyInterpolatedVertices(Map, this->ionoPP.lat, this->ionoPP.lon, incrementOfSearchWindowforHorizontalInterPolator);
			if (igp.valid == true) {

				try
				{
					rtv.CorrinMeter = igp.getIonoCorr();
					rtv.Variance = igp.getIonoCorrVariance();
				}
				catch (const std::exception& e)
				{
					throw(e);
				}
				
				return rtv;
			}
		}

		if (75 < abs(this->ionoPP.lat) && abs(this->ionoPP.lat) < 85.0) {
		
			try
			{
				rtv = this->grid10x10InterpolatorwHorizontalInterpolation(Map);
				return rtv;
			}
			catch (exception &e)
			{
			}
		}

		if (abs(this->ionoPP.lat) == 85.0) {
			
			IonosphericGridPoint igp = getHorizontallyInterpolatedVertices(Map, this->ionoPP.lat, this->ionoPP.lon, incrementOfSearchWindowforHorizontalInterPolator);
			if (igp.valid == true) {
				try
				{
					rtv.CorrinMeter = igp.getIonoCorr();
					rtv.Variance = igp.getIonoCorrVariance();
				}
				catch (const std::exception& e)
				{
					throw(e);
				}
				
				return rtv;
			}
		}

		if (abs(this->ionoPP.lat) > 85.0) {
			
			try
			{
				rtv = this->polarInterpolator(Map);
				return rtv;
			}
			catch (exception &e)
			{
			}
		}
		
		
		throw std::domain_error("Interppolation is not possible"); 

		return rtv;
	}
	
	IonosphericGridPoint VerticalIonoDelayInterpolator::getIGP(IGPMapBase &Map, double lat, double lon) {

		try
		{
			IonosphericGridPoint igp = Map.getIGP(lat, lon);

			if(igp.getGIVEI() == 15){
				throw std::domain_error("IGP is not monitored in is not possible");
			}
			return igp;
		}
		catch (std::exception& e)
		{
			throw std::domain_error("IGP is not valid in is not possible");
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

	IonCorrandVar VerticalIonoDelayInterpolator::polarInterpolator(IGPMapBase& Map) {

		VerticesOfSquare table;
		getPolarVertices(table, Map);
		IonCorrandVar rtv;

		double numberOfValidIGP = int(table.first.valid) + int(table.second.valid) + int(table.third.valid) + int(table.fourth.valid);

		if (numberOfValidIGP < 3) {
			throw std::domain_error("Interppolation is not possible");
		}

		if (table.first.valid && table.second.valid && table.third.valid && table.fourth.valid) {

			double ypp = (abs(ionoPP.lat) - 85) / 10;
			double xpp = restrictLong(ionoPP.lon - table.third.lon) / 90 * (1 - 2*ypp) + ypp;

			try
			{
				rtv.CorrinMeter = interpolation4point(xpp, ypp, table.first.getIonoCorr(), table.second.getIonoCorr(), table.third.getIonoCorr(), table.fourth.getIonoCorr());
				rtv.Variance = interpolation4point(xpp, ypp, table.first.getIonoCorrVariance(), table.second.getIonoCorrVariance(), table.third.getIonoCorrVariance(), table.fourth.getIonoCorrVariance());
			}
			catch (const std::exception& e)
			{
				throw(e);
			}
			
			return rtv;
		}

		if (table.first.valid == false) {
			
			double ypp = (abs(ionoPP.lat) - 85) / 10;
			double xpp = restrictLong(ionoPP.lon - table.third.lon) / 90 * (1 - 2 * ypp) + ypp;

			try
			{
				rtv.CorrinMeter = interpolation3point(xpp, ypp, table.second.getIonoCorr(), table.third.getIonoCorr(), table.fourth.getIonoCorr());
				rtv.Variance = interpolation4point(xpp, ypp, table.second.getIonoCorrVariance(), table.third.getIonoCorrVariance(), table.fourth.getIonoCorrVariance(), table.fourth.getIonoCorrVariance());
			}
			catch (const std::exception& e)
			{
				throw(e);
			}

			return rtv;
		}
		else if (table.second.valid == false) {
			
			double ypp = (abs(ionoPP.lat) - 85) / 10;
			double xpp = restrictLong(ionoPP.lon - table.third.lon) / 90 * (1 - 2 * ypp) + ypp;

			try
			{
				rtv.CorrinMeter = interpolation3point(xpp, ypp, table.first.getIonoCorr(), table.fourth.getIonoCorr(), table.third.getIonoCorr());
				rtv.Variance = interpolation4point(xpp, ypp, table.first.getIonoCorrVariance(), table.fourth.getIonoCorrVariance(), table.third.getIonoCorrVariance(), table.fourth.getIonoCorrVariance());
			}
			catch (const std::exception& e)
			{
				throw(e);
			}

			return rtv;
		}
		
		throw std::domain_error("Interppolation is not possible");
		return rtv;
	}

	IonCorrandVar VerticalIonoDelayInterpolator::grid5x5Interpolator(IGPMapBase& Map) {
	
		
		IonCorrandVar corr;
		double latDistance = 5;
		double lonDistance = 5;
		VerticesOfSquare table;
		getVerticesOf5x5Square(table, Map);

		try
		{
			corr = Interpolator(latDistance, lonDistance, table);
		}
		catch (const std::exception&)
		{
			throw std::domain_error("Interppolation in 5x5 grid is not possible");
		}

		return corr;
	}

	IonCorrandVar VerticalIonoDelayInterpolator::grid5x10Interpolator(IGPMapBase& Map) {

		IonCorrandVar corr;
		double lonDistance = 10;
		double latDistance = 5;
		VerticesOfSquare table;

		getVerticesOf5x10Square(table, Map);

		try
		{
			corr = Interpolator(latDistance, lonDistance, table);
		}
		catch (const std::exception&)
		{
			throw std::domain_error("Interppolation in 5x10 grid is not possible");
		}

		return corr;
	}

	IonCorrandVar VerticalIonoDelayInterpolator::grid10x10InterpolatorwHorizontalInterpolation(IGPMapBase& Map) {

		IonCorrandVar corr;
		double lonDistance = 10;
		double latDistance = 10;
		VerticesOfSquare table;

		getVerticesOf10x10SquarewHorizontalInterpolation(table, Map);

		try
		{
			corr = Interpolator(latDistance, lonDistance, table);
		}
		catch (const std::exception&)
		{
			throw std::domain_error("Interppolation in 10x10 grid is not possible");
		}

		return corr;
	}

	IonCorrandVar VerticalIonoDelayInterpolator::grid10x10Interpolator(IGPMapBase& Map) {

		IonCorrandVar corr;
		double lonDistance = 10;
		double latDistance = 10;
		VerticesOfSquare table;

		getVerticesOf10x10Square(table, Map);

		try
		{
			corr = Interpolator(latDistance, lonDistance, table);
		}
		catch (const std::exception&)
		{
			throw std::domain_error("Interppolation in 10x10 grid is not possible");
		}

		return corr;
	}

	IonCorrandVar VerticalIonoDelayInterpolator::Interpolator(double latDistance, double lonDistance, VerticesOfSquare table) {

		double numberOfValidIGP = int(table.first.valid) + int(table.second.valid) + int(table.third.valid) + int(table.fourth.valid);
		IonCorrandVar rtv;

		if (numberOfValidIGP < 3) {
			throw std::domain_error("Interppolation is not possible");
		}

		if (table.first.valid && table.second.valid && table.third.valid && table.fourth.valid) {

			double xpp = absDistanceOfLongitude(ionoPP.lon, table.third.lon) / lonDistance;
			double ypp = abs(ionoPP.lat - table.third.lat) / latDistance;

			try
			{
				rtv.CorrinMeter = interpolation4point(xpp, ypp, table.first.getIonoCorr(), table.second.getIonoCorr(), table.third.getIonoCorr(), table.fourth.getIonoCorr());
				rtv.Variance = interpolation4point(xpp, ypp, table.first.getIonoCorrVariance(), table.second.getIonoCorrVariance(), table.third.getIonoCorrVariance(), table.fourth.getIonoCorrVariance());
			}
			catch (const std::exception& e)
			{
				throw(e);
			}

			return rtv;
		}


		if (table.first.valid == false) {
			if (abs(ionoPP.lat - table.third.lat) <= latDistance - latDistance / lonDistance * absDistanceOfLongitude(table.third.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, table.third.lon) / lonDistance;
				double ypp = abs(ionoPP.lat - table.third.lat) / latDistance;

				try
				{
					rtv.CorrinMeter = interpolation3point(xpp, ypp, table.second.getIonoCorr(), table.third.getIonoCorr(), table.fourth.getIonoCorr());
					rtv.Variance = interpolation4point(xpp, ypp, table.second.getIonoCorrVariance(), table.third.getIonoCorrVariance(), table.fourth.getIonoCorrVariance(), table.fourth.getIonoCorrVariance());
				}
				catch (const std::exception& e)
				{
					throw(e);
				}
				return rtv;
			}
			else {
				throw std::domain_error("Interppolation is not possible");
			}
		}
		else if (table.second.valid == false) {
			if (abs(ionoPP.lat - table.fourth.lat) <= latDistance - latDistance / lonDistance * absDistanceOfLongitude(table.fourth.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, table.fourth.lon) / lonDistance;
				double ypp = abs(ionoPP.lat - table.fourth.lat) / latDistance;

				try
				{
					rtv.CorrinMeter = interpolation3point(xpp, ypp, table.first.getIonoCorr(), table.fourth.getIonoCorr(), table.third.getIonoCorr());
					rtv.Variance = interpolation4point(xpp, ypp, table.first.getIonoCorrVariance(), table.fourth.getIonoCorrVariance(), table.third.getIonoCorrVariance(), table.fourth.getIonoCorrVariance());
				}
				catch (const std::exception& e)
				{
					throw(e);
				}

				return rtv;
			}
			else {
				throw std::domain_error("Interppolation is not possible");
			}
		}
		else if (table.third.valid == false) {
			if (abs(ionoPP.lat - table.fourth.lat) >= latDistance - latDistance / lonDistance * absDistanceOfLongitude(table.second.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, table.first.lon) / lonDistance;
				double ypp = abs(ionoPP.lat - table.first.lat) / latDistance;

				try
				{
					rtv.CorrinMeter = interpolation3point(xpp, ypp, table.fourth.getIonoCorr(), table.first.getIonoCorr(), table.second.getIonoCorr());
					rtv.Variance = interpolation4point(xpp, ypp, table.fourth.getIonoCorrVariance(), table.first.getIonoCorrVariance(), table.second.getIonoCorrVariance(), table.fourth.getIonoCorrVariance());
				}
				catch (const std::exception& e)
				{
					throw(e);
				}

				return rtv;
			}
			else {
				throw std::domain_error("Interppolation is not possible");
			}
		}
		else if (table.fourth.valid == false) {
			if (abs(ionoPP.lat - table.third.lat) >= latDistance / lonDistance * absDistanceOfLongitude(table.third.lon, this->ionoPP.lon)) {

				double xpp = absDistanceOfLongitude(ionoPP.lon, table.second.lon) / lonDistance;
				double ypp = abs(ionoPP.lat - table.second.lat) / latDistance;

				try
				{
					rtv.CorrinMeter = interpolation3point(xpp, ypp, table.third.getIonoCorr(), table.second.getIonoCorr(), table.first.getIonoCorr());
					rtv.Variance = interpolation4point(xpp, ypp, table.third.getIonoCorrVariance(), table.second.getIonoCorrVariance(), table.first.getIonoCorrVariance(), table.fourth.getIonoCorrVariance());
				}
				catch (const std::exception& e)
				{
					throw(e);
				}
				return rtv;
			}
			else {
				throw std::domain_error("Interppolation is not possible");
			}
		}

		throw std::domain_error("Interppolation is not possible");
		return rtv;
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

		double lat1, lat2, lon1, lon2;
		getNearestLatLot(lat1, lat2, lon1, lon2);

		try
		{
			interpolatedVertEast = getHorizontallyNearestIGP(Map, lat, lon, lat, lon2, increment);
			interpolatedVertWest = getHorizontallyNearestIGP(Map, lat, lon, lat, lon1, -increment);
		}
		catch (const std::exception& e)
		{
			interpolatedVert.valid = false;
			return interpolatedVert;
		}
		
		if (interpolatedVertEast.valid == true && interpolatedVertEast.lon == lon) {
			return interpolatedVertEast;
		}

		if (interpolatedVertWest.valid == true && interpolatedVertWest.lon == lon)
		{
			return interpolatedVertWest;
		}

		if (interpolatedVertEast.valid == false || interpolatedVertWest.valid == false) {
			interpolatedVert.valid = false;
			return interpolatedVert;
		}

		double lonDistance = abs(restrictLong(interpolatedVertWest.lon - interpolatedVertEast.lon));
		if (lonDistance < 10e-3) {
			interpolatedVert.setIonoDelayinMeter( interpolatedVertWest.getIonoCorr() );
			int givei = interpolatedVertWest.getGIVEI();

			if (givei == 15) {
				throw std::domain_error("Grid is not monitored");
			}

			interpolatedVert.setGIVEI(givei);
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


		int givei = interpolatedVertWest.getGIVEI();

		if (givei == 15) {
			throw std::domain_error("Grid is not monitored");
		}

		interpolatedVert.setGIVEI(givei);
		
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
					igp22 = getIGP(Map, lat2 + 5, lon1);
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

	const int IonexCreator::maxValue = 9999;
	const int IonexCreator::invalidValue = maxValue;

	IonexCreator::~IonexCreator(void){
		delete ionoData; 
		delete interPol;
	};

	bool IonexCreator::writeIGPMap2file(std::string newIonexFile) {

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

	void IonexCreator::setIonexData(IonexCompatibleMap &Ionex) {

		ionoData = Ionex.clone();
	}

	void IonexCreator::setInterpolator(IonexCompatibleInterPolator &interPol) {

		isInterPolatorSet = true;
		this->interPol = interPol.clone();
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

		switch (headerType)
		{
		case EGNOSMapType::world5x5_meter:
			exponent = -1;
			unit = unitType::meter;
			createHeader_World(5);
			break;
		case EGNOSMapType::world5x5_tec:
			exponent = 0;
			unit = unitType::tec;
			createHeader_World(5);
			break;
		case EGNOSMapType::world2_5x2_5_meter:
			exponent = -1;
			unit = unitType::meter;
			createHeader_World(2.5);
			break;
		case EGNOSMapType::world2_5x2_5_tec:
			exponent = 0;
			unit = unitType::tec;
			createHeader_World(2.5);
			break;
		case EGNOSMapType::europe5x5_tec:
			exponent = 0;
			unit = unitType::tec;
			createHeader_Europe(5);
			break;
		case EGNOSMapType::europe5x5_meter:
			exponent = -1;
			unit = unitType::meter;
			createHeader_Europe(5);
			break;
		case EGNOSMapType::europe2_5x2_5_tec:
			exponent = 0;
			unit = unitType::tec;
			createHeader_Europe(2.5);
			break;
		case EGNOSMapType::europe2_5x2_5_meter:
			exponent = -1;
			unit = unitType::meter;
			createHeader_Europe(2.5);
			break;
		default:
			unit = unitType::meter;
			exponent = -1;
			createHeader_World(5);
			break;
		}
		
	}

	void IonexCreator::createHeaderBase(void) {

		header.clear();
		gpstk::CivilTime firstEpoch = epochs[0];
		gpstk::CivilTime lastEpoch = epochs[epochs.size() - 1];

		double intervalBetweenEpochinSec;
		try
		{
			intervalBetweenEpochinSec = calculateIntervalinSec(firstEpoch, lastEpoch);
		}
		catch (const std::exception&)
		{
			intervalBetweenEpochinSec = 0;
		}

		header.version = version;
		header.fileType = fileType;
		header.system = system;
		header.fileProgram = fileProgram;
		header.fileAgency = fileAgency;
		header.date = getCurrentTimeinStr();

		header.descriptionList.clear();

		for (int i = 0; i < description.size(); i++) {

			header.descriptionList.push_back(description[i]);
		}

		switch (unit)
		{
		case unitType::tec:
			exponent = 0;
			header.exponent = exponent;
			header.commentList.push_back(comment_part1 + "10e" + std::to_string(header.exponent) + " [TEC] " + comment_part2);
			break;
		case unitType::meter:
			exponent = -1;
			header.exponent = exponent;
			header.commentList.push_back(comment_part1 + " 10e" + std::to_string(header.exponent) + " [m] " + comment_part2);
			break;
		default:
			exponent = 0;
			header.exponent = exponent;
			header.commentList.push_back(comment_part1 + "10e" + std::to_string(header.exponent) + " [TEC] " + comment_part2);
			break;
		}

		header.firstEpoch = firstEpoch;
		header.lastEpoch = lastEpoch;
		header.interval = intervalBetweenEpochinSec;
		header.numMaps = epochs.size(); // TODO - if the recieved maps are fewer than the epoch number, the number of maps won't match with the declared number in header.
		header.mappingFunction = this->mappingFunction; // TODO - to be discussed
		header.elevation = elevation;
		header.observablesUsed = observablesUsed;
		header.numStations = numStations;
		header.numSVs = numSVs;
		header.baseRadius = baseRadius;
		header.mapDims = mapDims;

		header.hgt[0] = minHeight;
		header.hgt[1] = maxHeight;
		header.hgt[2] = heightDistance;

		header.lat[0] = maxLat;
		header.lat[1] = minLat;
		header.lat[2] = latDistance;

		header.lon[0] = minLon;
		header.lon[1] = maxLon;
		header.lon[2] = lonDistance;

		header.auxData = "";

		header.svsmap.clear();
		header.auxDataFlag = false;

		header.valid = true;
	}

	void IonexCreator::createHeader_Europe(double gridSize){

		if (gridSize < 2.5) {
			throw std::domain_error("GridSize is too small. 2.5 degree is the smallest value");
		}

		maxLat = 90;
		minLat = 10;
		latDistance = -abs(gridSize);

		maxLon = 60;
		minLon = -60;
		lonDistance = abs(gridSize);

		createHeaderBase();
		
	}

	void IonexCreator::createHeader_World(double gridSize) {

		if (gridSize < 2.5) {
			throw std::domain_error("GridSize is too small. 2.5 degree is the smallest value");
		}

		maxLat = 90;
		minLat = -90;
		latDistance = -abs(gridSize);

		maxLon = 180;
		minLon = -180;
		lonDistance = abs(gridSize);
		
		createHeaderBase();
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

	gpstk::IonexData IonexCreator::createDataBlock(gpstk::CommonTime currentEpoch, int mapID, dataType type) {

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
			double TECorRMS = getData(currentEpoch, currLat, currLon, type);

			

			values(counter) = TECorRMS;

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
		if (unit == unitType::tec) {
			iod.type.units = "10e" + std::to_string(header.exponent) + " [TEC]";
		}
		else if (unit == unitType::meter) {
			iod.type.units = "10e" + std::to_string(header.exponent) + " [m]";
		}
		else {
			iod.type.units = "Unknown";
		}
		
		iod.type.description = "Total Electron Content map";

		iod.time = currentEpoch;

		return iod;
	}

	double IonexCreator::getData(gpstk::CommonTime currentEpoch, double currLat, double currLon, dataType type) {

		double rtv;
		switch (type)
		{
			case TEC:
				try	{
					if (isInterPolatorSet == true) {
						rtv = interPol->getTEC(ionoData, currentEpoch, currLat, currLon);
						
					}
					else {
						rtv = ionoData->getTEC(currentEpoch, currLat, currLon);
					}
				}
				catch (const std::exception&){
					rtv = invalidValue * std::pow(10, header.exponent);
					return rtv;
				}
				break;

			case RMS:
				try	{

					if (isInterPolatorSet == true) {
						rtv = interPol->getRMS(ionoData, currentEpoch, currLat, currLon);
						
					}
					else {
						rtv = ionoData->getRMS(currentEpoch, currLat, currLon);
					}

				}
				catch (const std::exception&){
					rtv = invalidValue * std::pow(10, header.exponent);
					return rtv;
				}
				break;

			default:
				rtv = invalidValue * std::pow(10, header.exponent);
				return rtv;
				break;
		}

		if (unit == unitType::tec) {
			rtv = rtv / TEC_IN_METER;
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

	double IonexCreator::calculateIntervalinSec(gpstk::CommonTime firstEpoch, gpstk::CommonTime secondEpoch) {
		
		gpstk::CommonTime timeDiff = firstEpoch;
			
		timeDiff - secondEpoch;
		gpstk::CivilTime civTime(timeDiff);

		double intervalBetweenEpochinSec;
		if (epochs.size() > 1) {
			if ((civTime.year == 0) && (civTime.month == 0) && (civTime.day == 0)) {

				intervalBetweenEpochinSec = civTime.hour * 3600 +
											civTime.minute * 60 +
											civTime.second;
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