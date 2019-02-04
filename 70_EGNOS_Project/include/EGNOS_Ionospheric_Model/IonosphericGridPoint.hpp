#pragma once

#include <bitset>
#include <vector> 

#include "Position.hpp"

#include "IonosphericMaskBand.hpp"


namespace EGNOS {

#define NO_IODI_SET -1
#define INVALID_BAND_NUMBER -1
#define INVALID_BLOCK_NUMBER -1
	
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

		double getIonoCorr(void);
		int getIGPVerticalDelayEstimater(void) { return IGPVerticalDelayEstimate; };
		void setIonoDelayinMeter(double delay) { IonoCorrinMeter = delay; };
		void setIGPVerticalDelayEstimate(int delay) { IGPVerticalDelayEstimate = delay; IonoCorrinMeter = 8.0 * delay; };

		bool valid = false;
		double lat;
		double lon;

		int IODI;
		int GIVEI;

		int placeInBlock;
		int blockId;
		int bandNumber;

		friend std::ostream &operator<<(std::ostream &os, IonosphericGridPoint const &igp);
		
	private:
		double IonoCorrinMeter;
		int IGPVerticalDelayEstimate;
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
		std::vector<IGPMaskBlock> blocks;

		friend std::ostream &operator<<(std::ostream &os, IonosphericGridPointMasksMessageParser const &igpmp);

		void const updateIGP(IonosphericGridPoint &igp) const;

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

	class IonosphericDelayCorrectionsMessageParser
	{
	public:
		IonosphericDelayCorrectionsMessageParser();

		IonosphericDelayCorrectionsMessageParser& operator+=(const std::bitset<256> &message);
		IonosphericDelayCorrectionsMessageParser& operator+=(const std::bitset<250> &message);

		int currentRecievedIODI = NO_IODI_SET;
		int currentRecievedBandNumber = INVALID_BAND_NUMBER;
		int currentRecievedBlockNumber = INVALID_BLOCK_NUMBER;

		std::bitset<256> message;

		std::vector<IonosphericGridPoint> const & getIonosphericGridPoint(void) const;

		friend std::ostream &operator<<(std::ostream &os, IonosphericDelayCorrectionsMessageParser const &idcmp);

	private:
		void processMessage(void);
		void addIonosphericGridPoint2Vector(int offset);
		void addAllIGP2Vector(void);

		std::vector<IonosphericGridPoint> ionoPoints;

		void reset(void);

		int getIODI(void);
		int getBandNumber(void);
		int getBlockId(void);
		int getIGPVerticalDelay(int offset);
		int getGIVEI(int offset);

		bool checkMessageType(void);
		void addMessage(const std::bitset<256> &message);
		void addMessage(const std::bitset<250> &message);
	};
}