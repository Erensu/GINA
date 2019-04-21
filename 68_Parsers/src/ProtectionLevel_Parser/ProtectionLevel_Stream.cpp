
#include "EGNOS_EMS_Stream.hpp"

namespace EGNOS_EMS_Parser {
	EGNOS_EMS_Stream::EGNOS_EMS_Stream(){
		init();
	}

	EGNOS_EMS_Stream::EGNOS_EMS_Stream(const char* fn, std::ios::openmode mode)
		: FFTextStream(fn, mode){
		init();
	}

	EGNOS_EMS_Stream::~EGNOS_EMS_Stream(){
	}

	void EGNOS_EMS_Stream ::
		open(const char* fn, std::ios::openmode mode){
		FFTextStream::open(fn, mode);
		init();
	}

	void EGNOS_EMS_Stream ::
		init(){
	}

}