
#include "EGNOS_ftp_data_Stream.hpp"

namespace EGNOS_ftp_data_Parser {
	EGNOS_ftp_data_Stream::EGNOS_ftp_data_Stream(){
		init();
	}

	EGNOS_ftp_data_Stream::EGNOS_ftp_data_Stream(const char* fn, std::ios::openmode mode)
		: FFTextStream(fn, mode){
		init();
	}

	EGNOS_ftp_data_Stream::~EGNOS_ftp_data_Stream(){
	}

	void EGNOS_ftp_data_Stream ::
		open(const char* fn, std::ios::openmode mode){
		FFTextStream::open(fn, mode);
		init();
	}

	void EGNOS_ftp_data_Stream ::
		init(){
		headerRead = false;
		header = EGNOS_ftp_data_Header();
	}

}