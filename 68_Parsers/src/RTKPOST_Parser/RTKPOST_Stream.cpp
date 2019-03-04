
#include "RTKPOST_Stream.hpp"

namespace RTKPOST_Parser {
	RTKPOST_Pos_Stream::RTKPOST_Pos_Stream(){
		init();
	}

	RTKPOST_Pos_Stream::RTKPOST_Pos_Stream(const char* fn, std::ios::openmode mode)
		: FFTextStream(fn, mode){
		init();
	}

	RTKPOST_Pos_Stream::~RTKPOST_Pos_Stream(){
	}

	void RTKPOST_Pos_Stream ::
		open(const char* fn, std::ios::openmode mode){
		FFTextStream::open(fn, mode);
		init();
	}

	void RTKPOST_Pos_Stream ::
		init(){
	}

}