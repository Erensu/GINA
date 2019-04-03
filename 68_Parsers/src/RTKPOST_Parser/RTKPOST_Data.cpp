
#include "RTKPOST_Data.hpp"


namespace RTKPOST_Parser
{
	using namespace std;

	void RTKPOST_Pos_Data::reallyPutRecord(gpstk::FFStream& ffs) const
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);

		// Date
		gpstk::CivilTime civTime(dataTime);
		strm << civTime.year;
		strm << "/";
		if (civTime.month < 10) { strm << "0" << civTime.month; }
		else { strm << civTime.month; }
		strm << "/";
		if (civTime.day < 10) {	strm << "0" << civTime.day;	}
		else { strm << civTime.day; }
		strm << " ";
		if (civTime.hour < 10) { strm << "0" << civTime.hour; }
		else { strm << civTime.hour; }
		strm << ":";
		if (civTime.minute < 10) { strm << "0" << civTime.minute; }
		else { strm << civTime.minute; }
		strm << ":";
		strm << std::fixed;
		strm << std::setprecision(3);
		if (civTime.second < 10) { strm << "0" << civTime.second; }
		else { strm << civTime.second; }
		strm << "   ";
	
		// Lat lon height
		strm << std::fixed;
		strm << std::setprecision(9) << pos.geodeticLatitude() << "   " << pos.longitude() << "   " << std::setprecision(4) << pos.height() << "   ";

		strm << typeOfSolution;

		if (numberOfSvId > 9) { strm << "  " << numberOfSvId; }
		else { strm << "   " << numberOfSvId; }
		
		strm << "   ";

		strm << std::setprecision(4);
		strm << sdn << "   ";
		strm << sde << "   ";
		strm << sdu;

		if (sdne > 0) { strm << "   " << sdne; }
		else { strm << "  -" << abs(sdne); }

		if (sdeu > 0) { strm << "   " << sdeu; }
		else { strm << "  -" << abs(sdeu); }

		if (sdun > 0) { strm << "   " << sdun ; }
		else { strm << "  -" << abs(sdun) ; }
		
		strm << "   ";

		strm << std::setprecision(2);
		strm << age << "    ";
		strm << std::setprecision(1);
		strm << ratio;
	
		strm << endl;
		strm.lineNumber++;
	}



	void RTKPOST_Pos_Data::reallyGetRecord(gpstk::FFStream& ffs)
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {
				
		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);
		this->strm = &strm;

		string line;

		if (!strm.headerRead) {
			strm >> strm.header;
		}
			


		strm.formattedGetLine(line, true);
		gpstk::StringUtils::stripTrailing(line);
		parseLine(line);

		return;
	}

	
	
	void RTKPOST_Pos_Data::parseLine(std::string& currentLine)
		throw(gpstk::StringUtils::StringException, gpstk::FFStreamError)
	{
		try
		{
			
			std::stringstream   ss(currentLine);
			string yyyymmdd, hhmmss, lat, lon, height, Q, numberOfSat, sdn, sde, sdu, sdne, sdeu, sdun, age, ratio;
				
			ss >> yyyymmdd >> hhmmss >> lat >> lon >> height >> Q >> numberOfSat >> sdn >> sde >> sdu >> sdne >> sdeu >> sdun >> age >> ratio;

			gpstk::CivilTime time;
			time.setTimeSystem(strm->header.timeSys);
			time.year = stoi(yyyymmdd.substr(0, 4));
			time.month = stoi(yyyymmdd.substr(5, 2));
			time.day = stoi(yyyymmdd.substr(8, 2));
			time.hour = stoi(hhmmss.substr(0, 2));
			time.minute = stod(hhmmss.substr(3, 2));
			time.second = stod(hhmmss.substr(6, 6));
			dataTime = time;

			gpstk::Position posTemp(std::stod(lat), std::stod(lon), std::stod(height), gpstk::Position::CoordinateSystem::Geodetic, NULL, strm->header.datum);
			pos = posTemp;
			
			this->typeOfSolution = stoi(Q);
			this->numberOfSvId = stoi(numberOfSat);
			this->sdn = std::stod(sdn);
			this->sde = std::stod(sde);
			this->sdu = std::stod(sdu);
			this->sdne = std::stod(sdne);
			this->sdeu = std::stod(sdeu);
			this->sdun = std::stod(sdun);
			this->age = std::stod(age);
			this->ratio = std::stod(ratio);
		}
		catch (std::exception &e)
		{
			gpstk::FFStreamError err("std::exception: " +
				string(e.what()));
			GPSTK_THROW(err);
		}
	}

}