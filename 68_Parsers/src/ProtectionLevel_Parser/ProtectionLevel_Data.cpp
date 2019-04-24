#include "ProtectionLevel_Data.hpp"

namespace ProtectionLevel_Parser
{
	using namespace std;

	const std::string ProtectionLevel_Data::startofdataTag = "START OF DATA";
	const std::string ProtectionLevel_Data::endOfdataTag = "END OF DATA";
	const std::string ProtectionLevel_Data::timeofdataTag = "TIME OF DATA";
	const std::string ProtectionLevel_Data::positionTag = "POSITION";
	const std::string ProtectionLevel_Data::hplTag = "HPL";
	const std::string ProtectionLevel_Data::vplTag = "VPL";
	const std::string ProtectionLevel_Data::elevationTag = "ELEVATION MASK";
	const std::string ProtectionLevel_Data::typeofcalculationTag = "TYPE OF PL CALCULATION";
	const std::string ProtectionLevel_Data::probabilitofintegrityTag = "PROBABILITY OF INTEGRITY";
	const std::string ProtectionLevel_Data::horizontalpositionerrorTag = "HP ERROR";
	const std::string ProtectionLevel_Data::verticalpositionerrorTag = "VP ERROR";
	const std::string ProtectionLevel_Data::positionerrorTag = "POSITION ERROR";
	const std::string ProtectionLevel_Data::horizontalalarmTag = "HORIZONTAL ALARM LIMIT";
	const std::string ProtectionLevel_Data::verticalalarmTag = "VERTICAL ALARM LIMIT";
	const std::string ProtectionLevel_Data::alarmTag = "ALARM LIMIT 3D";
	const std::string ProtectionLevel_Data::startofcovmatrixecefTag = "START OF COV MATRIX ECEF";
	const std::string ProtectionLevel_Data::endofcovmatrixecefTag = "END OF COV MATRIX ECEF";
	const std::string ProtectionLevel_Data::startofcovmatrixenuTag = "START OF COV MATRIX ENU";
	const std::string ProtectionLevel_Data::endofcovmatrixenuTag = "END OF COV MATRIX ENU";
	const std::string ProtectionLevel_Data::startofusedsatTag = "START OF USED SAT";
	const std::string ProtectionLevel_Data::endofusedsatTag = "END OF USED SAT";
	const std::string ProtectionLevel_Data::startofunusedsatTag = "START OF UNUSED SAT";
	const std::string ProtectionLevel_Data::endofunusedsatTag = "END OF UNUSED SAT";

