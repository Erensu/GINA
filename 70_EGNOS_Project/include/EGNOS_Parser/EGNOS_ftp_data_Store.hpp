#pragma once

#include <iostream>
#include <map>
#include "EGNOS_ftp_data_Data.hpp"

namespace EGNOS_ftp_data_Parser {
	class EGNOS_ftp_data_Store
	{
	public:
		EGNOS_ftp_data_Store();
		~EGNOS_ftp_data_Store();

		gpstk::Position::CoordinateSystem coorSys = gpstk::Position::CoordinateSystem::Unknown;

		EGNOS_ftp_data_Data& addPosition(EGNOS_ftp_data_Data);
		EGNOS_ftp_data_Data findPosition(gpstk::GPSWeekSecond);
		/* Return epochs as a vector from the Trajectoryfile.
		*/
		vector<gpstk::GPSWeekSecond> listTime(void); // TODO tudja kilistázni a benne levo idoket.

		bool EGNOS_ftp_data_Store::operator==(const EGNOS_ftp_data_Store& other) const;
		bool EGNOS_ftp_data_Store::operator!=(const EGNOS_ftp_data_Store& other) const;

	protected:
		bool isCoorSystemSet = false;
		typedef std::map<gpstk::GPSWeekSecond, EGNOS_ftp_data_Data> IMUMap;
		IMUMap imuStore;
		void setCorrdinateSystem(gpstk::Position::CoordinateSystem);
		bool compare(const EGNOS_ftp_data_Store& other) const;
	};

}