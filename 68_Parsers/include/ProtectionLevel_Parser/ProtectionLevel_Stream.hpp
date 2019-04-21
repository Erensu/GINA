#ifndef PROTECTION_LEVEL_STREAM_HPP
#define PROTECTION_LEVEL_STREAM_HPP

#include "FFTextStream.hpp"

namespace ProtectionLevel_Parser {

	class ProtectionLevel_Stream : public gpstk::FFTextStream
	{
	public:
		ProtectionLevel_Stream();

		/** Constructor
		* Opens a file named \a fn using ios::openmode \a mode.
		*/
		ProtectionLevel_Stream(const char* fn, std::ios::openmode mode = std::ios::in);

		/// Destructor
		virtual ~ProtectionLevel_Stream();

		/// overrides open to reset the header
		virtual void open(const char* fn, std::ios::openmode mode);

		/// Flag showing whether or not the header has been read.
		bool dataStartRead= false;
		bool dataStartWritten = false;

	private:
		void init();
	};

	//@}


}

#endif
