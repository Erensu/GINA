#include "RTKPOST_Header.hpp"
#include "RTKPOST_Stream.hpp"

#include "CivilTime.hpp"

namespace RTKPOST_Parser {


	const string RTKPOST_Pos_Header::GINAProgram = "GINA Project RTKPOST_Parser 1.0";
	const string RTKPOST_Pos_Header::headerprefixTag = "%";
	const string RTKPOST_Pos_Header::programTag = "program   : ";
	const string RTKPOST_Pos_Header::inpFileTag = "inp file  : ";
	const string RTKPOST_Pos_Header::obsStartTag = "obs start : ";
	const string RTKPOST_Pos_Header::obsEndTag = "obs end   : ";
	const string RTKPOST_Pos_Header::posModeTag = "pos mode  : ";
	const string RTKPOST_Pos_Header::elevMaskTag = "elev mask : ";
	const string RTKPOST_Pos_Header::ionosOptTag = "ionos opt : ";
	const string RTKPOST_Pos_Header::tropoOptTag = "tropo opt : ";
	const string RTKPOST_Pos_Header::ephemerisOptTag = "ephemeris : ";
	const string RTKPOST_Pos_Header::tokenTag = "(lat/lon/height=WGS84/ellipsoidal,Q=1:fix,2:float,3:sbas,4:dgps,5:single,6:ppp,ns=# of satellites)";
	const string RTKPOST_Pos_Header::preTokenTag = "(lat/lon/height=";
	const string RTKPOST_Pos_Header::coloumnTag = " GPST                  latitude(deg) longitude(deg)  height(m)   Q  ns   sdn(m)   sde(m)   sdu(m)  sdne(m)  sdeu(m)  sdun(m) age(s)  ratio";
	const string RTKPOST_Pos_Header::degreeTag = "deg";

	const string RTKPOST_Pos_Header::gpsTimeTag = "GPST";

	void RTKPOST_Pos_Header::reallyGetRecord(gpstk::FFStream& ffs)
		throw(exception,
			gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		// if already read, just return
		if (strm.headerRead == true)
			return;

		while (1) {

			string line;
			streampos oldpos = strm.tellg();  // stores the position
			strm.formattedGetLine(line);
			gpstk::StringUtils::stripTrailing(line);
			
			numberOfLineRead++;

			if (line.length() == 0) {
				strm.lineNumber--;
				numberOfLineRead--;
				strm.seekg(oldpos);				// get back to the position
				break;
			};

			// Parse File
			if (isPartOfHeader(line, ffs) == false) {
				strm.lineNumber--;
				numberOfLineRead--;
				strm.seekg(oldpos);				// get back to the position
				break;
			};

			if (hasCreatorFound(line)) continue;
			else if (hasInpFileFound(line)) continue;

			//else if (hasTimeSystemFound(line)) continue;
			else if (hasStartTimeFound(line)) continue;
			else if (hasEndTimeFound(line)) continue;

			else if (hasPosModeFound(line)) continue;
			else if (hasElevMaskFound(line)) continue;
			else if (hasIonosOptFound(line)) continue;
			else if (hasTropoOptFound(line)) continue;
			else if (hasEphemerisOptFound(line)) continue;
			else if (hasPreTokenTagFound(line)) continue;
			
			else if (hasColoumnTagFound(line, ffs)) continue;
			else {
				UnknownHeaderLines[numberOfLineRead-1] = line;
			};

		}

		strm.headerRead = true;
	}

