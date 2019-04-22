#ifndef PROTECTION_LEVEL_DATA_HPP
#define PROTECTION_LEVEL_DATA_HPP

// Primary Math Library
#include <Eigen/Dense>

#include <bitset>
#include "StringUtils.hpp"
#include "CommonTime.hpp"
#include "FFStream.hpp"
#include "ProtectionLevel_Base.hpp"
#include "ProtectionLevel_Stream.hpp"

#include "CivilTime.hpp"
#include "GPSWeekSecond.hpp"
#include "Position.hpp"

namespace ProtectionLevel_Parser
{
	/// @ingroup FileHandling
	//@{

	/**
	* This class models a RINEX NAV record.
	*
	* \sa rinex_nav_test.cpp and rinex_nav_read_write.cpp for examples.
	* \sa gpstk::RinexNavHeader and gpstk::RinexNavStream classes.
	*/
	class ProtectionLevel_Data : public ProtectionLevel_Base
	{
	public:
		/**
		* Constructor
		* @warning CHECK THE PRNID TO SEE IF THIS DATA IS
		*  VALID BEFORE USING!!
		*/
		ProtectionLevel_Data() {};

		/// destructor
		virtual ~ProtectionLevel_Data() {}

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

		
		gpstk::CommonTime dataTime;
		gpstk::Position posData;

		//void reset(void);
		
		bool isDataEnd = false;
		bool isDataStart = false;
		std::string typeOfCalcuation = "Unknown";
		double HPL = 0;
		double VPL = 0;
		double AlarmLimit = 0;
		double HorizontalAlarmLimit = 0;
		double VerticalAlarmLimit = 0;
		double posError = 0;
		double horizontalPosError = 0;
		double verticalPosError = 0;
		double probabilityOfIntegrity = 0;
		double elevationMask = 0;
		Eigen::MatrixXd Covariance_ecef;
		Eigen::MatrixXd Covariance_enu;
		std::vector<gpstk::SatID> includedSatIds;
		std::vector<gpstk::SatID> excludedSatIds;

		static const std::string startofdataTag;
		static const std::string endOfdataTag;
		static const std::string timeofdataTag;
		static const std::string positionTag;
		static const std::string hplTag;
		static const std::string vplTag;
		static const std::string elevationTag;
		static const std::string typeofcalculationTag;
		static const std::string probabilitofintegrityTag;
		static const std::string horizontalpositionerrorTag;
		static const std::string verticalpositionerrorTag;
		static const std::string positionerrorTag;
		static const std::string horizontalalarmTag;
		static const std::string verticalalarmTag;
		static const std::string alarmTag;
		static const std::string startofcovmatrixecefTag;
		static const std::string endofcovmatrixecefTag;
		static const std::string startofcovmatrixenuTag;
		static const std::string endofcovmatrixenuTag;
		static const std::string startofusedsatTag;
		static const std::string endofusedsatTag;
		static const std::string startofunusedsatTag;
		static const std::string endofunusedsatTag;

	private:
		ProtectionLevel_Stream* strm;

		// Read methods
		bool hasStartofDataFound(std::string&, gpstk::FFStream&);
		bool hasTimeofDataFound(std::string&);
		bool hasPositionFound(std::string&);
		bool hasVerticalProtectionLevelFound(std::string& line);
		bool hasHorizontalProtectionLevelFound(std::string& line);
		bool hasElevationMaskFound(std::string& line);
		bool hasTypeOfCalculationFound(std::string& line);
		bool hasProbabilityOfIntegrityFound(std::string& line);
		bool hasHorizontalErrorFound(std::string& line);
		bool hasVerticalErrorFound(std::string& line);
		bool hasPositionErrorFound(std::string& line);
		bool hasHorizontalAlarmLimitFound(std::string& line);
		bool hasVerticalAlarmLimitFound(std::string& line);
		bool hasAlarmLimitFound(std::string& line);
		bool hasCovEcefMatrixFound(std::string& line, gpstk::FFStream& ffs);
		bool hasCovEnuMatrixFound(std::string& line, gpstk::FFStream& ffs);
		bool hasUsedSatsFound(std::string& line, gpstk::FFStream& ffs);
		bool hasUnusedSatsFound(std::string& line, gpstk::FFStream& ffs);
		bool hasEndofDataFound(std::string&, gpstk::FFStream&);

		void parseLine(std::string& currentLine)
			throw(gpstk::StringUtils::StringException, gpstk::FFStreamError);

	};  // class 

		//@}
} // namespace

#endif

