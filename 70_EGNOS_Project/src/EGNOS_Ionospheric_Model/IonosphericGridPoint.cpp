
#include "IonosphericGridPoint.hpp"
#include "CivilTime.hpp"

namespace EGNOS {

#define IONO_GRID_MASK_MESSAGE_TYPE 18
#define IONO_DELAY_CORRECTION_MESSAGE_TYPE 26
#define IONO_GRID_MASK_SIZE 201
#define EGNOS_SATELLITE_MESSAGE_SIZE 250
#define EGNOS_EMS_MESSAGE_SIZE 256
#define INVALID_GIVEI_NUMBER 15
#define MAX_NUMBER_OF_BLOCKS 15
#define SIZE_OF_PREAMBLE 8

#define IGP_MASK_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID 6
#define IGP_MASK_MESSAGE_SIZE_OF_NUMBER_OF_BANDS 4
#define IGP_MASK_MESSAGE_SIZE_OF_BAND_NUMBER 4
#define IGP_MASK_MESSAGE_SIZE_OF_IODI 2
#define IGP_MASK_MESSAGE_STARTING_BIT_OF_MESSAGE_TYPE_ID SIZE_OF_PREAMBLE
#define IGP_MASK_MESSAGE_STARTING_BIT_OF_NUMBER_OF_BANDS IGP_MASK_MESSAGE_STARTING_BIT_OF_MESSAGE_TYPE_ID + IGP_MASK_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID
#define IGP_MASK_MESSAGE_STARTING_BIT_OF_BAND_NUMBER IGP_MASK_MESSAGE_STARTING_BIT_OF_NUMBER_OF_BANDS + IGP_MASK_MESSAGE_SIZE_OF_NUMBER_OF_BANDS
#define IGP_MASK_MESSAGE_STARTING_BIT_OF_IODI IGP_MASK_MESSAGE_STARTING_BIT_OF_BAND_NUMBER + IGP_MASK_MESSAGE_SIZE_OF_BAND_NUMBER
#define IGP_MASK_MESSAGE_STARTING_BIT_OF_IONOMASK IGP_MASK_MESSAGE_STARTING_BIT_OF_IODI + IGP_MASK_MESSAGE_SIZE_OF_IODI


#define IGP_DELAY_CORR_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID 6
#define IGP_DELAY_CORR_MESSAGE_SIZE_OF_BAND_NUMBER 4
#define IGP_DELAY_CORR_MESSAGE_SIZE_OF_BLOCK_ID 4
#define IGP_DELAY_CORR_MESSAGE_SIZE_OF_VERTICAL_DELAY 9
#define IGP_DELAY_CORR_MESSAGE_SIZE_OF_GIVEI 4

#define IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_MESSAGE_TYPE_ID SIZE_OF_PREAMBLE
#define IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_BAND_NUMBER IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_MESSAGE_TYPE_ID + IGP_DELAY_CORR_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID
#define IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_BLOCK_ID IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_BAND_NUMBER + IGP_DELAY_CORR_MESSAGE_SIZE_OF_BAND_NUMBER
#define IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_REPEATED_DATA IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_BLOCK_ID + IGP_DELAY_CORR_MESSAGE_SIZE_OF_BLOCK_ID

#define IGP_DELAY_CORR_MESSAGE_SIZE_OF_IODI 2
#define	IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_IODI  IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_REPEATED_DATA + 15 * (IGP_DELAY_CORR_MESSAGE_SIZE_OF_VERTICAL_DELAY + IGP_DELAY_CORR_MESSAGE_SIZE_OF_GIVEI)// 217

	const double IonosphericGridPoint::GIVEI_Meters[16] = { 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.6, 4.5, 6.0, 15.0, 45.0, 0 };
	const double IonosphericGridPoint::GIVEI_Variance[16] = { 0.0084, 0.0333, 0.0749, 0.1331, 0.2079, 0.2994, 0.4075, 0.5322, 0.6735, 0.8315, 1.1974, 1.8709, 3.326, 20.787, 187.0826, 0 };

	void IonosphericGridPoint::copy(const IonosphericGridPoint &obj){

		referenceTime = obj.referenceTime;  
		varianceCalcStatus = obj.varianceCalcStatus;

		this->GIVEI = obj.GIVEI;
		this->IGPVerticalDelayEstimate = obj.IGPVerticalDelayEstimate;
		this->IonoCorrinMeter = obj.IonoCorrinMeter;
		this->IODI = obj.IODI;
		this->lat = obj.lat;
		this->lon = obj.lon;
		this->valid = obj.valid;

		this->placeInBlock = obj.placeInBlock;
		this->blockId = obj.blockId;
		this->bandNumber = obj.bandNumber;
	}

