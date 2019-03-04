#ifndef RTKPOST_DATA_HPP
#define RTKPOST_DATA_HPP

#include <bitset>
#include "StringUtils.hpp"
#include "CommonTime.hpp"
#include "FFStream.hpp"
#include "RTKPOST_Base.hpp"
#include "RTKPOST_Stream.hpp"

#include "CivilTime.hpp"
#include "GPSWeekSecond.hpp"

namespace RTKPOST_Parser
{
	/// @ingroup FileHandling
	//@{

	/**
	* This class models a RINEX NAV record.
	*
	* \sa rinex_nav_test.cpp and rinex_nav_read_write.cpp for examples.
	* \sa gpstk::RinexNavHeader and gpstk::RinexNavStream classes.
	*/
	class RTKPOST_Pos_Data : public RTKPOST_Pos_Base
	{
	public:
		/**
		* Constructor
		* @warning CHECK THE PRNID TO SEE IF THIS DATA IS
		*  VALID BEFORE USING!!
		*/
		RTKPOST_Pos_Data() {};

		/// destructor
		virtual ~RTKPOST_Pos_Data() {}

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

		
		gpstk::CommonTime messageTime;

		std::bitset<256> message;
		unsigned int messageId;
		unsigned int svId;

		double getGPSWeek(void);
		double getGPSToW(void);

		void reset(void);
		
	private:
		RTKPOST_Pos_Stream* strm;

		std::string RTKPOST_Pos_Data::HexCharToBin(char c);
		std::string RTKPOST_Pos_Data::HexStrToBin(const std::string & hs);
		std::string RTKPOST_Pos_Data::reverseStr(std::string& str) const;
		std::string RTKPOST_Pos_Data::int2string(unsigned int number) const;
		char RTKPOST_Pos_Data::getHexCharacter(std::string str) const;
		std::string RTKPOST_Pos_Data::bitset2hexstring(void) const;

		void RTKPOST_Pos_Data::parseLine(std::string& currentLine)
			throw(gpstk::StringUtils::StringException, gpstk::FFStreamError);

	};  // class 

		//@}
} // namespace



#endif