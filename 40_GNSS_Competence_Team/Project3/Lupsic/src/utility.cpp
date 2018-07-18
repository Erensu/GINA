
#include "utility.h"
#include "firstDGNSS_code.h"
#include "secondDGNSS_code.h"

namespace utility_firstDGNSS_code {

	using namespace firstDGNSS_code;

	static GPSEphemerisStore bcestore;

	void set_bcestore(GPSEphemerisStore in_bcestore) {

		bcestore = in_bcestore;
	}

	void setDGNSSNaviagtionCalculator_Base(gpstk::CommonTime time, vector<SatID> vid, vector<double> prv, double basePosition[3]) {

		GPSWeekSecond gpstime(time);

		int vsize = 0;

		for (int i = 0; i < vid.size(); i++) {
			if (vid[i].system == SatID::systemGPS) {
				vsize++;
			}
		}
		int* satId;
		double* pr;

		satId = (int*)malloc(sizeof(int)*vsize);
		pr = (double*)malloc(sizeof(double)*vsize);

		for (int i = 0; i < vsize; i++) {
			if (vid[i].system == SatID::systemGPS) {
				satId[i] = vid[i].id;
				pr[i] = prv[i];
			}
		}
		set_position_of_Base(basePosition);
		set_position_of_Rover(basePosition); // The first poision guess for Rover is the Base Station
		set_time_Base(gpstime.week, gpstime.sow);
		set_satId_Base(satId, vsize);
		set_pseudoRange_Base(pr, vsize);
	}

	void setDGNSSNaviagtionCalculator_Rover(gpstk::CommonTime time, vector<SatID> vid, vector<double> prv) {

		GPSWeekSecond gpstime(time);

		int vsize = 0;

		for (int i = 0; i < vid.size(); i++) {
			if (vid[i].system == SatID::systemGPS) {
				vsize++;
			}
		}
		int* satId;
		double* pr;

		satId = (int*)malloc(sizeof(int)*vsize);
		pr = (double*)malloc(sizeof(double)*vsize);

		for (int i = 0; i < vsize; i++) {
			if (vid[i].system == SatID::systemGPS) {
				satId[i] = vid[i].id;
				pr[i] = prv[i];
			}
		}

		set_time_Rover(gpstime.week, gpstime.sow);
		set_satId_Rover(satId, vsize);
		set_pseudoRange_Rover(pr, vsize);
	}

	int get_satPos(int wn, double tow, int satId, double* pos) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		GPSWeekSecond gpstime;
		gpstime.week = wn;
		gpstime.sow = tow;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);

			pos[0] = xvt.x[0];
			pos[1] = xvt.x[1];
			pos[2] = xvt.x[2];

			return 1;
		}
		catch (Exception& e) {

			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0;

			//cerr << e << endl;

			return 0;
		}
	}

	int get_satPos(double roverPos[3], int wn, double tow, int satId, double* pos) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		GPSWeekSecond gpstime;
		gpstime.week = wn;
		gpstime.sow = tow;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);
			Position rovPos(roverPos[0], roverPos[1], roverPos[2]);
			double elevation = rovPos.elevation(xvt.x);
			if (elevation > 0) {

				pos[0] = xvt.x[0];
				pos[1] = xvt.x[1];
				pos[2] = xvt.x[2];
				return elevation;
			}
			else {

				pos[0] = 0;
				pos[1] = 0;
				pos[2] = 0;
				return 0;
			}



			return 1;
		}
		catch (Exception& e) {

			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0;

			//cerr << e << endl;

			return 0;
		}
	}

	int get_satClock(int wn, double tow, int satId, double &clockbias) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		GPSWeekSecond gpstime;
		gpstime.week = wn;
		gpstime.sow = tow;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);
			clockbias = xvt.getClockBias();

			return 1;
		}
		catch (Exception& e) {

			//cerr << e << endl;

			return 0;
		}

	}

	int get_satRelCorr(int wn, double tow, int satId, double &relcorr) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		GPSWeekSecond gpstime;
		gpstime.week = wn;
		gpstime.sow = tow;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);
			relcorr = xvt.relcorr;

			return 1;
		}
		catch (Exception& e) {

			//cerr << e << endl;

			return 0;
		}
	}
}

