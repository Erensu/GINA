#pragma once

#include "FFTextStream.hpp"
#include "EGNOS_ftp_data_Header.hpp"

namespace EGNOS_ftp_data_Parser {

	class EGNOS_ftp_data_Stream : public gpstk::FFTextStream
	{
	public:
		EGNOS_ftp_data_Stream();

		/** Constructor
		* Opens a file named \a fn using ios::openmode \a mode.
		*/
		EGNOS_ftp_data_Stream(const char* fn, std::ios::openmode mode = std::ios::in);

		/// Destructor
		virtual ~EGNOS_ftp_data_Stream();

		/// overrides open to reset the header
		virtual void open(const char* fn, std::ios::openmode mode);

		/// RINEX NAV header for this file.
		EGNOS_ftp_data_Header header;
		
		/// Flag showing whether or not the header has been read.
		bool headerRead = false;
		bool dataStartRead= false;
		bool dataStartWritten = false;

	private:
		void init();
	};

	//@}


}

