
#include "IonosphericMaskBand.hpp"


namespace EGNOS {

	const int IonosphericMaskBands::S75N85[28] = { -75, -65, -55, -50, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 65, 75, 85 };
	const int IonosphericMaskBands::S55N55[23] = { -55, -50, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55 };
	const int IonosphericMaskBands::S75N75[27] = { -75, -65, -55, -50, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 65, 75 };
	const int IonosphericMaskBands::S85N75[28] = { -85, -75, -65, -55, -50, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 65, 75 };
	const int IonosphericMaskBands::W180E175[72] = { -180,-175,-170,-165,-160,-155,-150,-145,-140,-135,-130,-125,-120,-115,-110,-105,-100,-95,-90,-85,-80,-75,-70,-65,-60,-55,-50,-45,-40,-35,-30,-25,-20,-15,-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175 };
	const int IonosphericMaskBands::W180E170[36] = { -180,-170,-160,-150,-140,-130,-120,-110,-100,-90,-80,-70,-60,-50,-40,-30,-20,-10,0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170 };
	const int IonosphericMaskBands::W180E150[12] = { -180,-150,-120,-90,-60,-30,0,30,60,90,120,150 };
	const int IonosphericMaskBands::W170E160[12] = { -170,-140,-110,-80,-50,-20,10,40,70,100,130,160 };

	const VerticalBandPart IonosphericMaskBands::vBands[72] = {
											// Band 0
											{ -180, IonosphericMaskBands::S75N85, 28, 1 },
											{ -175, IonosphericMaskBands::S55N55, 23, 29 },
											{ -170, IonosphericMaskBands::S75N75, 27, 52 },
											{ -165, IonosphericMaskBands::S55N55, 23, 79 },
											{ -160, IonosphericMaskBands::S75N75, 27, 102 },
											{ -155, IonosphericMaskBands::S55N55, 23, 129 },
											{ -150, IonosphericMaskBands::S75N75, 27, 152 },
											{ -145, IonosphericMaskBands::S55N55, 23, 179 },

											// Band 1
											{ -140, IonosphericMaskBands::S85N75, 28, 1 },
											{ -135, IonosphericMaskBands::S55N55, 23, 29 },
											{ -130, IonosphericMaskBands::S75N75, 27, 52 },
											{ -125, IonosphericMaskBands::S55N55, 23, 79 },
											{ -120, IonosphericMaskBands::S75N75, 27, 102 },
											{ -115, IonosphericMaskBands::S55N55, 23, 129 },
											{ -110, IonosphericMaskBands::S75N75, 27, 152 },
											{ -105, IonosphericMaskBands::S55N55, 23, 179 },

											// Band 2
											{ -100, IonosphericMaskBands::S75N75, 27, 1 },
											{ -95, IonosphericMaskBands::S55N55, 23, 28 },
											{ -90, IonosphericMaskBands::S75N85, 28, 51 },
											{ -85, IonosphericMaskBands::S55N55, 23, 79 },
											{ -80, IonosphericMaskBands::S75N75, 27, 102 },
											{ -75, IonosphericMaskBands::S55N55, 23, 129 },
											{ -70, IonosphericMaskBands::S75N75, 27, 152 },
											{ -65, IonosphericMaskBands::S55N55, 23, 179 },

											// Band 3
											{ -60, IonosphericMaskBands::S75N75, 28, 1 },
											{ -55, IonosphericMaskBands::S55N55, 23, 28 },
											{ -50, IonosphericMaskBands::S85N75, 28, 51 },
											{ -45, IonosphericMaskBands::S55N55, 23, 79 },
											{ -40, IonosphericMaskBands::S75N75, 27, 102 },
											{ -35, IonosphericMaskBands::S55N55, 23, 129 },
											{ -30, IonosphericMaskBands::S75N75, 27, 152 },
											{ -25, IonosphericMaskBands::S55N55, 23, 179 },

											// Band 4
											{ -20, IonosphericMaskBands::S75N75, 28, 1 },
											{ -15, IonosphericMaskBands::S55N55, 23, 28 },
											{ -10, IonosphericMaskBands::S75N75, 27, 51 },
											{ -5, IonosphericMaskBands::S55N55, 23, 78 },
											{ 0, IonosphericMaskBands::S75N85, 28, 101 },
											{ 5, IonosphericMaskBands::S55N55, 23, 129 },
											{ 10, IonosphericMaskBands::S75N75, 27, 152 },
											{ 15, IonosphericMaskBands::S55N55, 23, 179 },

											// Band 5
											{ 20, IonosphericMaskBands::S75N75, 27, 1 },
											{ 25, IonosphericMaskBands::S55N55, 23, 28 },
											{ 30, IonosphericMaskBands::S75N75, 27, 51 },
											{ 35, IonosphericMaskBands::S55N55, 23, 78 },
											{ 40, IonosphericMaskBands::S85N75, 28, 101 },
											{ 45, IonosphericMaskBands::S55N55, 23, 129 },
											{ 50, IonosphericMaskBands::S75N75, 27, 152 },
											{ 55, IonosphericMaskBands::S55N55, 23, 179 },

											// Band 6
											{ 60, IonosphericMaskBands::S75N75, 27, 1 },
											{ 65, IonosphericMaskBands::S55N55, 23, 28 },
											{ 70, IonosphericMaskBands::S75N75, 27, 51 },
											{ 75, IonosphericMaskBands::S55N55, 23, 78 },
											{ 80, IonosphericMaskBands::S75N75, 27, 101 },
											{ 85, IonosphericMaskBands::S55N55, 23, 128 },
											{ 90, IonosphericMaskBands::S75N85, 28, 151 },
											{ 95, IonosphericMaskBands::S55N55, 23, 179 },

											// Band 7
											{ 100, IonosphericMaskBands::S75N75, 27, 1 },
											{ 105, IonosphericMaskBands::S55N55, 23, 28 },
											{ 110, IonosphericMaskBands::S75N75, 27, 51 },
											{ 115, IonosphericMaskBands::S55N55, 23, 78 },
											{ 120, IonosphericMaskBands::S75N75, 27, 101 },
											{ 125, IonosphericMaskBands::S55N55, 23, 128 },
											{ 130, IonosphericMaskBands::S85N75, 28, 151 },
											{ 135, IonosphericMaskBands::S55N55, 23, 179 },

											// Band 8
											{ 140, IonosphericMaskBands::S75N75, 27, 1 },
											{ 145, IonosphericMaskBands::S55N55, 23, 28 },
											{ 150, IonosphericMaskBands::S75N75, 27, 51 },
											{ 155, IonosphericMaskBands::S55N55, 23, 78 },
											{ 160, IonosphericMaskBands::S75N75, 27, 101 },
											{ 165, IonosphericMaskBands::S55N55, 23, 128 },
											{ 170, IonosphericMaskBands::S75N75, 27, 151 },
											{ 175, IonosphericMaskBands::S55N55, 23, 178 },
										};

