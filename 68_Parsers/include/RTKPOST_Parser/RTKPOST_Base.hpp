#pragma once

#include "FFData.hpp"

namespace RTKPOST_Parser {
		/// @ingroup FileHandling
		//@{

		/**
		* This class serves no purpose other than to make readable diagrams.
		*/
	class RTKPOST_Pos_Base : public gpstk::FFData
	{
	protected:
		/// Does the actual reading from the stream into this FFData object.
		virtual void reallyGetRecord(gpstk::FFStream& s)
			throw(std::exception, gpstk::StringUtils::StringException,
				gpstk::FFStreamError) = 0;

		/// Does the actual writing from the stream into this FFData object.
		virtual void reallyPutRecord(gpstk::FFStream& s) const
			throw(std::exception, gpstk::StringUtils::StringException,
				gpstk::FFStreamError) = 0;

	public:
		RTKPOST_Pos_Base() {}
		/// in deference to our coding standards, this is declared virtual
		virtual ~RTKPOST_Pos_Base() {}
	};
		//@}
}