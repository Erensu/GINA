﻿
#include "CreateTrajectorywIMUCommand.hpp"
#include "IMUSimulatorControl.hpp"

void IMUCommandForTrajectory(std::string trajFileNamewPath, std::string imuFileNamewPath, std::string imuCommandFileNamewPath) {


	// IMU Control file parser
	GINAParser::IMUControlStream imuControlFileIn(imuCommandFileNamewPath.c_str());
	
	GINAParser::IMUControlHeader imuControlHeader;
	GINAParser::IMUControlData imuControlData;

	imuControlFileIn >> imuControlHeader;

	gpstk::GPSWeekSecond startTimeinGPSTime(imuControlHeader.startTime);
	gpstk::GPSWeekSecond endTimeinGPSTime(imuControlHeader.endTime);

	Eigen::Vector3d Vb, rollpitchyaw, ecef;
	ecef << imuControlHeader.startPosition.getX(), imuControlHeader.startPosition.getY(), imuControlHeader.startPosition.getZ();

	rollpitchyaw << imuControlHeader.startAttitude[0] / 180.0*EIGEN_PI,
					imuControlHeader.startAttitude[1] / 180.0*EIGEN_PI,
					imuControlHeader.startAttitude[2] / 180.0*EIGEN_PI;
	Vb << imuControlHeader.startVelocity[0], imuControlHeader.startVelocity[1], imuControlHeader.startVelocity[2];

	typedef std::numeric_limits< double > dbl;
	std::cout.precision(dbl::max_digits10);

	IMUSimulator::IMUStore imuStore;
	IMUSimulator::Trajectory traj;
	IMUSimulator::PositionData posData;
	IMUSimulator::Measure_IMU meas;
	IMUSimulator::IMUData imu_meas;

	GINAParser::TrajectoryStream trajFileOut(trajFileNamewPath.c_str(), std::ios::out);
	GINAParser::TrajectoryHeader trajHeader;
	GINAParser::TrajectoryData trajData;

	GINAParser::IMUStream imuFileOut(imuFileNamewPath.c_str(), std::ios::out);
	GINAParser::IMUHeader imuHeader;
	GINAParser::IMUData imuData;

	IMUSimulator::IMUSignalGenerator imuGenerator;
	IMUSimulator::strapdown_ecef str_e(rollpitchyaw, Vb, ecef);

	setGINAParsers(	trajFileOut, trajHeader,
					imuFileOut, imuHeader,
					ecef, rollpitchyaw,
					startTimeinGPSTime.week,startTimeinGPSTime.sow,
					endTimeinGPSTime.week, endTimeinGPSTime.sow);

	trajFileOut << trajHeader;
	imuFileOut << imuHeader;

	IMUSimulator::IMUControl  imuControl;
	
	imuControl.setIMUControl(	imuControlHeader.startPosition.getGeodeticLatitude(), 
								imuControlHeader.startPosition.getLongitude(), 
								imuControlHeader.startPosition.getHeight(),
								rollpitchyaw(0), rollpitchyaw(1), rollpitchyaw(2),
								imuControlHeader.startVelocity);

		
		
	while (imuControlFileIn >> imuControlData) {
		IMUSimulator::IMUControlCommand imuCommand(imuControlData);
		imuControl.setCommand(imuCommand);

		while (imuControl.runStep()) {

			imuControl.getPositionData(posData);
			imuControl.getMeasurement(meas);

			trajFileOut << convert2GINAcompatible(posData);
			imuFileOut << convert2GINAcompatible(meas);
		}

	}
		/*Usage of runAll() method*/
		/*imuControl.runAll();

		imuControl.getPositionData(posData);

		meas = imuControl.getMeasurement();

		trajFileOut << convert2GINAcompatible(posData);
		imuFileOut << convert2GINAcompatible(meas);*/

	trajFileOut.close();
	imuFileOut.close();
	imuControlFileIn.close();

	return;
}
