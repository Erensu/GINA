#pragma once


#include "Position.hpp"
#include "IonosphericMaskBand.hpp"
#include <bitset>
#include <vector> 

namespace EGNOS {

//#define	EGNOS_IGPMESSAGEPARSER_OBJECT_LONG_DISPLAY
#define NO_IODI_SET -1
#define INVALID_BAND_NUMBER -1

	typedef struct {
		int idodi = NO_IODI_SET;
		int bandId = INVALID_BAND_NUMBER;
		unsigned char block[225] = {0};
	}IGPMaskBlock;

	class IonosphericGridPoint
	{
	public:

		IonosphericGridPoint(void);
		IonosphericGridPoint::IonosphericGridPoint(const IonosphericGridPoint &obj);

		IonosphericGridPoint& operator=(const IonosphericGridPoint& other);

		bool valid = false;
		int lat;
		int lon;

		int IGPVerticalDelayEstimate;
		unsigned char IODI;
		unsigned char GIVEI;

	private:
		void copy(const IonosphericGridPoint &obj);
	};
	

	class IonosphericGridPointMasksMessageParser
	{
	public:

		IonosphericGridPointMasksMessageParser(void);

		IonosphericGridPointMasksMessageParser& operator+=(const std::bitset<256> &message);
		IonosphericGridPointMasksMessageParser& operator+=(const std::bitset<250> &message);

		int currentRecievedBandNumber = INVALID_BAND_NUMBER;
		int numberOfBroadcastedBands = 0;
		int currentRecievedIODI = NO_IODI_SET;

		std::bitset<256> message;
		std::list<IGPMaskBlock> blocks;

		friend std::ostream &operator<<(std::ostream &os, IonosphericGridPointMasksMessageParser const &igpmp);

	private:

		void processMessage(void);

		void reset(void);

		int getIODI(void);
		int getNumberOfBroadcastedBands(void);
		int getBandNumber(void);
		void setIODIMAsk(void);

		bool doWeHaveHisIODIMask(void);

		bool checkMessageType(void);
		void addMessage(const std::bitset<256> &message);
		void addMessage(const std::bitset<250> &message);
	};

}