	const HorizontalBandPart IonosphericMaskBands::hBands[10] = {
												// Band 9
												{ 60, IonosphericMaskBands::W180E175, 72, 1 },
												{ 65, IonosphericMaskBands::W180E170, 36, 73 },
												{ 70, IonosphericMaskBands::W180E170, 36, 109 },
												{ 75, IonosphericMaskBands::W180E170, 36, 145 },
												{ 85, IonosphericMaskBands::W180E150, 12, 181 },

												// Band 10
												{ -60, IonosphericMaskBands::W180E175, 72, 1 },
												{ -65, IonosphericMaskBands::W180E170, 36, 73 },
												{ -70, IonosphericMaskBands::W180E170, 36, 109 },
												{ -75, IonosphericMaskBands::W180E170, 36, 145 },
												{ -85, IonosphericMaskBands::W170E160, 12, 181 },
											};

	bool IonosphericMaskBands::getPosition(unsigned char band, unsigned char bitpos, int &lat, int &lon) {
	
		if (bitpos == 0) {
		
			throw("Bit position cannot be 0. Minimum value is 1.");
			return false;
		}
		if ( 0 <= band && band <= 8) {
			for (size_t i = 0; i < 8; i++)
			{
				if ((this->vBands[8 * band + i].startBit <= bitpos) && (bitpos <= this->vBands[8 * band + i].startBit + this->vBands[8 * band + i].sizeOfArray - 1)){
					lon = this->vBands[8 * band + i].longitude;
					lat = this->vBands[8 * band + i].lattidudeArray[bitpos - this->vBands[8 * band + i].startBit];

					return true;
				}
				else{
					continue;
				}	
			}

			return false;
		}
		else{
			for (size_t i = 0; i < 5; i++)
			{
				if ((this->hBands[5 * (band-9) + i].startBit <= bitpos) && (bitpos <= this->hBands[5 * (band - 9) + i].startBit + this->hBands[5 * (band - 9) + i].sizeOfArray - 1)) {
					lat = this->hBands[5 * (band - 9) + i].lattitude;
					lon = this->hBands[5 * (band - 9) + i].longitudeArray[bitpos - this->hBands[5 * (band - 9) + i].startBit];

					return true;
				}
				else {
					continue;
				}
			}
			return false;
		}
	
	}
}