	void ProtectionLevel_Data::reallyGetRecord(gpstk::FFStream& ffs)
		throw(exception,
			gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {
		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		isDataStart = false;

		resetData();
		
		while (isDataStart == false) {
			string line;
			strm.formattedGetLine(line);
			gpstk::StringUtils::stripTrailing(line);
			if (hasStartofDataFound(line, ffs) == true) {
				isDataStart = true;
				break;
			}
		}

		while (isDataStart && !isDataEnd) {

			string line;
			strm.formattedGetLine(line);
			gpstk::StringUtils::stripTrailing(line);

			if (line.length() == 0) continue;

			// Parse File
			if (hasEndofDataFound(line, ffs)) continue;
			else if (hasTimeofDataFound(line)) continue;
			else if (hasPositionFound(line)) continue;
			else if (hasVerticalProtectionLevelFound(line)) continue;
			else if (hasHorizontalProtectionLevelFound(line)) continue;
			else if (hasElevationMaskFound(line)) continue;
			else if (hasTypeOfCalculationFound(line)) continue;
			else if (hasProbabilityOfIntegrityFound(line)) continue;
			else if (hasHorizontalErrorFound(line)) continue;
			else if (hasVerticalErrorFound(line)) continue;
			else if (hasPositionErrorFound(line)) continue;
			else if (hasHorizontalAlarmLimitFound(line)) continue;
			else if (hasVerticalAlarmLimitFound(line)) continue;
			else if (hasAlarmLimitFound(line)) continue;
			else if (hasCovEcefMatrixFound(line, ffs)) continue;
			else if (hasCovEnuMatrixFound(line, ffs)) continue;
			else if (hasUsedSatsFound(line, ffs)) continue;
			else if (hasUnusedSatsFound(line, ffs)) continue;
			else continue;
		}
	}

	void ProtectionLevel_Data::reallyPutRecord(gpstk::FFStream& ffs) const
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		writeStartofData(ffs);
		writeTimeOfData(ffs);
		writePosition(ffs);
		writeHPL(ffs);
		writeVPL(ffs);
		writeElevation(ffs);
		writeTypeOfCalculation(ffs);
		writeProbabilityOfIntegrity(ffs);
		writeHorizontalPositionError(ffs);
		writeVerticalPositionError(ffs);
		writePositionError3D(ffs);
		writeHorizontalAlarmLimit(ffs);
		writeVerticalAlarmLimit(ffs);
		writeAlarmLimit3D(ffs);
		writeCovMatrixEcef(ffs);
		writeCovMatrixEnu(ffs);
		writeUsedSats(ffs);
		writeUnusedSats(ffs);

		writeEndtofData(ffs);

		//strm << endl;
		//strm.lineNumber++;
	}
	void ProtectionLevel_Data::resetData(void){
	
		

		isDataEnd = false;
		isDataStart = false;
		typeOfCalcuation = "UNKNOWN";
		HPL = UNVALID_PL_DATA;
		VPL = UNVALID_PL_DATA;
		AlarmLimit = UNVALID_PL_DATA;
		HorizontalAlarmLimit = UNVALID_PL_DATA;
		VerticalAlarmLimit = UNVALID_PL_DATA;
		posError = UNVALID_PL_DATA;
		horizontalPosError = UNVALID_PL_DATA;
		verticalPosError = UNVALID_PL_DATA;
		probabilityOfIntegrity = UNVALID_PL_DATA;
		elevationMask = UNVALID_PL_DATA;
		Covariance_ecef = Eigen::MatrixXd::Zero(3,3);
		Covariance_enu = Eigen::MatrixXd::Zero(3, 3);
		includedSatIds.clear();
		excludedSatIds.clear();
		satInfo.clear();
	}

	/*Read methods*/
	bool ProtectionLevel_Data::hasStartofDataFound(std::string& line, gpstk::FFStream& ffs) {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);
		bool rtnv = false;

		if (line.find(startofdataTag) != string::npos){

			rtnv = true;
		}

