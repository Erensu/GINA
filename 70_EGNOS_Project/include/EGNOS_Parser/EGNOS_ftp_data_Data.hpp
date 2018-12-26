#pragma once


#include <list>

#include <bitset>
#include "StringUtils.hpp"
#include "CommonTime.hpp"
#include "FFStream.hpp"
#include "RinexNavBase.hpp"
#include "EGNOS_ftp_data_Base.hpp"
#include "EGNOS_ftp_data_Stream.hpp"
#include "Position.hpp"
#include "EGNOS_ftp_data_Header.hpp"

#include "CivilTime.hpp"
#include "YDSTime.hpp"

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

		
		gpstk::CivilTime messageTime;

		std::bitset<256> message;
		unsigned int messageId;
		unsigned int svId;

		double getGPSWeek(void);
		double getGPSToW(void);

		void reset(void);
		
	private:
		EGNOS_ftp_data_Stream* strm;

		string EGNOS_ftp_data_Data::HexCharToBin(char c);
		string EGNOS_ftp_data_Data::HexStrToBin(const std::string & hs);
		string EGNOS_ftp_data_Data::reverseStr(string& str) const;
		string EGNOS_ftp_data_Data::int2string(unsigned int number) const;
		char EGNOS_ftp_data_Data::getHexCharacter(std::string str) const;
		string EGNOS_ftp_data_Data::bitset2hexstring(void) const;

		void EGNOS_ftp_data_Data::parseLine(std::string& currentLine)
			throw(gpstk::StringUtils::StringException, gpstk::FFStreamError);

	};  // class RinexNavData

		//@}
} // namespace



