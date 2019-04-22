
#include "ProtectionLevel_Stream.hpp"

namespace ProtectionLevel_Parser {
	ProtectionLevel_Stream::ProtectionLevel_Stream(){
		init();
	}

	ProtectionLevel_Stream::ProtectionLevel_Stream(const char* fn, std::ios::openmode mode)
		: FFTextStream(fn, mode){
		init();
	}

	ProtectionLevel_Stream::~ProtectionLevel_Stream(){
	}

	void ProtectionLevel_Stream ::
		open(const char* fn, std::ios::openmode mode){
		FFTextStream::open(fn, mode);
		init();
	}

	void ProtectionLevel_Stream ::
		init(){
	}

}