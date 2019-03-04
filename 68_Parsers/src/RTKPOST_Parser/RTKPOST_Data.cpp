
#include "RTKPOST_Data.hpp"


namespace RTKPOST_Parser
{
	using namespace std;

	void RTKPOST_Pos_Data::reallyPutRecord(gpstk::FFStream& ffs) const
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);


		throw domain_error("");
		/*strm << this->svId << " ";

		gpstk::CivilTime civTime = this->messageTime;
		strm << this->int2string(civTime.year - EMS_YEAR_OFFSET) << " ";
		strm << this->int2string(civTime.month) << " ";
		strm << this->int2string(civTime.day) << " ";
		strm << this->int2string(civTime.hour) << " ";
		strm << this->int2string(civTime.minute) << " ";
		strm << this->int2string(civTime.second) << " ";
		strm << this->messageId << " ";
		strm << this->bitset2hexstring();

		strm << endl;*/
		strm.lineNumber++;
	}

	std::string RTKPOST_Pos_Data::bitset2hexstring(void) const {

		string bin = this->reverseStr(this->message.to_string());
		string hex;
		for (size_t i = 0; i <= bin.length()-4; i+=4)
		{
			hex += this->getHexCharacter(bin.substr(i, 4));
		}

		return hex;
	}

	std::string RTKPOST_Pos_Data::int2string(unsigned int number) const {
		std::string s = std::to_string(number);

		if (number < 10) {
			return "0" + s;
		}
		else if (number >= 10 && number < 100 ){
			return s;
		}
		else{
			throw gpstk::StringUtils::StringException(" The date number had 3 digits. Maximum allowable digit number is 2.");
		}
	}

	void RTKPOST_Pos_Data::reallyGetRecord(gpstk::FFStream& ffs)
		throw(std::exception, gpstk::FFStreamError,
			gpstk::StringUtils::StringException) {

		RTKPOST_Pos_Stream& strm = dynamic_cast<RTKPOST_Pos_Stream&>(ffs);
		this->strm = &strm;

		this->reset();

		std::string line;
		strm.formattedGetLine(line, true);
		gpstk::StringUtils::stripTrailing(line);
		parseLine(line);
		
		return;
	}

	double RTKPOST_Pos_Data::getGPSWeek(void) {
		// Set common time
		gpstk::CommonTime temp(messageTime);
		gpstk::GPSWeekSecond GPSTime(temp);
		return GPSTime.getWeek();
	}

	double RTKPOST_Pos_Data::getGPSToW(void) {
		gpstk::CommonTime temp(messageTime);
		gpstk::GPSWeekSecond GPSTime(temp);
		return GPSTime.getSOW();
	}

	void RTKPOST_Pos_Data::reset(void) {

		messageTime.reset();
		message.reset();
		messageId = 0;
		svId = 0;
	}
	
	void RTKPOST_Pos_Data::parseLine(std::string& currentLine)
		throw(gpstk::StringUtils::StringException, gpstk::FFStreamError)
	{
		try
		{
			
			std::stringstream   ss(currentLine);
			string satId, year, month, day, hour, minute, second, msgId, hex_message;
				
			ss >> satId >> year >> month >> day >> hour >> minute >> second >> msgId >> std::hex >> hex_message;

			
		}
		catch (std::exception &e)
		{
			gpstk::FFStreamError err("std::exception: " +
				string(e.what()));
			GPSTK_THROW(err);
		}
	}



	std::string RTKPOST_Pos_Data::HexCharToBin(char c) {
		switch (c) {
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'A': return "1010";
		case 'B': return "1011";
		case 'C': return "1100";
		case 'D': return "1101";
		case 'E': return "1110";
		case 'F': return "1111";
		}
	}

	char RTKPOST_Pos_Data::getHexCharacter(std::string str) const
	{
		if (str.compare("1111") == 0) return 'F';
		else if (str.compare("1110") == 0) return 'E';
		else if (str.compare("1101") == 0) return 'D';
		else if (str.compare("1100") == 0) return 'C';
		else if (str.compare("1011") == 0) return 'B';
		else if (str.compare("1010") == 0) return 'A';
		else if (str.compare("1001") == 0) return '9';
		else if (str.compare("1000") == 0) return '8';
		else if (str.compare("0111") == 0) return '7';
		else if (str.compare("0110") == 0) return '6';
		else if (str.compare("0101") == 0) return '5';
		else if (str.compare("0100") == 0) return '4';
		else if (str.compare("0011") == 0) return '3';
		else if (str.compare("0010") == 0) return '2';
		else if (str.compare("0001") == 0) return '1';
		else if (str.compare("0000") == 0) return '0';
		else if (str.compare("111") == 0) return '7';
		else if (str.compare("110") == 0) return '6';
		else if (str.compare("101") == 0) return '5';
		else if (str.compare("100") == 0) return '4';
		else if (str.compare("011") == 0) return '3';
		else if (str.compare("010") == 0) return '2';
		else if (str.compare("001") == 0) return '1';
		else if (str.compare("000") == 0) return '0';
		else if (str.compare("11") == 0) return '3';
		else if (str.compare("10") == 0) return '2';
		else if (str.compare("01") == 0) return '1';
		else if (str.compare("00") == 0) return '0';
		else if (str.compare("1") == 0) return '1';
		else if (str.compare("0") == 0) return '0';
	}


	std::string RTKPOST_Pos_Data::HexStrToBin(const std::string & hs) {
		std::string bin;
		for (auto c : hs) {
			bin += this->HexCharToBin(c);
		}
		return bin;
	}

	std::string RTKPOST_Pos_Data::reverseStr(std::string& str) const
	{
		std::string str_out;
		int n = str.length();

		// Swap character starting from two 
		// corners 
		for (int i = 0; i < n / 2; i++)
			std::swap(str[i], str[n - i - 1]);

		str_out = str;

		return str_out;
	}
}