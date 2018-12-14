#pragma once


#include <list>

#include "StringUtils.hpp"
#include "CommonTime.hpp"
#include "FFStream.hpp"
#include "RinexNavBase.hpp"
#include "EGNOS_ftp_data_Base.hpp"
#include "EGNOS_ftp_data_Stream.hpp"
#include "Position.hpp"
#include "EGNOS_ftp_data_Header.hpp"


namespace EGNOS_ftp_data_Parser
{
	/// @ingroup FileHandling
	//@{

	/**
	* This class models a RINEX NAV record.
	*
	* \sa rinex_nav_test.cpp and rinex_nav_read_write.cpp for examples.
	* \sa gpstk::RinexNavHeader and gpstk::RinexNavStream classes.
	*/
	class EGNOS_ftp_data_Data : public EGNOS_ftp_data_Base
	{
	public:
		/**
		* Constructor
		* @warning CHECK THE PRNID TO SEE IF THIS DATA IS
		*  VALID BEFORE USING!!
		*/
		EGNOS_ftp_data_Data() {};

		/// destructor
		virtual ~EGNOS_ftp_data_Data() {}

		// The next four lines is our common interface
		/// RinexNavData is "data" so this function always returns true.
		virtual bool isData() const { return true; }

		/// Outputs the record to the FFStream \a s.
		virtual void reallyPutRecord(gpstk::FFStream& s) const
			throw(std::exception, gpstk::FFStreamError,
				gpstk::StringUtils::StringException);

		/**
		* This function retrieves a RINEX NAV record from the given FFStream.
		* If an error is encountered in reading from the stream, the stream
		* is returned to its original position and its fail-bit is set.
		* @throws StringException when a StringUtils function fails
		* @throws FFStreamError when exceptions(failbit) is set and
		*  a read or formatting error occurs.  This also resets the
		*  stream to its pre-read position.
		*/
		virtual void reallyGetRecord(gpstk::FFStream& s)
			throw(std::exception, gpstk::FFStreamError,
				gpstk::StringUtils::StringException);
	
		bool compare(const EGNOS_ftp_data_Data&) const;
		bool operator==(const EGNOS_ftp_data_Data& ) const;
		bool operator!=(const EGNOS_ftp_data_Data& ) const;
		
		EGNOS_ftp_data_Data& operator+=(EGNOS_ftp_data_Data&);
		EGNOS_ftp_data_Data& operator=(EGNOS_ftp_data_Data&);

		gpstk::Position::CoordinateSystem coorSys = gpstk::Position::CoordinateSystem::Unknown;
		gpstk::TimeSystem timeSys = gpstk::TimeSystem::Systems::Unknown;
		gpstk::CommonTime time;

		unsigned int precision = 11;
		double acceleration[3];
		double angularRate[3];
		
		double getGPSWeek(void);
		double getGPSToW(void);

		static const string EGNOS_ftp_data_Data::startofDataTag;
		
	private:
		EGNOS_ftp_data_Stream* strm;

		void EGNOS_ftp_data_Data::parseLine(std::string& currentLine)
			throw(gpstk::StringUtils::StringException, gpstk::FFStreamError);

	};  // class RinexNavData

		//@}
} // namespace