		return rtnv;
	}

	bool ProtectionLevel_Data::hasPositionFound(std::string& line) {

		if (line.find(positionTag) != string::npos) {

			if (line.length() == positionTag.length()) {
				return false;
			}

			std::string posString;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				posString = line.substr(positionTag.length() + 1, index - positionTag.length() - 1);
			}
			else {
				posString = line.substr(positionTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(posString, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(posString, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(posString);
			string lat, lon, height;

			ss >> lat >> lon >> height;

			if (lat.empty() || lon.empty() || height.empty()) {
				return false;
			}

			posData.setGeodetic(stod(lat), stod(lon), stod(height));

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasVerticalProtectionLevelFound(std::string& line) {

		if (line.find(vplTag) != string::npos) {

			if (line.length() == vplTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(vplTag.length() + 1, index - vplTag.length() - 1);
			}
			else {
				infoStr = line.substr(vplTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->VPL = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasHorizontalProtectionLevelFound(std::string& line) {

		if (line.find(hplTag) != string::npos) {

			if (line.length() == hplTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(hplTag.length() + 1, index - hplTag.length() - 1);
			}
			else {
				infoStr = line.substr(hplTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->HPL = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasElevationMaskFound(std::string& line) {

		if (line.find(elevationTag) != string::npos) {

			if (line.length() == elevationTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(elevationTag.length() + 1, index - elevationTag.length() - 1);
			}
			else {
				infoStr = line.substr(elevationTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->elevationMask = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasTypeOfCalculationFound(std::string& line) {

		if (line.find(typeofcalculationTag) != string::npos) {

			if (line.length() == typeofcalculationTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(typeofcalculationTag.length() + 1, index - typeofcalculationTag.length() - 1);
			}
			else {
				infoStr = line.substr(typeofcalculationTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->typeOfCalcuation = dataStr;

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasProbabilityOfIntegrityFound(std::string& line) {

		if (line.find(probabilitofintegrityTag) != string::npos) {

			if (line.length() == probabilitofintegrityTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(probabilitofintegrityTag.length() + 1, index - probabilitofintegrityTag.length() - 1);
			}
			else {
				infoStr = line.substr(probabilitofintegrityTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->probabilityOfIntegrity = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasHorizontalErrorFound(std::string& line) {

		if (line.find(horizontalpositionerrorTag) != string::npos) {

			if (line.length() == horizontalpositionerrorTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(horizontalpositionerrorTag.length() + 1, index - horizontalpositionerrorTag.length() - 1);
			}
			else {
				infoStr = line.substr(horizontalpositionerrorTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->horizontalPosError = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasVerticalErrorFound(std::string& line) {

		if (line.find(verticalpositionerrorTag) != string::npos) {

			if (line.length() == verticalpositionerrorTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(verticalpositionerrorTag.length() + 1, index - verticalpositionerrorTag.length() - 1);
			}
			else {
				infoStr = line.substr(verticalpositionerrorTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->verticalPosError = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasPositionErrorFound(std::string& line) {

		if (line.find(positionerrorTag) != string::npos) {

			if (line.length() == positionerrorTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(positionerrorTag.length() + 1, index - positionerrorTag.length() - 1);
			}
			else {
				infoStr = line.substr(positionerrorTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->posError = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasHorizontalAlarmLimitFound(std::string& line) {

		if (line.find(horizontalalarmTag) != string::npos) {

			if (line.length() == horizontalalarmTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(horizontalalarmTag.length() + 1, index - horizontalalarmTag.length() - 1);
			}
			else {
				infoStr = line.substr(horizontalalarmTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->HorizontalAlarmLimit = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasVerticalAlarmLimitFound(std::string& line) {

		if (line.find(verticalalarmTag) != string::npos) {

			if (line.length() == verticalalarmTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(verticalalarmTag.length() + 1, index - verticalalarmTag.length() - 1);
			}
			else {
				infoStr = line.substr(verticalalarmTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->VerticalAlarmLimit = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasAlarmLimitFound(std::string& line) {

		if (line.find(alarmTag) != string::npos) {

			if (line.length() == alarmTag.length()) {
				return false;
			}

			std::string infoStr;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				infoStr = line.substr(alarmTag.length() + 1, index - alarmTag.length() - 1);
			}
			else {
				infoStr = line.substr(alarmTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);


			std::stringstream   ss(infoStr);
			string dataStr;

			ss >> dataStr;

			if (dataStr.empty()) {
				return false;
			}

			this->AlarmLimit = stod(dataStr);

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasCovEcefMatrixFound(std::string& line, gpstk::FFStream& ffs) {

		if (line.find(startofcovmatrixecefTag) != string::npos) {

			ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

			string line1;
			strm.formattedGetLine(line1);
			gpstk::StringUtils::stripTrailing(line1);

			if (line1.find(endofcovmatrixecefTag) != string::npos) {
				strm.lineNumber = strm.lineNumber - 1;
				return false;
			}
			string line2;
			strm.formattedGetLine(line2);
			gpstk::StringUtils::stripTrailing(line2);

			if (line2.find(endofcovmatrixecefTag) != string::npos) {
				strm.lineNumber = strm.lineNumber - 2;
				return false;
			}

			string line3;
			strm.formattedGetLine(line3);
			gpstk::StringUtils::stripTrailing(line3);

			if (line3.find(endofcovmatrixecefTag) != string::npos) {
				strm.lineNumber = strm.lineNumber - 3;
				return false;
			}

			string line4;
			strm.formattedGetLine(line4);
			gpstk::StringUtils::stripTrailing(line4);

			if (line4.find(endofcovmatrixecefTag) == string::npos) {
				strm.lineNumber = strm.lineNumber - 4;
				return false;
			}

			std::string infoStr = line1 + " " + line2 + " " + line3;
			std::stringstream   ss(infoStr);

			string dataStr1, dataStr2, dataStr3, dataStr4, dataStr5, dataStr6, dataStr7, dataStr8, dataStr9, dataStr_this_shall_be_empty;
				

			ss >> dataStr1 >> dataStr2 >> dataStr3 >> dataStr4 >> dataStr5 >> dataStr6 >> dataStr7 >> dataStr8 >> dataStr9 >> dataStr_this_shall_be_empty;

			if (dataStr1.empty() || dataStr9.empty() || dataStr_this_shall_be_empty.empty() == false) {
				strm.lineNumber = strm.lineNumber - 4;
				return false;
			}
			Eigen::MatrixXd Covariance_ecef_local = Eigen::MatrixXd(3, 3);
			Covariance_ecef_local <<	stod(dataStr1), stod(dataStr2), stod(dataStr3),
										stod(dataStr4), stod(dataStr5), stod(dataStr6),
										stod(dataStr7), stod(dataStr8), stod(dataStr9);
			
			this->Covariance_ecef = Covariance_ecef_local;
			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasCovEnuMatrixFound(std::string& line, gpstk::FFStream& ffs) {

		if (line.find(startofcovmatrixenuTag) != string::npos) {

			ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

			string line1;
			strm.formattedGetLine(line1);
			gpstk::StringUtils::stripTrailing(line1);

			if (line1.find(endofcovmatrixenuTag) != string::npos) {
				strm.lineNumber = strm.lineNumber - 1;
				return false;
			}
			string line2;
			strm.formattedGetLine(line2);
			gpstk::StringUtils::stripTrailing(line2);

			if (line2.find(endofcovmatrixenuTag) != string::npos) {
				strm.lineNumber = strm.lineNumber - 2;
				return false;
			}

			string line3;
			strm.formattedGetLine(line3);
			gpstk::StringUtils::stripTrailing(line3);

			if (line3.find(endofcovmatrixenuTag) != string::npos) {
				strm.lineNumber = strm.lineNumber - 3;
				return false;
			}

			string line4;
			strm.formattedGetLine(line4);
			gpstk::StringUtils::stripTrailing(line4);

			if (line4.find(endofcovmatrixenuTag) == string::npos) {
				strm.lineNumber = strm.lineNumber - 4;
				return false;
			}

			std::string infoStr = line1 + " " + line2 + " " + line3;
			std::stringstream   ss(infoStr);

			string dataStr1, dataStr2, dataStr3, dataStr4, dataStr5, dataStr6, dataStr7, dataStr8, dataStr9, dataStr_this_shall_be_empty;


			ss >> dataStr1 >> dataStr2 >> dataStr3 >> dataStr4 >> dataStr5 >> dataStr6 >> dataStr7 >> dataStr8 >> dataStr9 >> dataStr_this_shall_be_empty;

			if (dataStr1.empty() || dataStr9.empty() || dataStr_this_shall_be_empty.empty() == false) {
				strm.lineNumber = strm.lineNumber - 4;
				return false;
			}
			Eigen::MatrixXd Covariance_enu_local = Eigen::MatrixXd(3, 3);
			Covariance_enu_local << stod(dataStr1), stod(dataStr2), stod(dataStr3),
				stod(dataStr4), stod(dataStr5), stod(dataStr6),
				stod(dataStr7), stod(dataStr8), stod(dataStr9);

			this->Covariance_enu = Covariance_enu_local;
			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasUsedSatsFound(std::string& line, gpstk::FFStream& ffs) {

		if (line.find(startofusedsatTag) != string::npos) {

			int numberOfSatLine = 0;
			ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

			string lineSatId;
			while (lineSatId.find(endofusedsatTag) == string::npos || lineSatId.find(endOfdataTag) == string::npos) {
			
				strm.formattedGetLine(lineSatId);
				gpstk::StringUtils::stripTrailing(lineSatId);
				numberOfSatLine++;

				if (lineSatId.find(endofusedsatTag) != string::npos || lineSatId.find(endOfdataTag) != string::npos) {
					break;
				}

				std::stringstream   ss(lineSatId);

				string satSysStr, satIdStr;

				ss >> satSysStr >> satIdStr;
				if (satSysStr.empty() || satIdStr.empty()) {
					strm.lineNumber = strm.lineNumber - numberOfSatLine;
					return false;
				}

				gpstk::SatID satId;
				switch (stoi(satSysStr)){
				case 0:
					satId.system = gpstk::SatID::SatelliteSystem::systemGPS;
					break;
				case 2:
					satId.system = gpstk::SatID::SatelliteSystem::systemGalileo;
					break;
				case 6:
					satId.system = gpstk::SatID::SatelliteSystem::systemGlonass;
					break;
				default:
					satId.system = gpstk::SatID::SatelliteSystem::systemUnknown;
					break;
				}

				satId.id = stoi(satIdStr);
				this->includedSatIds.push_back(satId);

			}
			
			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasUnusedSatsFound(std::string& line, gpstk::FFStream& ffs) {

		if (line.find(startofunusedsatTag) != string::npos) {

			int numberOfSatLine = 0;
			ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

			string lineSatId;
			while (lineSatId.find(endofunusedsatTag) == string::npos || lineSatId.find(endOfdataTag) == string::npos) {

				strm.formattedGetLine(lineSatId);
				gpstk::StringUtils::stripTrailing(lineSatId);
				numberOfSatLine++;

				if (lineSatId.find(endofunusedsatTag) != string::npos || lineSatId.find(endOfdataTag) != string::npos) {
					break;
				}

				std::stringstream   ss(lineSatId);

				string satSysStr, satIdStr;

				ss >> satSysStr >> satIdStr;
				if (satSysStr.empty() || satIdStr.empty()) {
					strm.lineNumber = strm.lineNumber - numberOfSatLine;
					return false;
				}

				gpstk::SatID satId;
				switch (stoi(satSysStr)) {
				case 0:
					satId.system = gpstk::SatID::SatelliteSystem::systemGPS;
					break;
				case 2:
					satId.system = gpstk::SatID::SatelliteSystem::systemGalileo;
					break;
				case 6:
					satId.system = gpstk::SatID::SatelliteSystem::systemGlonass;
					break;
				default:
					satId.system = gpstk::SatID::SatelliteSystem::systemUnknown;
					break;
				}

				satId.id = stoi(satIdStr);
				this->excludedSatIds.push_back(satId);

			}

			return true;
		}

		else {
			return false;
		}

	}

	bool ProtectionLevel_Data::hasEndofDataFound(std::string& line, gpstk::FFStream& ffs) {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (line == endOfdataTag) {
			isDataEnd = true;
			return true;
		}
		else {
			return false;
		}
	}

	bool ProtectionLevel_Data::hasTimeofDataFound(std::string& line) {
		if (line.find(timeofdataTag) != std::string::npos) {

			if (line.length() == timeofdataTag.length()) {
				return false;
			}

			std::string timeString;

			std::size_t index = line.find("#");
			if (index != string::npos) {
				timeString = line.substr(timeofdataTag.length() + 1, index - timeofdataTag.length() - 1);
			}
			else {
				timeString = line.substr(timeofdataTag.length() + 1, -1);
			}

			gpstk::StringUtils::stripTrailing(timeString, std::string(1, ' '), std::string::npos);
			gpstk::StringUtils::stripTrailing(timeString, std::string(1, '\t'), std::string::npos);

			std::stringstream   ss(timeString);
			std::string satId, year, month, day, hour, minute, second;

			ss >> year >> month >> day >> hour >> minute >> second;

			gpstk::CivilTime civTime;
			civTime.year = std::stoi(year, nullptr);
			civTime.month = std::stoi(month, nullptr);
			civTime.day = std::stoi(day, nullptr);
			civTime.hour = std::stoi(hour, nullptr);
			civTime.minute = std::stoi(minute, nullptr);
			civTime.second = std::stod(second, nullptr);

			civTime.setTimeSystem(gpstk::TimeSystem::GPS);
			gpstk::CommonTime commonTimefromCivTime(civTime);
			this->dataTime = commonTimefromCivTime;

			return true;
		}
		else {
			return false;
		}
	}


	/*Write methods*/
	bool ProtectionLevel_Data::writeStartofData(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);
		strm << startofdataTag << endl;
		strm.lineNumber++;
		return true;
	}
	bool ProtectionLevel_Data::writeEndtofData(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);
		strm << endOfdataTag << endl;
		strm.lineNumber++;
		return true;
	}
	
	bool ProtectionLevel_Data::writeTimeOfData(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);
		gpstk::CivilTime civTimeOfData(this->dataTime);
		gpstk::CommonTime defaultTime;
		if (dataTime == defaultTime) {
			strm << timeofdataTag << std::endl;
		}
		else {
			strm << timeofdataTag << " " << civTimeOfData.year << " " << civTimeOfData.month << " " << civTimeOfData.day << " " << civTimeOfData.hour << " " << civTimeOfData.minute << " " << civTimeOfData.second << " " << std::endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writePosition(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (posData.X() == 0 && posData.Y() == 0 && posData.Z() == 0) {
			strm << positionTag << endl;
		}
		else {
			strm << positionTag << " " << this->posData.getGeodeticLatitude() << " " << this->posData.getLongitude() << " " << this->posData.getHeight() << " " << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeHPL(gpstk::FFStream& ffs) const {

		
		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);
		if (HPL == UNVALID_PL_DATA) {
			strm << hplTag << endl;
		}
		else {
			strm << hplTag << " " << HPL << endl;
		}
		
		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeVPL(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (VPL == UNVALID_PL_DATA) {
			strm << vplTag << endl;
		}
		else {
			strm << vplTag << " " << VPL << endl;
		}
	
		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeElevation(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (elevationMask == UNVALID_PL_DATA) {
			strm << elevationTag << endl;
		}
		else {
			strm << elevationTag << " " << elevationMask << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeTypeOfCalculation(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (typeOfCalcuation.empty()) {
			strm << typeofcalculationTag << endl;
		}
		else {
			strm << typeofcalculationTag << " " << typeOfCalcuation << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeProbabilityOfIntegrity(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (probabilityOfIntegrity == UNVALID_PL_DATA) {
			strm << probabilitofintegrityTag << endl;
		}
		else {
			strm << probabilitofintegrityTag << " " << probabilityOfIntegrity << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeHorizontalPositionError(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (horizontalPosError == UNVALID_PL_DATA) {
			strm << horizontalpositionerrorTag << endl;
		}
		else {
			strm << horizontalpositionerrorTag << " " << horizontalPosError << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeVerticalPositionError(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (verticalPosError == UNVALID_PL_DATA) {
			strm << verticalpositionerrorTag << endl;
		}
		else {
			strm << verticalpositionerrorTag << " " << verticalPosError << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writePositionError3D(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (posError == UNVALID_PL_DATA) {
			strm << positionerrorTag << endl;
		}
		else {
			strm << positionerrorTag << " " << posError << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeHorizontalAlarmLimit(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (HorizontalAlarmLimit == UNVALID_PL_DATA) {
			strm << horizontalalarmTag << endl;
		}
		else {
			strm << horizontalalarmTag << " " << HorizontalAlarmLimit << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeVerticalAlarmLimit(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (VerticalAlarmLimit == UNVALID_PL_DATA) {
			strm << verticalalarmTag << endl;
		}
		else {
			strm << verticalalarmTag << " " << VerticalAlarmLimit << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeAlarmLimit3D(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

		if (AlarmLimit == UNVALID_PL_DATA) {
			strm << alarmTag << endl;
		}
		else {
			strm << alarmTag << " " << AlarmLimit << endl;
		}

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeCovMatrixEcef(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);

	
		strm << startofcovmatrixecefTag << endl;
		strm.lineNumber++;

		if (Covariance_ecef.size() >=9 ) {
			strm.lineNumber++;
			strm << Covariance_ecef(0, 0) << " " << Covariance_ecef(0, 1) << " " << Covariance_ecef(0, 2) << endl;
			strm.lineNumber++;
			strm << Covariance_ecef(1, 0) << " " << Covariance_ecef(1, 1) << " " << Covariance_ecef(1, 2) << endl;
			strm.lineNumber++;
			strm << Covariance_ecef(2, 0) << " " << Covariance_ecef(2, 1) << " " << Covariance_ecef(2, 2) << endl;
			strm.lineNumber++;
		}
		

		strm << endofcovmatrixecefTag << endl;
		strm.lineNumber++;

		return true;
	}

	bool ProtectionLevel_Data::writeCovMatrixEnu(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);


		strm << startofcovmatrixenuTag << endl;
		strm.lineNumber++;

		if (Covariance_enu.size() >= 9) {
			strm.lineNumber++;
			strm << Covariance_enu(0, 0) << " " << Covariance_enu(0, 1) << " " << Covariance_enu(0, 2) << endl;
			strm.lineNumber++;
			strm << Covariance_enu(1, 0) << " " << Covariance_enu(1, 1) << " " << Covariance_enu(1, 2) << endl;
			strm.lineNumber++;
			strm << Covariance_enu(2, 0) << " " << Covariance_enu(2, 1) << " " << Covariance_enu(2, 2) << endl;
			strm.lineNumber++;
		}

		strm << endofcovmatrixenuTag << endl;

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeUsedSats(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);


		strm << startofusedsatTag << endl;
		strm.lineNumber++;

		for (int a = 0; a < includedSatIds.size(); a++) {

			switch (includedSatIds[a].system) {
			case gpstk::SatID::SatelliteSystem::systemGPS:
				strm << "0" << " " << includedSatIds[a].id << " ";
				break;
			case gpstk::SatID::SatelliteSystem::systemGalileo:
				strm << "2" << " " << includedSatIds[a].id << " ";
				break;
			case gpstk::SatID::SatelliteSystem::systemGlonass:
				strm << "6" << " " << includedSatIds[a].id << " ";
				break;
			default:
				continue;
				break;
			}

			bool satFound = false;
			for (int b = 0; b < satInfo.size(); b++) {

				if (includedSatIds[a] == satInfo[b].satId) {
					satFound = true;
					if (satInfo[b].el_deg_valid == true) {
						strm << "el " << to_string(satInfo[b].el_deg) << " ";
					}
					else {
						strm << "el " << "UNKNOWN" << " ";
					}

					if (satInfo[b].az_deg_valid == true) {
						strm << "az " << to_string(satInfo[b].az_deg) << " ";
					}
					else {
						strm << "az " << "UNKNOWN" << " ";
					}

					if (satInfo[b].ippLat_valid == true) {
						strm << "IPP lat " << to_string(satInfo[b].ippLat) << " ";
					}
					else {
						strm << "IPP lat " << "UNKNOWN" << " ";
					}

					if (satInfo[b].ippLon_valid == true) {
						strm << "IPP lon " << to_string(satInfo[b].ippLon) << " ";
					}
					else {
						strm << "IPP lon " << "UNKNOWN" << " ";
					}

					if (satInfo[b].ionoCorr_meter_valid == true) {
						strm << "Iono Delay [m] " << to_string(satInfo[b].ionoCorr_meter) << " ";
					}
					else {
						strm << "Iono Delay [m] " << "UNKNOWN" << " ";
					}

					if (satInfo[b].ionoRMS_meter_valid == true) {
						strm << "Iono RMS [m] " << to_string(satInfo[b].ionoRMS_meter) << " ";
					}
					else {
						strm << "Iono RMS [m] " << "UNKNOWN" << " ";
					}
					break;
				}
			}

			if (satFound == false) {
				strm << "el " << "UNKNOWN" << " ";
				strm << "az " << "UNKNOWN" << " ";
				strm << "IPP lat " << "UNKNOWN" << " ";
				strm << "IPP lon " << "UNKNOWN" << " ";
				strm << "Iono Delay [m] " << "UNKNOWN" << " ";
				strm << "Iono RMS [m] " << "UNKNOWN" << " ";
			}
		
			strm << endl;
			strm.lineNumber++;
		}

		strm << endofusedsatTag << endl;

		strm.lineNumber++;
		return true;
	}

	bool ProtectionLevel_Data::writeUnusedSats(gpstk::FFStream& ffs) const {

		ProtectionLevel_Stream& strm = dynamic_cast<ProtectionLevel_Stream&>(ffs);


		strm << startofunusedsatTag << endl;
		strm.lineNumber++;

		for (int a = 0; a < excludedSatIds.size(); a++) {

			switch (excludedSatIds[a].system) {
			case gpstk::SatID::SatelliteSystem::systemGPS:
				strm << "0" << " " << excludedSatIds[a].id << " ";
				break;
			case gpstk::SatID::SatelliteSystem::systemGalileo:
				strm << "2" << " " << excludedSatIds[a].id << " ";
				break;
			case gpstk::SatID::SatelliteSystem::systemGlonass:
				strm << "6" << " " << excludedSatIds[a].id << " ";
				break;
			default:
				continue;
				break;
			}
			bool satFound = false;
			for (int b = 0; b < satInfo.size(); b++) {

				if (excludedSatIds[a] == satInfo[b].satId) {

					satFound = true;

					if (satInfo[b].el_deg_valid == true) {
						strm << "el " << to_string(satInfo[b].el_deg) << " ";
					}
					else {
						strm << "el " << "UNKNOWN" << " ";
					}

					if (satInfo[b].az_deg_valid == true) {
						strm << "az " << to_string(satInfo[b].az_deg) << " ";
					}
					else {
						strm << "az " << "UNKNOWN" << " ";
					}

					if (satInfo[b].ippLat_valid == true) {
						strm << "IPP lat " << to_string(satInfo[b].ippLat) << " ";
					}
					else {
						strm << "IPP lat " << "UNKNOWN" << " ";
					}

					if (satInfo[b].ippLon_valid == true) {
						strm << "IPP lon " << to_string(satInfo[b].ippLon) << " ";
					}
					else {
						strm << "IPP lon " << "UNKNOWN" << " ";
					}

					if (satInfo[b].ionoCorr_meter_valid == true) {
						strm << "Iono Delay [m] " << to_string(satInfo[b].ionoCorr_meter) << " ";
					}
					else {
						strm << "Iono Delay [m] " << "UNKNOWN" << " ";
					}

					if (satInfo[b].ionoRMS_meter_valid == true) {
						strm << "Iono RMS [m] " << to_string(satInfo[b].ionoRMS_meter) << " ";
					}
					else {
						strm << "Iono RMS [m] " << "UNKNOWN" << " ";
					}
					break;
				}
			}

			if (satFound == false) {
				strm << "el " << "UNKNOWN" << " ";	
				strm << "az " << "UNKNOWN" << " ";		
				strm << "IPP lat " << "UNKNOWN" << " ";		
				strm << "IPP lon " << "UNKNOWN" << " ";		
				strm << "Iono Delay [m] " << "UNKNOWN" << " ";
				strm << "Iono RMS [m] " << "UNKNOWN" << " ";
			}

			strm << endl;
			strm.lineNumber++;
		}

		strm << endofunusedsatTag << endl;

		strm.lineNumber++;
		return true;
	}

	
}