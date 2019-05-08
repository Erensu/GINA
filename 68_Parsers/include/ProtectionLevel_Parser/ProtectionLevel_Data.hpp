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

		enum info_code {
			eSatElevationTagFound,
			eAzimuthTagFound,
			eIppLatTagFound,
			eIppLonTagFound,
			eIonoDelayTagFound,
			eIonoRMSTagFound,
			eReferenceIonoDelayTagFound,
			eReferenceIonoRMSTagFound,
			eUnknownInfoTagFound
		};

		

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

		typedef struct {
			bool el_deg_valid = false;
			double el_deg = 0;
			bool az_deg_valid = false;
			double az_deg = 0;
			gpstk::SatID satId;
			bool ippLat_valid = false;
			double ippLat = 0;
			bool ippLon_valid = false;
			double ippLon = 0;
			bool ionoCorr_meter_valid = false;
			double ionoCorr_meter = 0;
			bool ionoRMS_meter_valid = false;
			double ionoRMS_meter = 0;
			bool ionoReferenceCorr_meter_valid = false;
			double ionoReferenceCorr_meter = 0;
			bool ionoReferenceRMS_meter_valid = false;
			double ionoReferenceRMS_meter = 0;
		}SatInfo;
		
		gpstk::CommonTime dataTime;
		gpstk::Position posData;
		std::vector<SatInfo> satInfo;
		//void reset(void);
		
		

		bool isDataEnd = false;
		bool isDataStart = false;
		std::string typeOfCalcuation = "UNKNOWN";
		double HPL = UNVALID_PL_DATA;
		double VPL = UNVALID_PL_DATA;
		double AlarmLimit = UNVALID_PL_DATA;
		double HorizontalAlarmLimit = UNVALID_PL_DATA;
		double VerticalAlarmLimit = UNVALID_PL_DATA;
		double posError = UNVALID_PL_DATA;
		double horizontalPosError = UNVALID_PL_DATA;
		double verticalPosError = UNVALID_PL_DATA;
		double probabilityOfIntegrity = UNVALID_PL_DATA;
		double elevationMask = UNVALID_PL_DATA;
		Eigen::MatrixXd Covariance_ecef;
		Eigen::MatrixXd Covariance_enu;
		std::vector<gpstk::SatID> includedSatIds;
		std::vector<gpstk::SatID> excludedSatIds;

		static const double UNVALID_PL_DATA;

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

		static const std::string satElevationTag;
		static const std::string azimuthTag;
		static const std::string ippLatTag;
		static const std::string ippLonTag;
		static const std::string ionoDelayTag;
		static const std::string ionoRMSTag;
		static const std::string ionoReferenceDelayTag;
		static const std::string ionoReferenceRMSTag;
		static const std::string unknownInfoTag;

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

		// Write methods
		bool writeStartofData(gpstk::FFStream& ffs) const;
		bool writeEndtofData(gpstk::FFStream& ffs) const;
		bool writeTimeOfData(gpstk::FFStream& ffs) const;
		bool writePosition(gpstk::FFStream& ffs) const;
		bool writeHPL(gpstk::FFStream& ffs) const;
		bool writeVPL(gpstk::FFStream& ffs) const;
		bool writeElevation(gpstk::FFStream& ffs) const;
		bool writeTypeOfCalculation(gpstk::FFStream& ffs) const;
		bool writeProbabilityOfIntegrity(gpstk::FFStream& ffs) const;
		bool writeHorizontalPositionError(gpstk::FFStream& ffs) const;
		bool writeVerticalPositionError(gpstk::FFStream& ffs) const;
		bool writePositionError3D(gpstk::FFStream& ffs) const;
		bool writeHorizontalAlarmLimit(gpstk::FFStream& ffs) const;
		bool writeVerticalAlarmLimit(gpstk::FFStream& ffs) const;
		bool writeAlarmLimit3D(gpstk::FFStream& ffs) const;
		bool writeCovMatrixEcef(gpstk::FFStream& ffs) const;
		bool writeCovMatrixEnu(gpstk::FFStream& ffs) const;
		bool writeUsedSats(gpstk::FFStream& ffs) const;
		bool writeUnusedSats(gpstk::FFStream& ffs) const;

		ProtectionLevel_Data::info_code hashit(std::string const& inString);
		void resetData(void);

	};  // class 

		//@}
} // namespace

#endif

