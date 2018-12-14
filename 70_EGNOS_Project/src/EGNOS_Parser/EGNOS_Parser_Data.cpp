
#include "EGNOS_ftp_data_Data.hpp"

namespace EGNOS_ftp_data_Parser
{
	using namespace gpstk::StringUtils;
	using namespace std;

	const string EGNOS_ftp_data_Data::startofDataTag = "START OF DATA";


	void EGNOS_ftp_data_Data::reallyPutRecord(gpstk::FFStream& ffs) const
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		EGNOS_ftp_data_Stream& strm = dynamic_cast<EGNOS_ftp_data_Stream&>(ffs);

		if (!strm.dataStartWritten) {
			strm << startofDataTag << endl;
			strm.lineNumber++;
			strm.dataStartWritten = true;
		}

		if (timeSys == gpstk::TimeSystem::Systems::GPS) {
			gpstk::GPSWeekSecond timeGPS(time);
			strm << timeGPS.getWeek() << "  ";
			strm << fixed << std::setprecision(5) << timeGPS.getSOW() << "  ";
		}
		else if (timeSys == gpstk::TimeSystem::Systems::GAL) {
			gpstk::GALWeekSecond timeGAL(time);
			strm << timeGAL.getWeek() << "  ";
			strm << fixed << std::setprecision(5) << timeGAL.getSOW() << "  ";
		}
		else {
			strm << " ??? " << " ??? ";
		}

		
		strm << fixed << std::setprecision(precision) << acceleration[0] << "  ";
		strm << fixed << std::setprecision(precision) << acceleration[1] << "  ";
		strm << fixed << std::setprecision(precision) << acceleration[2] << "  ";
		

		if (angularRate == nullptr) {
			strm << "0.0" << "  ";
			strm << "0.0" << "  ";
			strm << "0.0" << "  ";
		}
		else {
			strm << fixed << std::setprecision(precision) << angularRate[0] << "  ";
			strm << fixed << std::setprecision(precision) << angularRate[1] << "  ";
			strm << fixed << std::setprecision(precision) << angularRate[2] << "  ";
		}
		
