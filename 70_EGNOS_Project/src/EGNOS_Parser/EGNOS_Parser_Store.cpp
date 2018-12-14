#include "EGNOS_ftp_data_Store.hpp"

namespace EGNOS_ftp_data_Parser {
	EGNOS_ftp_data_Store::EGNOS_ftp_data_Store()
	{
	}


	EGNOS_ftp_data_Store::~EGNOS_ftp_data_Store()
	{
	}

	void EGNOS_ftp_data_Store::setCorrdinateSystem(gpstk::Position::CoordinateSystem coor) {
		if (isCoorSystemSet == false && coor != gpstk::Position::CoordinateSystem::Unknown) {
			coorSys = coor;
			isCoorSystemSet = true;
		}
	}

	EGNOS_ftp_data_Data& EGNOS_ftp_data_Store::addPosition(EGNOS_ftp_data_Data data) {
		// TODO not sure if it's correct
		EGNOS_ftp_data_Data ret;
		if (imuStore.find(data.time) == imuStore.end()) {
			setCorrdinateSystem(data.coorSys);
			if (isCoorSystemSet && coorSys != gpstk::Position::CoordinateSystem::Unknown) {
				ret = data;

				ret.acceleration[0] = data.acceleration[0];
				ret.acceleration[1] = data.acceleration[1];
				ret.acceleration[2] = data.acceleration[2];

				ret.angularRate[0] = data.angularRate[0];
				ret.angularRate[1] = data.angularRate[1];
				ret.angularRate[2] = data.angularRate[2];

				imuStore[data.time] = ret;
			}
		}
	    return ret;
	}

	EGNOS_ftp_data_Data EGNOS_ftp_data_Store::findPosition(gpstk::GPSWeekSecond time) {
		EGNOS_ftp_data_Data trajData;
		EGNOS_ftp_data_Data ret;
		IMUMap::iterator it;
		it = imuStore.find(time);
		if (it != imuStore.end()) {
			ret = it->second;
		}
		else {
			bool isTimeFound = false;
			vector<gpstk::GPSWeekSecond> timeVector = this->listTime();
			gpstk::CommonTime comtime; 
			gpstk::CommonTime original_comtime = time.convertToCommonTime();
			std::vector<gpstk::GPSWeekSecond>::iterator it_previous;
			
			gpstk::CommonTime comtime_last_valid;
			for (std::vector<gpstk::GPSWeekSecond>::iterator it = timeVector.begin(); it != timeVector.end(); it) {
				
				it_previous = it;
				gpstk::CommonTime comtime(*it);
				gpstk::CommonTime comtime_previous(*it_previous);
				comtime_last_valid = comtime;
				if ( comtime_previous <= original_comtime && original_comtime <= comtime) {
					ret = this->findPosition(comtime);
					bool isTimeFound = true;
				}

				++it;
			}
			if (isTimeFound == false) {
				ret = this->findPosition(comtime_last_valid);
			}
		}
		return ret;
	}

	bool EGNOS_ftp_data_Store::operator==(const EGNOS_ftp_data_Store& other) const {
		return this->compare(other);
	}
	bool EGNOS_ftp_data_Store::operator!=(const EGNOS_ftp_data_Store& other) const {
		return !(this->compare(other));
	}

	bool EGNOS_ftp_data_Store::compare(const EGNOS_ftp_data_Store& other) const {
		bool returnValue = true;
		IMUMap::const_iterator itother;
		IMUMap::const_iterator it ;

		for (it = this->imuStore.begin(); it != this->imuStore.end(); ++it) {
			itother = other.imuStore.find(it->first);
			if (itother == other.imuStore.end() || it->second != itother->second ) {
				returnValue = false;
			}
		}
		return returnValue;
	}

	vector<gpstk::GPSWeekSecond> EGNOS_ftp_data_Store::listTime(void) {

		vector<gpstk::GPSWeekSecond> timeVector;
		for (IMUMap::const_iterator it = this->imuStore.begin(); it != this->imuStore.end(); ++it) {
			timeVector.push_back(it->first);
		}
		return timeVector;
	}



}
