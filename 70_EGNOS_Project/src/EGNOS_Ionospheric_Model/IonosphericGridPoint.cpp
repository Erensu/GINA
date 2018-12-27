
#include "IonosphericGridPoint.hpp"


namespace EGNOS {

	IonosphericGridPoint::IonosphericGridPoint(void) {
	
	}

	IonosphericGridPoint::IonosphericGridPoint(const IonosphericGridPoint &obj)	{

		this->GIVEI = obj.GIVEI;
		this->IGPVerticalDelayEstimate = obj.IGPVerticalDelayEstimate;
		this->IODI = obj.IODI;
		this->lat = obj.lat;
		this->lon = obj.lon;
		this->valid = obj.valid;
	}

	void IonosphericGridPoint::copy(const IonosphericGridPoint &obj){

		this->GIVEI = obj.GIVEI;
		this->IGPVerticalDelayEstimate = obj.IGPVerticalDelayEstimate;
		this->IODI = obj.IODI;
		this->lat = obj.lat;
		this->lon = obj.lon;
		this->valid = obj.valid;
	}

	IonosphericGridPoint& IonosphericGridPoint::operator=(const IonosphericGridPoint& other) {
		
		this->copy(other);
		return *this;
	}

}