namespace utility_secondDGNSS_code {

	using namespace secondDGNSS_code;

	static GPSEphemerisStore bcestore;

	void set_bcestore(GPSEphemerisStore in_bcestore) {

		bcestore = in_bcestore;
	}

	void setDGNSSNaviagtionCalculator_Base(gpstk::CommonTime time, vector<SatID> vid, vector<double> prv, double basePosition[3]) {

		GPSWeekSecond gpstime(time);

		int vsize = 0;

		for (int i = 0; i < vid.size(); i++) {
			if (vid[i].system == SatID::systemGPS) {
				vsize++;
			}
		}
		int* satId;
		double* pr;

		satId = (int*)malloc(sizeof(int)*vsize);
		pr = (double*)malloc(sizeof(double)*vsize);

		for (int i = 0; i < vsize; i++) {
			if (vid[i].system == SatID::systemGPS) {
				satId[i] = vid[i].id;
				pr[i] = prv[i];
			}
		}
		set_position_of_Base(basePosition);
		set_position_of_Rover(basePosition); // The first poision guess for Rover is the Base Station
		set_time_Base(gpstime.week, gpstime.sow);
		set_satId_Base(satId, vsize);
		set_pseudoRange_Base(pr, vsize);
	}

	void setDGNSSNaviagtionCalculator_Rover(gpstk::CommonTime time, vector<SatID> vid, vector<double> prv) {

		GPSWeekSecond gpstime(time);

		int vsize = 0;

		for (int i = 0; i < vid.size(); i++) {
			if (vid[i].system == SatID::systemGPS) {
				vsize++;
			}
		}
		int* satId;
		double* pr;

		satId = (int*)malloc(sizeof(int)*vsize);
		pr = (double*)malloc(sizeof(double)*vsize);

		for (int i = 0; i < vsize; i++) {
			if (vid[i].system == SatID::systemGPS) {
				satId[i] = vid[i].id;
				pr[i] = prv[i];
			}
		}

		set_time_Rover(gpstime.week, gpstime.sow);
		set_satId_Rover(satId, vsize);
		set_pseudoRange_Rover(pr, vsize);
	}

	bool get_satPos(int wn, double tow, int satId, double pos[3]) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		GPSWeekSecond gpstime;
		gpstime.week = wn;
		gpstime.sow = tow;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);

			pos[0] = xvt.x[0];
			pos[1] = xvt.x[1];
			pos[2] = xvt.x[2];

			return true;
		}
		catch (Exception& e) {

			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0;

			//cerr << e << endl;

			return false;
		}
	}

	bool get_satPos(double roverPos[3], int wn, double tow, int satId, double pos[3], double& elevation) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		GPSWeekSecond gpstime;
		gpstime.week = wn;
		gpstime.sow = tow;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);
			Position rovPos(roverPos[0], roverPos[1], roverPos[2]);
			elevation = rovPos.elevation(xvt.x);
			if (elevation > 0) {

				pos[0] = xvt.x[0];
				pos[1] = xvt.x[1];
				pos[2] = xvt.x[2];
				return true;
			}
			else {

				pos[0] = 0;
				pos[1] = 0;
				pos[2] = 0;
				return false;
			}



			return 1;
		}
		catch (Exception& e) {

			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0;

			//cerr << e << endl;

			return 0;
		}
	}

	int get_satClock(int wn, double tow, int satId, double &clockbias) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		GPSWeekSecond gpstime;
		gpstime.week = wn;
		gpstime.sow = tow;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);
			clockbias = xvt.getClockBias();

			return 1;
		}
		catch (Exception& e) {

			//cerr << e << endl;

			return 0;
		}

	}

	int get_satRelCorr(int wn, double tow, int satId, double &relcorr) {

		SatID id;
		id.id = satId;
		id.system = SatID::SatelliteSystem::systemGPS;

		GPSWeekSecond gpstime;
		gpstime.week = wn;
		gpstime.sow = tow;

		Xvt xvt;

		try {

			xvt = bcestore.getXvt(id, gpstime);
			relcorr = xvt.relcorr;

			return 1;
		}
		catch (Exception& e) {

			//cerr << e << endl;

			return 0;
		}
	}
}