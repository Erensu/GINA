#pragma once


#include "Position.hpp"
#include "IonosphericMaskBand.hpp"

namespace EGNOS {

	
	class IonosphericGridPoint
	{
	public:

		IonosphericGridPoint(void);
		IonosphericGridPoint::IonosphericGridPoint(const IonosphericGridPoint &obj);

		void copy(const IonosphericGridPoint &obj);
		IonosphericGridPoint& operator=(const IonosphericGridPoint& other);

		bool valid = false;
		int lat;
		int lon;

		int IGPVerticalDelayEstimate;
		unsigned char IODI;
		unsigned char GIVEI;

	private:
		
	};
	
	
}