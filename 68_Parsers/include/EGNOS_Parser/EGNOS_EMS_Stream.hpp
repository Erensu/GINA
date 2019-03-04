#ifndef EGNOS_EMS_STREAM_HPP
#define EGNOS_EMS_STREAM_HPP

#include "FFTextStream.hpp"

namespace EGNOS_EMS_Parser {

	class EGNOS_EMS_Stream : public gpstk::FFTextStream
	{
	public:
		EGNOS_EMS_Stream();

		/** Constructor
		* Opens a file named \a fn using ios::openmode \a mode.
		*/
		EGNOS_EMS_Stream(const char* fn, std::ios::openmode mode = std::ios::in);

		/// Destructor
		virtual ~EGNOS_EMS_Stream();

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
