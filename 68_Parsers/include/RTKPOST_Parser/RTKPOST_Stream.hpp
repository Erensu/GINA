#ifndef RTKPOST_STREAM_HPP
#define RTKPOST_STREAM_HPP

#include "FFTextStream.hpp"
#include "RTKPOST_Header.hpp"

namespace RTKPOST_Parser {

	class RTKPOST_Pos_Stream : public gpstk::FFTextStream
	{
	public:
		RTKPOST_Pos_Stream();

		/** Constructor
		* Opens a file named \a fn using ios::openmode \a mode.
		*/
		RTKPOST_Pos_Stream(const char* fn, std::ios::openmode mode = std::ios::in);

		/// Destructor
		virtual ~RTKPOST_Pos_Stream();

		/// overrides open to reset the header
		virtual void open(const char* fn, std::ios::openmode mode);

		/// Header for this file.
		RTKPOST_Pos_Header header;

		/// Flag showing whether or not the header has been read.
		bool headerRead = false;
		//bool dataStartRead= false;
		bool dataStartWritten = false;

	private:
		void init();
	};

	//@}


}

#endif
