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

		gpstk::CivilTime civTime = this->dataTime;
		/*strm << this->int2string(civTime.year - EMS_YEAR_OFFSET) << " ";
		strm << this->int2string(civTime.month) << " ";
		strm << this->int2string(civTime.day) << " ";
		strm << this->int2string(civTime.hour) << " ";
		strm << this->int2string(civTime.minute) << " ";
		strm << this->int2string(civTime.second) << " ";
		strm << this->messageId << " ";
		strm << this->bitset2hexstring();*/

		strm << endl;
		strm.lineNumber++;
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

	void ProtectionLevel_Data::parseLine(std::string& currentLine)
		throw(gpstk::StringUtils::StringException, gpstk::FFStreamError)
	{
		try
		{
			
			/*std::stringstream   ss(currentLine);
			string satId, year, month, day, hour, minute, second, msgId, hex_message;
				
			ss >> satId >> year >> month >> day >> hour >> minute >> second >> msgId >> std::hex >> hex_message;

			// Set raw message in binary form
			// We have to reverse the binary string before bc of the properties of bitset.
			string bin = this->HexStrToBin(hex_message);
			this->reverseStr(bin);
			std::bitset<256> message_temp(bin);
			message = message_temp;

			// Set message Id
			messageId = stoi(msgId, nullptr);

			// Set SvId
			svId = stoi(satId, nullptr);
			
			// Set time
			gpstk::CivilTime civTime;
			civTime.year = EMS_YEAR_OFFSET + stoi(year, nullptr);
			civTime.month = stoi(month, nullptr);
			civTime.day = stoi(day, nullptr);
			civTime.hour = stoi(hour, nullptr);
			civTime.minute = stoi(minute, nullptr);
			civTime.second = stoi(second, nullptr);

			// EMS is in GPS time
			if (civTime.hour == 24 && civTime.minute == 0 && civTime.second == 0) {
				civTime.hour = 23;
				civTime.minute = 59;  
				civTime.second = 59.999;
			}

			civTime.setTimeSystem(gpstk::TimeSystem::GPS);
			this->messageTime = civTime;*/
			
		}
		catch (std::exception &e)
		{
			gpstk::FFStreamError err("std::exception: " +
				string(e.what()));
			GPSTK_THROW(err);
		}
	}

}