	IonosphericGridPoint& IonosphericGridPoint::operator=(const IonosphericGridPoint& other) {
		
		this->copy(other);
		return *this;
	}

	void IonosphericGridPoint::setReferenceTime(gpstk::CommonTime time) {

		referenceTime = time; 
	};

	gpstk::CommonTime IonosphericGridPoint::getReferenceTime(void) {

		return referenceTime; 
	};

	double IonosphericGridPoint::getIonoCorr(void) {

		return IonoCorrinMeter;
	}

	double IonosphericGridPoint::getIonoCorrVariance(void) {

		double var = 0;

		if (GIVEI == INVALID_GIVEI_NUMBER) {
			throw std::domain_error("Grid is not monitored");
		}

		if (varianceCalcStatus == DEGRADATAION_MODEL_NOT_USED) {
			var = GIVEI_Variance[GIVEI];
		}
		else if (varianceCalcStatus = DEGRADATAION_MODEL_USED) {
			// TODO - implement the algorithm
		}
		else {
			var = GIVEI_Variance[GIVEI];
		}
		
		return var;
	}

	int IonosphericGridPoint:: getGIVEI(void) {
	
		return GIVEI;
	}

	void IonosphericGridPoint::setGIVEI(int New_GIVEI) {

		if (New_GIVEI < 0 || New_GIVEI >= INVALID_GIVEI_NUMBER) {

			this->valid = false;
		}

		IonoCorr_RMS = GIVEI_Meters[GIVEI];
		IonoCorr_Variance = GIVEI_Variance[GIVEI];
		GIVEI = New_GIVEI;
	}

	bool operator==(const IonosphericGridPoint& lhs, const IonosphericGridPoint& rhs) {

		return (	lhs.lat == rhs.lat && 
					lhs.lon == rhs.lon && 
					lhs.GIVEI == rhs.GIVEI &&  
					lhs.IonoCorrinMeter == rhs.IonoCorrinMeter && 
					lhs.referenceTime == rhs.referenceTime			);
	}

	std::ostream &operator<<(std::ostream &os, IonosphericGridPoint const &igp) {

		gpstk::CivilTime civtime(igp.referenceTime);

		os << "Time of Data: "  << civtime.year << " " << civtime.month << " "<< civtime.day << " " << civtime.hour << " " << civtime.minute << " " << civtime.second << std::endl;

		os << "IODI " << igp.IODI << std::endl;
		os << "bandNumber " << igp.bandNumber << std::endl;
		os << "blockId " << igp.blockId << std::endl;
		os << "placeInBlock " << igp.placeInBlock << std::endl;
		os << "IGPVerticalDelayEstimate " << igp.IGPVerticalDelayEstimate << std::endl;
		os << "GIVEI " << igp.GIVEI << std::endl;

		if (igp.valid) {
			os << "Lat " << igp.lat << std::endl;
			os << "Lon " << igp.lon << std::endl;
		}
		
		return os;
	}
#pragma region IonosphericGridPointMasksMessageParser

	IonosphericGridPointMasksMessageParser::IonosphericGridPointMasksMessageParser(void) {
	
	}

	void IonosphericGridPointMasksMessageParser::addMessage(const std::bitset<EGNOS_EMS_MESSAGE_SIZE> &message) {
	
		this->message = message;

		this->processMessage();
	}

	void IonosphericGridPointMasksMessageParser::addMessage(const std::bitset<EGNOS_SATELLITE_MESSAGE_SIZE> &message) {

		this->message.reset();

		for (size_t i = 0; i < EGNOS_SATELLITE_MESSAGE_SIZE; i++){
			this->message[i] = message[i];
		}
		
		this->processMessage();
	}

	void IonosphericGridPointMasksMessageParser::processMessage(void) {

		if (this->checkMessageType() == false) {
			this->message.reset();
			std::cerr << "Wrong message type. It should be type 18." << std::endl;
			throw("Wrong message type. It should be type 18.");
		}

		int newIodi = this->getIODI();

		if (this->currentRecievedIODI != newIodi && this->currentRecievedIODI != NO_IODI_SET) {
			this->reset();
		}
		this->currentRecievedIODI = newIodi;

		this->currentRecievedBandNumber = this->getBandNumber();
		this->numberOfBroadcastedBands = this->getNumberOfBroadcastedBands();

		this->setIODIMAsk();
	}

	IonosphericGridPointMasksMessageParser& IonosphericGridPointMasksMessageParser::operator+=(const std::bitset<EGNOS_EMS_MESSAGE_SIZE> &message) {
	
		this->addMessage(message);
		return *this;
	}

