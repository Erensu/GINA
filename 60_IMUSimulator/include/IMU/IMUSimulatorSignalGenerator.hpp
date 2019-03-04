#ifndef IMUSIMULATOR_SIGNAL_GENERATOR_HPP
#define IMUSIMULATOR_SIGNAL_GENERATOR_HPP

#include "std_IMUSimulator.hpp"
#include "geoparam_wgs84.hpp"
#include "lib_Frames.hpp"

#include <iostream>
#include <vector>

namespace IMUSimulator {

	class IMUSignalGenerator {

		public:

			double g;
			double wie_e;
			Measure_IMU meas;

			WGS84Ellipsoid wgs84;

			IMUSignalGenerator(void);
			void addError(void);
			Measure_IMU calculate(Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d);
			Measure_IMU calculate(double[3], double[3], Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d);

		private:

			void update_gravitiy(double, double, double);
			Eigen::Matrix3d pos2Cne_RAD(double& lat, double& lon);

	};






}

#endif