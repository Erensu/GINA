#ifndef TRAJECTORY_STORE_HPP
#define TRAJECTORY_STORE_HPP

#include <iostream>
#include <map>
#include "TrajectoryData.hpp"

namespace GINAParser {
	class TrajectoryStore
	{
	public:
		TrajectoryStore();
		~TrajectoryStore();

		gpstk::Position::CoordinateSystem coorSys = gpstk::Position::CoordinateSystem::Unknown;

		TrajectoryData& addPosition(TrajectoryData);
		TrajectoryData& findPosition(gpstk::GPSWeekSecond);
		/* Return epochs as a vector from the Trajectoryfile.
		*/
		vector<gpstk::GPSWeekSecond> listTime(void); // TODO tudja kilist�zni a benne levo idoket.

		bool TrajectoryStore::operator==(const TrajectoryStore& other) const;
		bool TrajectoryStore::operator!=(const TrajectoryStore& other) const;

	protected:
		bool isCoorSystemSet = false;
		typedef std::map<gpstk::GPSWeekSecond, TrajectoryData> TrajectoryMap;
		TrajectoryMap TrajStore;
		void setCorrdinateSystem(gpstk::Position::CoordinateSystem);
		bool compare(const TrajectoryStore& other) const;
	};

}

#endif