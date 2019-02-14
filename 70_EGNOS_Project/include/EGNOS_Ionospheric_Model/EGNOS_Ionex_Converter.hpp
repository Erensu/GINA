#pragma once

#include "IonexStream.hpp"
#include "IonexHeader.hpp"
#include "IonexData.hpp"
#include "IonexStore.hpp"

#include "IGPMap.hpp"

namespace EGNOS {

	class IGPMap2IonexData
	{
	public:

		IGPMap2IonexData();
		~IGPMap2IonexData();

		std::vector<gpstk::IonexData> convert(IGPMap &map, int mapID);
		std::vector<gpstk::IonexData> convert(IGPMap &map);

		gpstk::IonexStore convert(IGPMapStore &store);

	private:

		int IGPMap2IonexData::calcDim(int lat1, int lat2, double dlat);
	};

};