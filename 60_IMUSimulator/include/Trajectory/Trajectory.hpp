#ifndef IMUSIMULATOR_TRAJECTORY_HPP
#define IMUSIMULATOR_TRAJECTORY_HPP

#include "geoparam_wgs84.hpp"
#include "strapdown_ecef.hpp"
#include "PositionData.hpp"
#include "std_IMUSimulator.hpp"
#include <iostream>
#include <vector>

#include "I_GINA_writer.hpp"

namespace IMUSimulator {

	std::ostream& operator<<(std::ostream& os, const Position_IMU&);

	class Trajectory {
		
		public:
			Position_IMU node;						// this is a temp variable. Used to collect every position data before we can push it to the Position_IMU vector
			std::vector<Position_IMU> traj_data;
			Trajectory(void);

			void add_position(double, double, double, double, double, double, int, double, double, double, double);
			void add_position(CoordiateFrame, double[], TimeFrame, int, double, double, double, double);
			void add_position(CoordiateFrame, double[], TimeFrame, int, double);
			void add_position(CoordiateFrame, TimeFrame, Position_IMU);

			void updateTime(TimeFrame, int, double);

			friend std::ostream& operator<<(std::ostream&, Trajectory&);
			friend std::ostream& operator<<(std::ostream&, const Position_IMU&);

			friend strapdown_ecef& operator>>(strapdown_ecef&, Trajectory&);
			friend PositionData& operator>>(PositionData&, Trajectory&);

		private:
			void printf(std::ostream&);
			void clearNode(void);
			void add_position(void);
			void add_position(CoordiateFrame, TimeFrame);
			void transform2missingCoordinateFrame(CoordiateFrame, TimeFrame);
	};
}

#endif