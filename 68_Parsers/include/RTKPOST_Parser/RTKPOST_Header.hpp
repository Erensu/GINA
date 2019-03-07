#ifndef RTKPOST_HEADER_HPP
#define RTKPOST_HEADER_HPP

#include "RTKPOST_Base.hpp"
#include "CommonTime.hpp"
#include "GPSWeekSecond.hpp"
#include "GALWeekSecond.hpp"
#include "Position.hpp"
#include "FFTextStream.hpp"

using namespace std;

#define INVALID_EPOCHINTERVAL_IMU_HEADER -1

namespace RTKPOST_Parser {

	/// This class models the RINEX 3 Nav header for a RINEX 3 Nav file.
	/// \sa Rinex3NavData and Rinex3NavStream classes.
	class RTKPOST_Pos_Header : public RTKPOST_Pos_Base
	{
	public:

		//// Public member functions

		/// Constructor
		RTKPOST_Pos_Header(void)
			: valid(true), firstLineisRead(false)
			{}

		/// Destructor
		virtual ~RTKPOST_Pos_Header() {}

		/// Rinex3NavHeader is a "header" so this function always returns true.
		virtual bool isHeader(void) const{
			return true;
		}

		/// This function dumps the contents of the header.
		//virtual void dump(std::ostream& s) const;

	
		//bool isHeaderEnd = false;
		bool firstLineisRead;
		bool valid;

		int numberOfLineRead = 0;

		string programInfo = "Unknown";
		std::vector<string> inpFiles;
		gpstk::CommonTime obsStart;
		gpstk::CommonTime obsEnd;
		std::string posMode = "Unknown";
		double elevAngle = -10;
		std::string ionosOpt = "Unknown";
		std::string tropoOpt = "Unknown";
		std::string ephemerisOpt = "Unknown";
		std::string token = tokenTag;
		
		gpstk::ReferenceFrame datum = gpstk::ReferenceFrame::WGS84;
		gpstk::TimeSystem timeSys = gpstk::TimeSystem::GPS;

		std::map<int, std::string> UnknownHeaderLines;

		static const string GINAProgram;
		static const string headerprefixTag;
		static const string programTag;
		static const string inpFileTag;
		static const string obsStartTag;
		static const string obsEndTag;
		static const string posModeTag;
		static const string elevMaskTag;
		static const string ionosOptTag;
		static const string tropoOptTag;
		static const string ephemerisOptTag;
		static const string tokenTag;
		static const string preTokenTag;
		static const string coloumnTag;
		static const string degreeTag;
		static const string gpsTimeTag;

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


		void dump(ostream& s) const;

	private:

		// Read methods
		bool isPartOfHeader(string&, gpstk::FFStream&);
		bool hasCreatorFound(string&);
		bool hasInpFileFound(string&);
		bool hasPosModeFound(string&);
		bool hasElevMaskFound(string&);
		bool hasStartTimeFound(string&);
		bool hasEndTimeFound(string&);
		bool hasIonosOptFound(string&);
		bool hasTropoOptFound(string&);
		bool hasEphemerisOptFound(string&);
		bool hasPreTokenTagFound(string&);
		bool hasColoumnTagFound(string&, gpstk::FFStream&);

		// Write methods
		bool writeProgramInfoFound(gpstk::FFStream&, bool&) const;
		bool writeInpFileFound(gpstk::FFStream&, bool&) const;
		bool writeObsStartFound(gpstk::FFStream&, bool&) const;
		bool writeObsEndFound(gpstk::FFStream&, bool&) const;
		bool writePosModeFound(gpstk::FFStream&, bool&) const;
		bool writeElevAngleSystemFound(gpstk::FFStream&, bool&) const;
		bool writeIonosOptFound(gpstk::FFStream&, bool&) const;
		bool writeTropoOptFound(gpstk::FFStream&, bool&) const;
		bool writeEphemerisOptFound(gpstk::FFStream& , bool&) const;
		bool writeTokenFound(gpstk::FFStream&, bool&) const;
		bool writeColoumnFound(gpstk::FFStream&, bool&) const;
		bool writeUnknownLines(gpstk::FFStream&) const;

	}; // End of class 'Rinex3NavHeader'

	   //@}

}  // End of namespace gpstk

#endif