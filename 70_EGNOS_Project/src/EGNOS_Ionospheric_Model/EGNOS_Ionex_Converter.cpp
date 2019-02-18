#include "EGNOS_Ionex_Converter.hpp"


namespace EGNOS {
#pragma region IGPMap2IonexData
	IGPMap2IonexData::IGPMap2IonexData()
	{
	}

	IGPMap2IonexData::~IGPMap2IonexData()
	{
	}

	std::vector<gpstk::IonexData> IGPMap2IonexData::convert(IGPMap &map) {
		return this->convert(map, 0);
	}

	std::vector<gpstk::IonexData> IGPMap2IonexData::convert(IGPMap &map, int mapID) {
	
		std::vector<gpstk::IonexData> rtv;
		gpstk::IonexData ionexDataTEC;
		gpstk::IonexData ionexDataRMS;

		std::vector<gpstk::CommonTime> epochs = map.getEpochTimes();
		gpstk::CommonTime CurrentTime = epochs[0];

		double lat1, lat2, dlat;
		double lon1, lon2, dlon;
		double hgt1, hgt2, dhgt;
		int dimlat, dimlon, dimhgt;

		lat1 = 85;
		lat2 = 10;
		dlat = -5;

		lon1 = -60;
		lon2 = 60;
		dlon = 5;

		hgt1 = 350;
		hgt2 = 350;
		dhgt = 0;

		dimlat = calcDim(lat1, lat2, dlat);
		dimlon = calcDim(lon1, lon2, dlon);
		dimhgt = calcDim(hgt1, hgt2, dhgt);

		double numberOfValues = dimlat * dimlon;

		gpstk::Vector<double> valuesTEC(numberOfValues);
		gpstk::Vector<double> valuesRMS(numberOfValues);
		int counter = 0;

		double currLat = lat1;
		double currLon = lon1;

		double TEC; 
		double RMS;

		while (counter < numberOfValues)
		{
			try
			{
				TEC = map.getTEC(CurrentTime, currLat, currLon);
				RMS = map.getRMS(CurrentTime, currLat, currLon);
			}
			catch (const std::exception& e)
			{
				TEC = 999.9;
				RMS = 999.9;
			}
			

			valuesTEC(counter) = TEC;
			valuesRMS(counter) = RMS;

			if (abs(currLon - lon2) < dlon) {
				currLon = lon1 - dlon;
				currLat += dlat;
			}
			currLon += dlon;
			counter++;
		}

		ionexDataTEC.data = valuesTEC;
		ionexDataTEC.mapID = mapID;

		ionexDataRMS.data = valuesRMS;
		ionexDataRMS.mapID = mapID;

		ionexDataTEC.dim[0] = dimlat;
		ionexDataTEC.dim[1] = dimlon;
		ionexDataTEC.dim[2] = dimhgt;

		ionexDataRMS.dim[0] = dimlat;
		ionexDataRMS.dim[1] = dimlon;
		ionexDataRMS.dim[2] = dimhgt;

		ionexDataTEC.exponent = 0;
		ionexDataTEC.lat[0] = lat1;
		ionexDataTEC.lat[1] = lat2;
		ionexDataTEC.lat[2] = dlat;

		ionexDataRMS.exponent = 0;
		ionexDataRMS.lat[0] = lat1;
		ionexDataRMS.lat[1] = lat2;
		ionexDataRMS.lat[2] = dlat;

		ionexDataTEC.lon[0] = lon1;
		ionexDataTEC.lon[1] = lon2;
		ionexDataTEC.lon[2] = dlon;

		ionexDataRMS.lon[0] = lon1;
		ionexDataRMS.lon[1] = lon2;
		ionexDataRMS.lon[2] = dlon;

		ionexDataTEC.hgt[0] = hgt1;
		ionexDataTEC.hgt[1] = hgt2;
		ionexDataTEC.hgt[2] = dhgt;

		ionexDataRMS.hgt[0] = hgt1;
		ionexDataRMS.hgt[1] = hgt2;
		ionexDataRMS.hgt[2] = dhgt;

		ionexDataTEC.valid = true;
		ionexDataRMS.valid = true;

		ionexDataTEC.type.type = "TEC";
		ionexDataRMS.type.type = "RMS";
		
		ionexDataTEC.type.units = "10e0 [TEC]";
		ionexDataRMS.type.units = "10e0 [TEC]";
		
		ionexDataTEC.type.description = "Total Electron Content map";
		ionexDataRMS.type.description = "Total Electron Content map";

		ionexDataTEC.time = CurrentTime;
		ionexDataRMS.time = CurrentTime;

		rtv.push_back(ionexDataTEC);
		rtv.push_back(ionexDataRMS);

		return rtv;
	}

	gpstk::IonexStore IGPMap2IonexData::convert(IGPMapStore &store) {
	
		gpstk::IonexStore ionexStore;
		std::vector<gpstk::CommonTime> epochs = store.getEpochTimes();

		int mapIndex = 0;
		IGPMap igpMap;
		std::vector<gpstk::IonexData> ionexDataVector;
		for (size_t epochIndex = 0; epochIndex < epochs.size(); epochIndex++)
		{
			try
			{
				igpMap = store.getIGPMap(epochs[epochIndex]);
				ionexDataVector = this->convert(igpMap, mapIndex);

				for (size_t iodataIndex = 0; iodataIndex < ionexDataVector.size(); iodataIndex++)	{
					ionexStore.addMap(ionexDataVector[iodataIndex]);
				}
				mapIndex++;
			}
			catch (const std::exception&){	}

		}
		
		
		
		return ionexStore;
	}

	int IGPMap2IonexData::calcDim(int lat1, int lat2, double dlat) {

		if (dlat == 0) {
			return 1;
		}
		else {
			return int(abs((int(lat2) - int(lat1)) / dlat) + 1);
		}
	}
#pragma endregion

};