

namespace EGNOS {

	typedef struct {
		int longitude;
		const int* lattidudeArray;
		int sizeOfArray;
		int startBit;
	}VerticalBandPart;

	typedef struct {
		int lattitude;
		const int* longitudeArray;
		int sizeOfArray;
		int startBit;
	}HorizontalBandPart;

	class IonosphericMaskBands
	{
	public:

		static const int IonosphericMaskBands::S75N85[28];
		static const int IonosphericMaskBands::S55N55[23];
		static const int IonosphericMaskBands::S75N75[27];
		static const int IonosphericMaskBands::S85N75[28];
		static const int IonosphericMaskBands::W180E175[72];
		static const int IonosphericMaskBands::W180E170[36];
		static const int IonosphericMaskBands::W180E150[12];

		static const int IonosphericMaskBands::W170E160[12];
		static const VerticalBandPart vBands[72];
		static const HorizontalBandPart hBands[10];

		bool getPosition(unsigned char band, unsigned char bitpos, int &lat, int &lon);

	private:



	};
}