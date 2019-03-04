#ifndef TRAJECTORY_HEADER_HPP
#define TRAJECTORY_HEADER_HPP

#include "TrajectoryBase.hpp"
#include "CommonTime.hpp"
#include "GPSWeekSecond.hpp"
#include "GALWeekSecond.hpp"
#include "Position.hpp"
#include "FFTextStream.hpp"

using namespace std;
#define INVALID_EPOCHINTERVAL_TRAJ_HEADER -1

namespace GINAParser {

	/// This class models the RINEX 3 Nav header for a RINEX 3 Nav file.
	/// \sa Rinex3NavData and Rinex3NavStream classes.
	class TrajectoryHeader : public TrajectoryBase
	{
	public:

		//// Public member functions

		/// Constructor
		TrajectoryHeader(void) 
			: valid(true), firstLineisRead(false), isPosFormatSet(false), isHeaderEnd(false)
			{}

		/// Destructor
		virtual ~TrajectoryHeader() {}

		/// Rinex3NavHeader is a "header" so this function always returns true.
		virtual bool isHeader(void) const{
			return true;
		}

		/// This function dumps the contents of the header.
		virtual void dump(std::ostream& s) const;

		enum FormatSpec {
			isFormatGINA = 1,
			isFormatCSSIM = 2,
			isFormatGNSSSIM = 3
		} formatSpec;

		bool isPosFormatSet;
		bool isHeaderEnd;
		bool firstLineisRead;
		bool valid;
		gpstk::Position::CoordinateSystem coorSys;
		gpstk::TimeSystem timeSys;
		gpstk::Position startPosition;
		gpstk::CommonTime startTime;
		gpstk::CommonTime endTime;

		double startVelocity[3];
		double startAttitude[3];
		double epochInterval;

		string Creator;
		string timeOfCreation;

		//TrajectoryStream strm;

		static const int numberofLineinHeader;
		static const string startofHeaderGinaTag;
		static const string secondLineOfGINATrajectoryTag;
		static const string creatorOfFileTag;
		static const string timeofCreationTag;
		static const string positionTypeLLHTag;
		static const string positionTypeECEFTag;
		static const string timeSystemDefinitionTag;
		static const string startTimeTag;
		static const string endTimeTag;
		static const string startPositionTag;
		static const string startVelocityTag;
		static const string startAttitudeTag;
		static const string EpochIntervalTag;
		static const string endOfHeaderTag;


	protected:

		//// Protected member functions
		/// Write this header to stream \a s.
		void reallyPutRecord(gpstk::FFStream& s) const
			throw(exception,
				gpstk::FFStreamError,
				gpstk::StringUtils::StringException);


		/// This function reads the RINEX Nav header from the given FFStream.
		/// If an error is encountered in reading from the stream, the stream
		/// is reset to its original position and its fail-bit is set.
		/// @throws StringException when a StringUtils function fails
		/// @throws FFStreamError when exceptions(failbit) is set and a read
		///         or formatting error occurs.  This also resets the stream
		///         to its pre-read position.
		void reallyGetRecord(gpstk::FFStream& s)
			throw(exception,
				gpstk::FFStreamError,
				gpstk::StringUtils::StringException);

	private:

		// Read methods
		bool hasStartofHeaderFound(string&, gpstk::FFStream&);
		bool hasCreatorFound(string&);
		bool hasTimeofCreationFound(string&);
		bool hasCoordinateSystemFound(string&);
		bool hasTimeSystemFound(string&);
		bool hasStartTimeFound(string&);
		bool hasEndTimeFound(string&);
		bool hasStartPositionFound(string&);
		bool hasStartVelocityFound(string&);
		bool hasStartAttitudeFound(string&);
		bool hasEpochIntervalFound(string&);
		bool hasEndofHeaderFound(string&, gpstk::FFStream&);

		// Write methods
		bool writeStartofHeader(gpstk::FFStream&) const;
		bool writeCreatorFound(gpstk::FFStream&) const;
		bool writeTimeofCreationFound(gpstk::FFStream&) const;
		bool writeTimeSystemFound(gpstk::FFStream&) const;
		bool writeStartTimeFound(gpstk::FFStream&) const;
		bool writeEndTimeFound(gpstk::FFStream&) const;
		bool writeCoordinateSystemFound(gpstk::FFStream&) const;
		bool writeStartPositionFound(gpstk::FFStream&) const;
		bool writeStartVelocityFound(gpstk::FFStream&) const;
		bool writeStartAttitudeFound(gpstk::FFStream&) const;
		bool writeEpochIntervalFound(gpstk::FFStream&) const;
		bool writeEndofHeaderFound(gpstk::FFStream&) const;
		bool writeEmptyLine(gpstk::FFStream&) const;

		

	}; // End of class 'Rinex3NavHeader'

	   //@}

}  // End of namespace gpstk

#endif