	void RTKPOST_Pos_Header::reallyPutRecord(gpstk::FFStream& ffs) const
		throw(exception, gpstk::FFStreamError, gpstk::StringUtils::StringException) {
		
		const RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		bool programInfoWritten = false;
		bool inpFilesWritten = false;
		bool obsStartWritten = false;
		bool obsEndWritten = false;
		bool posModeWritten = false;
		bool elevAngleWritten = false;
		bool ionosOptWritten = false;
		bool tropoOptWritten = false;
		bool ephemerisOptWritten = false;
		bool tokenWritten = false;
		bool datumWritten = false;
		bool coloumnWritten = false;

		while (strm.lineNumber < numberOfLineRead) {

			if (writeUnknownLines(ffs)) continue;
			if (writeProgramInfoFound(ffs, programInfoWritten)) continue;
			if (writeInpFileFound(ffs, inpFilesWritten)) continue;
			if (writeObsStartFound(ffs, obsStartWritten)) continue;

			if (writeObsEndFound(ffs, obsEndWritten)) continue;
			if (writePosModeFound(ffs, posModeWritten)) continue;
			
			if (writeElevAngleSystemFound(ffs, elevAngleWritten)) continue;
			if (writeIonosOptFound(ffs, ionosOptWritten)) continue;
			if (writeTropoOptFound(ffs, tropoOptWritten)) continue;
			if (writeEphemerisOptFound(ffs, ephemerisOptWritten)) continue;
			if (writeTokenFound(ffs, tokenWritten)) continue;
			if (writeColoumnFound(ffs, coloumnWritten)) continue;
			if (writeProgramInfoFound(ffs, programInfoWritten)) continue;
			if (writeProgramInfoFound(ffs, programInfoWritten)) continue;
			if (writeProgramInfoFound(ffs, programInfoWritten)) continue;

		}
		

		return;
	}
		
	void RTKPOST_Pos_Header::dump(ostream& s) const {

	}

	/*Read methods*/
	bool RTKPOST_Pos_Header::isPartOfHeader(string& line, gpstk::FFStream& ffs) {

		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);
		bool rtnv = false;
		
		std::size_t index = line.find(headerprefixTag);
		if (index != std::string::npos && index == 0) {

			if (line.length() > 2) {
				line = line.substr(index + 2, line.length() - 1);
				
			}
			else { 
				line = "";
			}
			return true;
		}

