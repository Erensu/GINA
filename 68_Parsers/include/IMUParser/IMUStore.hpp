#ifndef IMU_STORE_HPP
#define IMU_STORE_HPP

#include <iostream>
#include <map>
#include "IMUData.hpp"

namespace GINAParser {
	class IMUStore
	{
	public:
		IMUStore();
		~IMUStore();

		gpstk::Position::CoordinateSystem coorSys = gpstk::Position::CoordinateSystem::Unknown;

		IMUData& addPosition(IMUData);
		IMUData findPosition(gpstk::GPSWeekSecond);
		/* Return epochs as a vector from the Trajectoryfile.
		*/
		vector<gpstk::GPSWeekSecond> listTime(void); // TODO tudja kilist�zni a benne levo idoket.

		bool IMUStore::operator==(const IMUStore& other) const;
		bool IMUStore::operator!=(const IMUStore& other) const;

	protected:
		bool isCoorSystemSet = false;
		typedef std::map<gpstk::GPSWeekSecond, IMUData> IMUMap;
		IMUMap imuStore;
		void setCorrdinateSystem(gpstk::Position::CoordinateSystem);
		bool compare(const IMUStore& other) const;
	};

}

#endif