		strm << endl;
		strm.lineNumber++;
	}

	/**
	* This function retrieves a RINEX NAV record from the given FFStream.
	* If an error is encountered in reading from the stream, the stream
	* is returned to its original position and its fail-bit is set.
	* @throws StringException when a StringUtils function fails
	* @throws FFStreamError when exceptions(failbit) is set and
	*  a read or formatting error occurs.  This also resets the
	*  stream to its pre-read position.
	*/
	void EGNOS_ftp_data_Data::reallyGetRecord(gpstk::FFStream& ffs)
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		EGNOS_ftp_data_Stream& strm = dynamic_cast<EGNOS_ftp_data_Stream&>(ffs);
		this->strm = &strm;

		string line;

		if (!strm.headerRead)
			strm >> strm.header;

		if (!strm.dataStartRead) {
			while (true) {

				strm.formattedGetLine(line);
				gpstk::StringUtils::stripTrailing(line);

				if (line == startofDataTag) {
					strm.dataStartRead = true;
					break;
				}
			}
		}

		if (coorSys == gpstk::Position::CoordinateSystem::Unknown) {
			this->coorSys = strm.header.coorSys;
		}
		if (timeSys == gpstk::TimeSystem::Systems::Unknown) {
			this->timeSys = strm.header.timeSys;
		}

		

		strm.formattedGetLine(line, true);
		gpstk::StringUtils::stripTrailing(line);
		parseLine(line);
		
		return;
	}

	bool EGNOS_ftp_data_Data::compare(const EGNOS_ftp_data_Data& other) const{

		bool rtv = true;
		if (this->acceleration[0]!= other.acceleration[0]) {
			rtv = false;
		}

		if (this->acceleration[1] != other.acceleration[1]) {
			rtv = false;
		}
		if (this->acceleration[2] != other.acceleration[2]) {
			rtv = false;
		}

		if (this->time != other.time) {
			rtv = false;
		}

		if (this->timeSys != other.timeSys) {
			rtv = false;
		}

		return rtv;
	}

	//bool operator== (const Triple& right) const;
	bool EGNOS_ftp_data_Data::operator==(const EGNOS_ftp_data_Data& other) const{
		return this->compare(other);
	}

	bool EGNOS_ftp_data_Data::operator!=(const EGNOS_ftp_data_Data& other) const {
		return !this->compare(other);
	}


	EGNOS_ftp_data_Data& EGNOS_ftp_data_Data::operator+=(EGNOS_ftp_data_Data& newData) {

		// TODO: This is not quit right here.
		if (this->time != newData.time) {
			throw ("Time value is not matching");
		} 

		this->acceleration[0] += newData.acceleration[0];
		this->acceleration[1] += newData.acceleration[1];
		this->acceleration[2] += newData.acceleration[2];

		this->angularRate[0] += newData.angularRate[0];
		this->angularRate[1] += newData.angularRate[1];
		this->angularRate[2] += newData.angularRate[2];
		
		return *this;
	}

	EGNOS_ftp_data_Data& EGNOS_ftp_data_Data::operator=(EGNOS_ftp_data_Data& newData) {

		this->acceleration[0] = newData.acceleration[0];
		this->acceleration[1] = newData.acceleration[1];
		this->acceleration[2] = newData.acceleration[2];

		this->angularRate[0] = newData.angularRate[0];
		this->angularRate[1] = newData.angularRate[1];
		this->angularRate[2] = newData.angularRate[2];

		return *this;
	}

	double EGNOS_ftp_data_Data::getGPSWeek(void) {
		gpstk::GPSWeekSecond GPSTime(time);
		return GPSTime.getWeek();
	}

	double EGNOS_ftp_data_Data::getGPSToW(void) {
		gpstk::GPSWeekSecond GPSTime(time);
		return GPSTime.getSOW();
	}
	
	void EGNOS_ftp_data_Data::parseLine(std::string& currentLine)
		throw(gpstk::StringUtils::StringException, gpstk::FFStreamError)
	{
		try
		{
			
			std::stringstream   ss(currentLine);
			string week, tow, accx, accy, accz, rollRate, pitchRate, yawRate;

			ss >> week >> tow >> accx >> accy >> accz >> rollRate >> pitchRate >> yawRate;

			gpstk::GPSWeekSecond timeGPS;
			gpstk::GALWeekSecond timeGAL;

			if (timeSys == gpstk::TimeSystem::GPS) {
				timeGPS.week = stoi(week, nullptr);
				timeGPS.sow = stod(tow, nullptr);

				gpstk::CommonTime temp(timeGPS);
				time = temp;

			}
			else if (timeSys == gpstk::TimeSystem::GAL) {
				timeGAL.week = stoi(week, nullptr);
				timeGAL.sow = stod(tow, nullptr);

				gpstk::CommonTime temp(timeGAL);
				time = temp;
			}



			if (accx.empty() || accy.empty() || accz.empty()) {
				acceleration[0] = 0;
				acceleration[1] = 0;
				acceleration[2] = 0;
				// TODO shall throw error or sys
			}
			else {
				acceleration[0] = stod(accx, nullptr);
				acceleration[1] = stod(accy, nullptr);
				acceleration[2] = stod(accz, nullptr);
			}
				
			

			if (rollRate.empty() || pitchRate.empty() || yawRate.empty()) {
				angularRate[0] = 0;
				angularRate[1] = 0;
				angularRate[2] = 0;
			}
			else{
				angularRate[0] = stod(rollRate, nullptr);
				angularRate[1] = stod(pitchRate, nullptr);
				angularRate[2] = stod(yawRate, nullptr);
			}
			

			
		}
		catch (std::exception &e)
		{
			gpstk::FFStreamError err("std::exception: " +
				string(e.what()));
			GPSTK_THROW(err);
		}
	}

}