		return false;
	}

	bool RTKPOST_Pos_Header::hasStartTimeFound(string &line) {

		if (line.find(obsStartTag) != string::npos) {

			std::string infoStr;
			std::string week;
			std::string tow;

			std::size_t index = line.find(":");
			if (index != std::string::npos) {
				infoStr = line.substr(index + 2, line.length() - 1);
				gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
				gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);

				index = infoStr.find(gpsTimeTag);
				if (index == std::string::npos) return true;

				infoStr = infoStr.substr(index + 5, line.length() - 1);
				index = infoStr.find(' ');
				if (index == std::string::npos) return true;

				week = infoStr.substr(5, index-5);
				std::size_t index_end = infoStr.find("s)");
				if (index_end == std::string::npos) return true;

				tow = infoStr.substr(index, index_end - index);

				// Set time system
				if (line.find("GPST") != std::string::npos) {
					obsStart.setTimeSystem(gpstk::TimeSystem::GPS);
				}	
				else {
					obsStart.setTimeSystem(gpstk::TimeSystem::Unknown);
				}

				gpstk::GPSWeekSecond gpsTime;
				gpsTime.week = stoi(week, nullptr);
				gpsTime.sow = stod(tow, nullptr);

				obsStart = gpsTime;
			}
			else {
				obsEnd.setTimeSystem(gpstk::TimeSystem::Unknown);
			}

			return true;
		}
		else {
			return false;
		}
		return false;
	}

	bool RTKPOST_Pos_Header::hasEndTimeFound(string &line) {

		if (line.find(obsEndTag) != string::npos) {

			std::string infoStr;
			std::string week;
			std::string tow;

			std::size_t index = line.find(":");
			if (index != std::string::npos) {
				infoStr = line.substr(index + 2, line.length() - 1);
				gpstk::StringUtils::stripTrailing(infoStr, std::string(1, ' '), std::string::npos);
				gpstk::StringUtils::stripTrailing(infoStr, std::string(1, '\t'), std::string::npos);

				index = infoStr.find(gpsTimeTag);
				if (index == std::string::npos) return true;

				infoStr = infoStr.substr(index + 5, line.length() - 1);
				index = infoStr.find(' ');
				if (index == std::string::npos) return true;

				week = infoStr.substr(5, index - 5);
				std::size_t index_end = infoStr.find("s)");
				if (index_end == std::string::npos) return true;

				tow = infoStr.substr(index, index_end - index);

				// Set time system
				if (line.find("GPST") != std::string::npos) {
					obsEnd.setTimeSystem(gpstk::TimeSystem::GPS);
				}
				else {
					obsEnd.setTimeSystem(gpstk::TimeSystem::Unknown);
				}
				
				gpstk::GPSWeekSecond gpsTime;
				gpsTime.week = stoi(week, nullptr);
				gpsTime.sow = stod(tow, nullptr);

				obsEnd = gpsTime;
			}
			else {
				obsEnd.setTimeSystem(gpstk::TimeSystem::Unknown);
			}

			return true;
		}
		else {
			return false;
		}
		return false;
	}

	bool RTKPOST_Pos_Header::hasPosModeFound(string& line) {

		if (line.find(posModeTag) != string::npos) {

			std::size_t index = line.find(":");
			if (index != std::string::npos) {
				posMode = line.substr(index + 2, line.length() - 1);
				gpstk::StringUtils::stripTrailing(posMode, std::string(1, ' '), std::string::npos);
				gpstk::StringUtils::stripTrailing(posMode, std::string(1, '\t'), std::string::npos);
			}
			else {
				posMode = "Unknown";
			}

			return true;
		}
		else {
			return false;
		}
		return false;

	}

	bool RTKPOST_Pos_Header::hasElevMaskFound(string& line) {

		if (line.find(elevMaskTag) != string::npos) {

			std::size_t index = line.find(":");
			if (index != std::string::npos) {
				std::string  infoStr  = line.substr(index + 2, line.length() - 1);
				gpstk::StringUtils::stripTrailing(posMode, std::string(1, ' '), std::string::npos);
				gpstk::StringUtils::stripTrailing(posMode, std::string(1, '\t'), std::string::npos);
			
				std::size_t index_end = infoStr.find(degreeTag);
				std::string  elevStr = infoStr.substr(0, index_end - 1);
				elevAngle = stod(elevStr, nullptr);
			}
			else {
				posMode = "Unknown";
			}

			return true;
		}
		else {
			return false;
		}
		return false;
	}

	bool RTKPOST_Pos_Header::hasIonosOptFound(string& line) {


		if (line.find(ionosOptTag) != string::npos) {

			std::size_t index = line.find(":");
			if (index != std::string::npos) {
				ionosOpt = line.substr(index + 2, line.length() - 1);
				gpstk::StringUtils::stripTrailing(ionosOpt, std::string(1, ' '), std::string::npos);
				gpstk::StringUtils::stripTrailing(ionosOpt, std::string(1, '\t'), std::string::npos);
			}
			else {
				ionosOpt = "Unknown";
			}

			return true;
		}
		else {
			return false;
		}
		return false;

	}

	bool RTKPOST_Pos_Header::hasTropoOptFound(string& line) {
		if (line.find(tropoOptTag) != string::npos) {

			std::size_t index = line.find(":");
			if (index != std::string::npos) {
				tropoOpt = line.substr(index + 2, line.length() - 1);
				gpstk::StringUtils::stripTrailing(tropoOpt, std::string(1, ' '), std::string::npos);
				gpstk::StringUtils::stripTrailing(tropoOpt, std::string(1, '\t'), std::string::npos);
			}
			else {
				tropoOpt = "Unknown";
			}

			return true;
		}
		else {
			return false;
		}
		return false;

	}

	bool RTKPOST_Pos_Header::hasEphemerisOptFound(string& line) {
		if (line.find(ephemerisOptTag) != string::npos) {

			std::size_t index = line.find(":");
			if (index != std::string::npos) {
				ephemerisOpt = line.substr(index + 2, line.length() - 1);
				gpstk::StringUtils::stripTrailing(ephemerisOpt, std::string(1, ' '), std::string::npos);
				gpstk::StringUtils::stripTrailing(ephemerisOpt, std::string(1, '\t'), std::string::npos);
			}
			else {
				ephemerisOpt = "Unknown";
			}

			return true;
		}
		else {
			return false;
		}
		return false;
	}

	bool RTKPOST_Pos_Header::hasPreTokenTagFound(string& line) {

		if (line.find(preTokenTag) != string::npos) {
			gpstk::StringUtils::stripTrailing(line, std::string(1, ' '), std::string::npos);
			token = line;
			std::size_t index = line.find("/ellipsoidal");
			if (index != std::string::npos) {

				std::string datumStr = line.substr(16, index-16);

				if (datumStr == "WGS84") {

					datum = gpstk::ReferenceFrame::WGS84;
				}

			}
			else {
				datum = gpstk::ReferenceFrame::Unknown;
			}

			return true;
		}

		return false;
	}

	bool RTKPOST_Pos_Header::hasColoumnTagFound(string& line, gpstk::FFStream& ffs) {

		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);
		if (line.find(coloumnTag) != string::npos) {

			if (line.find("GPST") != string::npos) {
				timeSys = gpstk::TimeSystem::GPS;
			}
			else {
				timeSys = gpstk::TimeSystem::Unknown;
			}
			
			return true;
		}

		return false;
	}

	bool RTKPOST_Pos_Header::hasCreatorFound(string& line) {
		
		if (line.find(programTag) != std::string::npos) {
			
			std::size_t index = line.find(":");
			if (index != std::string::npos) {
				programInfo = line.substr(index + 2, line.length()-1);
				gpstk::StringUtils::stripTrailing(programInfo, std::string(1, ' '), std::string::npos);
				gpstk::StringUtils::stripTrailing(programInfo, std::string(1, '\t'), std::string::npos);
			}
			else {
				programInfo = "Unknown";
			}

			return true;
		}
		else {
			return false;
		}
		return true;
	}

	bool RTKPOST_Pos_Header::hasInpFileFound(string& line) {
		if (line.find(inpFileTag) != string::npos) {

			std:string inpFile;

			std::size_t index = line.find(":");
			if (index != std::string::npos) {
				inpFile = line.substr(index + 2, line.length() - 1);
				gpstk::StringUtils::stripTrailing(inpFile, std::string(1, ' '), std::string::npos);
				gpstk::StringUtils::stripTrailing(inpFile, std::string(1, '\t'), std::string::npos);
			}
			else {
				inpFile = "Unknown";
			}

			inpFiles.push_back(inpFile);

			return true;
		}
		else {
			return false;
		}
		return true;
	}

	/*Write methods*/

	bool RTKPOST_Pos_Header::writeProgramInfoFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (dataWritten == false) {

			dataWritten = true;
			strm << headerprefixTag  << " " << programTag << programInfo << endl;
			strm.lineNumber++;
			return true;

		}
		else{
			return false;
		}

	}

	bool RTKPOST_Pos_Header::writeInpFileFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (dataWritten == false) {

			dataWritten = true;
			if (inpFiles.size() > 0) {

				for (size_t i = 0; i < inpFiles.size(); i++)
				{
					strm << headerprefixTag << " " << inpFileTag << inpFiles[i] << endl;
					strm.lineNumber++;
				}
			}
			else {
				strm << headerprefixTag << " " << inpFileTag << "Unknown" << endl;
				strm.lineNumber++;
			}
			
			return true;
		}
		else {
		
			return false;
		}
	}

	bool RTKPOST_Pos_Header::writeObsStartFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (dataWritten == false) {

			dataWritten = true;
			if (obsStart.getTimeSystem() == gpstk::TimeSystem::Unknown) {
				strm << headerprefixTag << " " << obsStartTag << "Unknown" << endl;
			}
			else {

				gpstk::CivilTime civTime = obsStart;
				gpstk::GPSWeekSecond  weekSec = obsStart;
				strm << headerprefixTag;
				strm << " " << obsStartTag;
				strm << civTime.year << "/";
				if (civTime.month < 10) { strm << "0" << civTime.month; }
				else { strm << civTime.month; }
				strm << "/";
				if (civTime.day < 10) { strm << "0" << civTime.day; }
				else { strm << civTime.day; }
				strm << " ";
				if (civTime.hour < 10) { strm << "0" << civTime.hour; }
				else { strm << civTime.hour; }
				strm << ":";
				if (civTime.minute < 10) { strm << "0" << civTime.minute; }
				else { strm << civTime.minute; }
				strm << ":";
				strm << std::fixed;
				strm << setprecision(1);
				if (civTime.second < 10) { strm << "0" << civTime.second; }
				else { strm << civTime.second; }
				strm << " ";
				strm << obsStart.getTimeSystem() << "T";
				strm << " (week";
				strm << weekSec.week << " ";
				strm << setprecision(1);
				strm << std::fixed;
				if (weekSec.sow < 10) { strm << "0" << weekSec.sow; }
				else { strm << weekSec.sow; }
				strm << "s)";
				strm << endl;
			}
			
			strm.lineNumber++;
			return true;
		}
		else {
			return false;
		}
		return false;
	}

	bool RTKPOST_Pos_Header::writeObsEndFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);
		if (dataWritten == false) {

			dataWritten = true;
			if (obsEnd.getTimeSystem() == gpstk::TimeSystem::Unknown) {
				strm << headerprefixTag << " " << obsEndTag << "Unknown" << endl;
			}
			else {
				gpstk::CivilTime civTime = obsEnd;
				gpstk::GPSWeekSecond  weekSec = obsEnd;

				std::string timeFrame = "Unknown";

				if (obsEnd.getTimeSystem() == gpstk::TimeSystem::GPS) {
					timeFrame = "GPST";
				}

				strm << headerprefixTag;
				strm << " " << obsEndTag;
				strm << civTime.year << "/";
				if (civTime.month < 10) { strm << "0" << civTime.month; }
				else { strm << civTime.month; }
				strm << "/";
				if (civTime.day < 10) { strm << "0" << civTime.day; }
				else { strm << civTime.day; }
				strm << " ";
				if (civTime.hour < 10) { strm << "0" << civTime.hour; }
				else { strm << civTime.hour; }
				strm << ":";
				if (civTime.minute < 10) { strm << "0" << civTime.minute; }
				else { strm << civTime.minute; }
				strm << ":";
				strm << std::fixed;
				strm << setprecision(1);
				if (civTime.second < 10) { strm << "0" << civTime.second; }
				else { strm << civTime.second; }
				strm << " ";
				strm << obsStart.getTimeSystem() << "T";
				strm << " (week";
				strm << weekSec.week << " ";
				strm << setprecision(1);
				if (weekSec.sow < 10) { strm << "0" << weekSec.sow; }
				else { strm << weekSec.sow; }
				strm << "s)";
				strm << endl;
			}

			strm.lineNumber++;
			return true;
		}
		else {
			return false;
		}
		return false;
	}

	bool RTKPOST_Pos_Header::writePosModeFound(gpstk::FFStream& ffs, bool &dataWritten) const {

		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);
		if (dataWritten == false) {

			dataWritten = true;
			strm << headerprefixTag << " " << posModeTag << posMode << endl;
			strm.lineNumber++;
		
			return true;
		}
		else {

			return false;
		}
	}

	bool RTKPOST_Pos_Header::writeElevAngleSystemFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (dataWritten == false) {

			dataWritten = true;
			strm << setprecision(1);
			strm << std::fixed;
			strm << headerprefixTag << " " << elevMaskTag << elevAngle << " " << degreeTag << endl;
			strm.lineNumber++;

			return true;
		}
		else {

			return false;
		}

		return false;
	}

	bool RTKPOST_Pos_Header::writeIonosOptFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (dataWritten == false) {

			dataWritten = true;
			strm << headerprefixTag << " " << ionosOptTag << ionosOpt << endl;
			strm.lineNumber++;

			return true;
		}
		else {

			return false;
		}

		return false;
	}

	bool RTKPOST_Pos_Header::writeTropoOptFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (dataWritten == false) {

			dataWritten = true;
			strm << headerprefixTag << " " << tropoOptTag << tropoOpt << endl;
			strm.lineNumber++;

			return true;
		}
		else {

			return false;
		}

		return false;
	}

	bool RTKPOST_Pos_Header::writeEphemerisOptFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (dataWritten == false) {

			dataWritten = true;
			strm << headerprefixTag << " " << ephemerisOptTag << ephemerisOpt << endl;
			strm.lineNumber++;

			return true;
		}
		else {

			return false;
		}

		return false;
	}

	bool RTKPOST_Pos_Header::writeTokenFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (dataWritten == false) {

			dataWritten = true;
			strm << headerprefixTag << " " << token << endl;
			strm.lineNumber++;

			return true;
		}
		else {

			return false;
		}

		return false;
	}

	bool RTKPOST_Pos_Header::writeColoumnFound(gpstk::FFStream& ffs, bool &dataWritten) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (dataWritten == false) {

			dataWritten = true;
			strm << headerprefixTag << " " << coloumnTag << endl;
			strm.lineNumber++;

			return true;
		}
		else {

			return false;
		}

		return false;
	}


	bool RTKPOST_Pos_Header::writeUnknownLines(gpstk::FFStream& ffs) const {
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		if (UnknownHeaderLines.find(strm.lineNumber) == UnknownHeaderLines.end()) {
			return false;
		}
		else {

			strm << headerprefixTag << " " << UnknownHeaderLines.at(strm.lineNumber) << endl;
			strm.lineNumber++;
			return true;
		}


		return false;
	}


}
