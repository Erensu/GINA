#pragma once


#include "Position.hpp"
#include "IonosphericMaskBand.hpp"

namespace EGNOS {

	
	class IonosphericGridPoint
	{
	public:

		gpstk::Position gridPos;

		int IGPVerticalDelayEstimate;
		unsigned char IODI;
		unsigned char GIVEI;

	private:
		
	};
	
	
}