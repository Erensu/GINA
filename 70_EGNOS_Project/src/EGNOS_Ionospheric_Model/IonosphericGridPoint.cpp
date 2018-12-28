
#include "IonosphericGridPoint.hpp"


#define IONO_GRID_MASK_MESSAGE_TYPE 18


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


	IonosphericGridPointMasksMessageParser::IonosphericGridPointMasksMessageParser(void) {
	
	}

	void IonosphericGridPointMasksMessageParser::addMessage(const std::bitset<256> &message) {
	
		this->message = message;

		this->processMessage();
	}

	void IonosphericGridPointMasksMessageParser::addMessage(const std::bitset<250> &message) {

		this->message.reset();

		for (size_t i = 0; i < 250; i++){
			this->message[i] = message[i];
		}
		
		this->processMessage();
	}

	void IonosphericGridPointMasksMessageParser::processMessage(void) {

		if (this->checkMessageType() == false) {
			this->message.reset();
			throw("Wrong message type. It should be type 18.");
		}

		int iodi = this->getIODI();

		if (this->currentRecievedIODI != iodi && this->currentRecievedIODI != NO_IODI_SET) {
			this->reset();
		}
		this->currentRecievedIODI = iodi;

		this->currentRecievedBandNumber = this->getBandNumber();
		this->numberOfBroadcastedBands = this->getNumberOfBroadcastedBands();

		this->setIODIMAsk();

	}

	IonosphericGridPointMasksMessageParser& IonosphericGridPointMasksMessageParser::operator+=(const std::bitset<256> &message) {
	
		this->addMessage(message);
		return *this;
	}

	IonosphericGridPointMasksMessageParser& IonosphericGridPointMasksMessageParser::operator+=(const std::bitset<250> &message) {

		this->addMessage(message);
		return *this;
	}

	void IonosphericGridPointMasksMessageParser::setIODIMAsk(void) {
		std::bitset<201> iodeBits;

		IGPMaskBlock temp;

		if (this->doWeHaveHisIODIMask()==true) {
			return;
		}

		temp.bandId = this->currentRecievedBandNumber;
		temp.idodi = this->currentRecievedIODI;

		int a = 0;

		for (size_t i = 0; i < 201; i++) {
			iodeBits[i] = this->message[24 + i];
			

			if (this->message[24 + i] > 0) {
				temp.block[a] = i;
				a++;
			}
		}

		this->blocks.push_back(temp);
	}

	bool IonosphericGridPointMasksMessageParser::doWeHaveHisIODIMask(void){
	
		for (std::list<IGPMaskBlock>::iterator it = this->blocks.begin(); it != this->blocks.end(); ++it) {
			/* std::cout << *it; ... */
			if (it->bandId == this->currentRecievedBandNumber) {
				if (it->idodi == this->currentRecievedIODI) {
					return true;
				}
				else {
					it = this->blocks.erase(it);
					return false;
				}
			}
		}

		return false;
	}

	int IonosphericGridPointMasksMessageParser::getIODI(void) {
		std::bitset<2> iodeBits;
	
		for (size_t i = 0; i < 2; i++) {
			iodeBits[i] = this->message[22 + i];
		}

		return iodeBits.to_ulong();
	}

	int IonosphericGridPointMasksMessageParser::getBandNumber(void) {
	
		std::bitset<4> Bits;

		for (size_t i = 0; i < 4; i++) {
			Bits[i] = this->message[18 + i];
		}

		return Bits.to_ulong();
	}

	int IonosphericGridPointMasksMessageParser::getNumberOfBroadcastedBands(void) {
		std::bitset<4> Bits;

		for (size_t i = 0; i < 4; i++) {
			Bits[i] = this->message[14 + i];
		}

		return Bits.to_ulong();
	}

	bool IonosphericGridPointMasksMessageParser::checkMessageType() {

		std::bitset<6> typeBits;

		for (size_t i = 0; i < 6; i++) {
			typeBits[i] = this->message[8 + i];
		}

		return typeBits.to_ulong() == IONO_GRID_MASK_MESSAGE_TYPE;
	}

	void IonosphericGridPointMasksMessageParser::reset(void) {
		this->blocks.clear();
		message.reset();

		currentRecievedBandNumber = INVALID_BAND_NUMBER;
		numberOfBroadcastedBands = 0;
		currentRecievedIODI = NO_IODI_SET;
	}

	std::ostream &operator<<(std::ostream &os, IonosphericGridPointMasksMessageParser const &igpmp) {
		os << "numberOfBroadcastedBands " << igpmp.numberOfBroadcastedBands << std::endl;
		os << "currentRecievedIODI " << igpmp.currentRecievedIODI << std::endl;
		os << "currentRecievedBandNumber " << igpmp.currentRecievedBandNumber << std::endl;
		os << "stored number of blocks " << igpmp.blocks.size() << std::endl;

		for (std::list<IGPMaskBlock>::const_iterator it = igpmp.blocks.begin(); it != igpmp.blocks.end(); ++it) {
			os << "Band Id and IODI: " << it->bandId <<  " " <<  it->idodi << std::endl;
#ifdef EGNOS_IGPMESSAGEPARSER_OBJECT_LONG_DISPLAY
			for (size_t i = 0; i < 201; i++)
			{
				if (int(it->block[i]) > 0) {
					os << " " << int(it->block[i]);
				}
			}
			os << std::endl;
#endif

		}
		return os;
	}

}