	IonosphericGridPointMasksMessageParser& IonosphericGridPointMasksMessageParser::operator+=(const std::bitset<EGNOS_SATELLITE_MESSAGE_SIZE> &message) {

		this->addMessage(message);
		return *this;
	}

	void IonosphericGridPointMasksMessageParser::setIODIMAsk(void) {
		
		IGPMaskBlock temp;

		if (this->doWeHaveHisIODIMask()==true) {
			return;
		}

		temp.bandId = this->currentRecievedBandNumber;
		temp.idodi = this->currentRecievedIODI;

		int a = 0;

		for (size_t i = 0; i < IONO_GRID_MASK_SIZE; i++) {

			if (this->message[IGP_MASK_MESSAGE_STARTING_BIT_OF_IONOMASK + i] > 0) {
				temp.block[a] = i + 1;
				a++;
			}
		}

		this->blocks.push_back(temp);
	}

	bool IonosphericGridPointMasksMessageParser::doWeHaveHisIODIMask(void){
	
		for (std::vector<IGPMaskBlock>::iterator it = this->blocks.begin(); it != this->blocks.end(); ++it) {

			if (it->bandId == this->currentRecievedBandNumber) {
				if (it->idodi == this->currentRecievedIODI) {
				
					// Double check. Let's see if the IGP masks are really the same
					// First. Create the new blocks from the new mask
					IGPMaskBlock temp;
					int a = 0;
					for (size_t i = 0; i < IONO_GRID_MASK_SIZE; i++) {

						if (this->message[IGP_MASK_MESSAGE_STARTING_BIT_OF_IONOMASK + i] > 0) {
							temp.block[a] = i;
							a++;
						}
					}
					// Second. Compare the old and new blocks
					for (size_t i = 0; i < IONO_GRID_MASK_SIZE; i++)
					{
						if (it->block[i] != temp.block[i]) {
							it = this->blocks.erase(it);
							return false;
						}
					}
					
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
		std::bitset<IGP_MASK_MESSAGE_SIZE_OF_IODI> iodeBits;
	
		for (size_t i = 0; i < IGP_MASK_MESSAGE_SIZE_OF_IODI; i++) {
			iodeBits[IGP_MASK_MESSAGE_SIZE_OF_IODI - 1 - i] = this->message[IGP_MASK_MESSAGE_STARTING_BIT_OF_IODI + i];
		}

		return iodeBits.to_ulong();
	}

	int IonosphericGridPointMasksMessageParser::getBandNumber(void) {
	
		std::bitset<IGP_MASK_MESSAGE_SIZE_OF_BAND_NUMBER> Bits;

		for (size_t i = 0; i < IGP_MASK_MESSAGE_SIZE_OF_BAND_NUMBER; i++) {
			Bits[IGP_MASK_MESSAGE_SIZE_OF_BAND_NUMBER -1 - i] = this->message[IGP_MASK_MESSAGE_STARTING_BIT_OF_BAND_NUMBER + i];
		}
		
		return Bits.to_ulong();
	}

	int IonosphericGridPointMasksMessageParser::getNumberOfBroadcastedBands(void) {
		std::bitset<IGP_MASK_MESSAGE_SIZE_OF_NUMBER_OF_BANDS> Bits;

		for (size_t i = 0; i < IGP_MASK_MESSAGE_SIZE_OF_NUMBER_OF_BANDS; i++) {
			Bits[IGP_MASK_MESSAGE_SIZE_OF_NUMBER_OF_BANDS -1 -i] = this->message[IGP_MASK_MESSAGE_STARTING_BIT_OF_NUMBER_OF_BANDS + i];
		}

		return Bits.to_ulong();
	}

	bool IonosphericGridPointMasksMessageParser::checkMessageType() {

		std::bitset<IGP_MASK_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID> typeBits;

		for (size_t i = 0; i < IGP_MASK_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID; i++) {
			typeBits[IGP_MASK_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID - 1 -i] = this->message[IGP_MASK_MESSAGE_STARTING_BIT_OF_MESSAGE_TYPE_ID + i];
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

	void const IonosphericGridPointMasksMessageParser::updateIGP(IonosphericGridPoint & const igp) const{

		if (igp.getGIVEI() == INVALID_GIVEI_NUMBER) {
			igp.valid = false;
			return;
		}

		int bitpos = 0;
		for (std::vector<IGPMaskBlock>::const_iterator it = this->blocks.begin(); it != this->blocks.end(); ++it) {
			if (it->bandId == igp.bandNumber && it->idodi == igp.IODI) {
				bitpos = it->block[igp.blockId * MAX_NUMBER_OF_BLOCKS + igp.placeInBlock];
				break;
			}
			
		}
		if (bitpos == 0) {
			igp.valid = false;
			return;
		}

		IonosphericMaskBands gridEngine;
		int temp_lat, temp_lon;
		gridEngine.getPosition(igp.bandNumber, bitpos, temp_lat, temp_lon);
		igp.lat = (double)temp_lat;
		igp.lon = (double)temp_lon;

		igp.valid = true;
	}

	std::ostream &operator<<(std::ostream &os, IonosphericGridPointMasksMessageParser const &igpmp) {
		os << "numberOfBroadcastedBands " << igpmp.numberOfBroadcastedBands << std::endl;
		os << "currentRecievedIODI " << igpmp.currentRecievedIODI << std::endl;
		os << "currentRecievedBandNumber " << igpmp.currentRecievedBandNumber << std::endl;
		os << "stored number of blocks " << igpmp.blocks.size() << std::endl;

		for (std::vector<IGPMaskBlock>::const_iterator it = igpmp.blocks.begin(); it != igpmp.blocks.end(); ++it) {
			os << "Band Id and IODI: " << it->bandId <<  " " <<  it->idodi << std::endl;

			#ifdef EGNOS_IGPMESSAGEPARSER_OBJECT_LONG_DISPLAY
				for (size_t i = 0; i < IONO_GRID_MASK_SIZE; i++)
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

#pragma endregion

#pragma region IonosphericDelayCorrectionsMessageParser

	bool IonosphericDelayCorrectionsMessageParser::checkMessageType(void) {
		std::bitset<IGP_DELAY_CORR_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID> typeBits;

		for (size_t i = 0; i < IGP_DELAY_CORR_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID; i++) {
			typeBits[IGP_DELAY_CORR_MESSAGE_SIZE_OF_MESSAGE_TYPE_ID  -1 - i] = this->message[IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_MESSAGE_TYPE_ID + i];
		}

		return typeBits.to_ulong() == IONO_DELAY_CORRECTION_MESSAGE_TYPE;
	}

	void IonosphericDelayCorrectionsMessageParser::addMessage(const std::bitset<EGNOS_EMS_MESSAGE_SIZE> &message) {

		this->message = message;

		this->processMessage();
	}

	void IonosphericDelayCorrectionsMessageParser::addMessage(const std::bitset<EGNOS_SATELLITE_MESSAGE_SIZE> &message) {

		this->message.reset();

		for (size_t i = 0; i < EGNOS_SATELLITE_MESSAGE_SIZE; i++) {
			this->message[i] = message[i];
		}

		this->processMessage();
	}

	int IonosphericDelayCorrectionsMessageParser::getBandNumber(void) {

		std::bitset<IGP_DELAY_CORR_MESSAGE_SIZE_OF_BAND_NUMBER> Bits;

		for (size_t i = 0; i < IGP_DELAY_CORR_MESSAGE_SIZE_OF_BAND_NUMBER; i++) {
			Bits[IGP_DELAY_CORR_MESSAGE_SIZE_OF_BAND_NUMBER - 1 -i] = this->message[IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_BAND_NUMBER + i];
		}

		return Bits.to_ulong();
	}

	int IonosphericDelayCorrectionsMessageParser::getIODI(void) {
		std::bitset<IGP_DELAY_CORR_MESSAGE_SIZE_OF_IODI> iodeBits;

		for (size_t i = 0; i < IGP_DELAY_CORR_MESSAGE_SIZE_OF_IODI; i++) {
			iodeBits[IGP_DELAY_CORR_MESSAGE_SIZE_OF_IODI - 1 -i] = this->message[IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_IODI + i];
		}

		return iodeBits.to_ulong();
	
	}

	void IonosphericDelayCorrectionsMessageParser::reset(void) {

		message.reset();
		ionoPoints.clear();

		currentRecievedBandNumber = INVALID_BAND_NUMBER;
		currentRecievedBlockNumber = INVALID_BLOCK_NUMBER;
		currentRecievedIODI = NO_IODI_SET;
	}

	const std::vector<IonosphericGridPoint> const & IonosphericDelayCorrectionsMessageParser::getIonosphericGridPoint(void) const {
	
		return ionoPoints;
	}

	void IonosphericDelayCorrectionsMessageParser::processMessage(void) {
	
		if (this->checkMessageType() == false) {
			std::cerr << "Wrong message type. It should be type 26." << std::endl;
			throw("Wrong message type. It should be type 26.");
		}

		this->currentRecievedIODI = this->getIODI();
		this->currentRecievedBandNumber = this->getBandNumber();
		this->currentRecievedBlockNumber = this->getBlockId();

		this->addAllIGP2Vector();
	}

	IonosphericDelayCorrectionsMessageParser& IonosphericDelayCorrectionsMessageParser::operator+=(const std::bitset<EGNOS_EMS_MESSAGE_SIZE> &message) {

		this->reset();
		this->addMessage(message);
		return *this;
	}

	IonosphericDelayCorrectionsMessageParser& IonosphericDelayCorrectionsMessageParser::operator+=(const std::bitset<EGNOS_SATELLITE_MESSAGE_SIZE> &message) {

		this->reset();
		this->addMessage(message);
		return *this;
	}

	int IonosphericDelayCorrectionsMessageParser::getBlockId(void) {

		std::bitset<IGP_DELAY_CORR_MESSAGE_SIZE_OF_BLOCK_ID> Bits;
		for (size_t i = 0; i < IGP_DELAY_CORR_MESSAGE_SIZE_OF_BLOCK_ID; i++) {
			Bits[IGP_DELAY_CORR_MESSAGE_SIZE_OF_BLOCK_ID - 1 - i] = this->message[IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_BLOCK_ID + i];
		}

		return Bits.to_ulong();
	}

	int IonosphericDelayCorrectionsMessageParser::getIGPVerticalDelay(int offset) {
	
		std::bitset<IGP_DELAY_CORR_MESSAGE_SIZE_OF_VERTICAL_DELAY> Bits;
		int offsetinBits = offset * (IGP_DELAY_CORR_MESSAGE_SIZE_OF_VERTICAL_DELAY + IGP_DELAY_CORR_MESSAGE_SIZE_OF_GIVEI);
		for (size_t i = 0; i < IGP_DELAY_CORR_MESSAGE_SIZE_OF_VERTICAL_DELAY; i++) {
			Bits[IGP_DELAY_CORR_MESSAGE_SIZE_OF_VERTICAL_DELAY - 1 -i] = this->message[offsetinBits + IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_REPEATED_DATA + i];
		}

		return Bits.to_ulong();
	}

	int IonosphericDelayCorrectionsMessageParser::getGIVEI(int offset) {
	
		std::bitset<IGP_DELAY_CORR_MESSAGE_SIZE_OF_GIVEI> Bits;
		int offsetinBits = offset * (IGP_DELAY_CORR_MESSAGE_SIZE_OF_VERTICAL_DELAY + IGP_DELAY_CORR_MESSAGE_SIZE_OF_GIVEI);
		for (size_t i = 0; i < IGP_DELAY_CORR_MESSAGE_SIZE_OF_GIVEI; i++) {
			Bits[IGP_DELAY_CORR_MESSAGE_SIZE_OF_GIVEI - 1 - i] = this->message[offsetinBits + IGP_DELAY_CORR_MESSAGE_STARTING_BIT_OF_REPEATED_DATA + IGP_DELAY_CORR_MESSAGE_SIZE_OF_VERTICAL_DELAY + i];
		}

		return Bits.to_ulong();
	}

	void IonosphericDelayCorrectionsMessageParser::addIonosphericGridPoint2Vector(int offset) {
	
		IonosphericGridPoint temp;

		temp.IODI = currentRecievedIODI;
		temp.bandNumber = currentRecievedBandNumber;
		temp.blockId = currentRecievedBlockNumber;
		temp.placeInBlock = offset;

		temp.setGIVEI(this->getGIVEI(offset));
		temp.setIGPVerticalDelayEstimate(this->getIGPVerticalDelay(offset));

		ionoPoints.push_back(temp);
	}

	void IonosphericDelayCorrectionsMessageParser::addAllIGP2Vector(void) {

		for (size_t offset = 0; offset < MAX_NUMBER_OF_BLOCKS; offset++)	{
			this->addIonosphericGridPoint2Vector(offset);
		}
	}

	std::ostream &operator<<(std::ostream &os, IonosphericDelayCorrectionsMessageParser const &idcmp) {
		os << "currentRecievedBandNumber " << idcmp.currentRecievedBandNumber << std::endl;
		os << "currentRecievedIODI " << idcmp.currentRecievedIODI << std::endl;
		os << "currentRecievedBlockNumber " << idcmp.currentRecievedBlockNumber << std::endl;
		
		for (std::vector<IonosphericGridPoint>::const_iterator it = idcmp.ionoPoints.begin(); it != idcmp.ionoPoints.end(); ++it) {		

			os << *it;
		}

		return os;
	}
#pragma endregion
}
