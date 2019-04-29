#include "Runnables.hpp"

#include "Position.hpp"

namespace EGNOS 
{
	namespace EGNOS_RUNNABLE
	{
	#define TEC_IN_METER 0.162372
		
		void runRTKPOSTParser(std::string &EDAS_FileNamewPath_in, std::string &EDAS_FileNamewPath_out, std::string &EDAS_FileNamewPath_out_out) {
		
			RTKPOST_Parser::RTKPOST_Pos_Stream strm_in;
			RTKPOST_Parser::RTKPOST_Pos_Stream strm_out;

			strm_in.open(EDAS_FileNamewPath_in.c_str(), std::ios::in);
			strm_out.open(EDAS_FileNamewPath_out.c_str(), std::ios::out);

			RTKPOST_Parser::RTKPOST_Pos_Header header;
			RTKPOST_Parser::RTKPOST_Pos_Data data;

			strm_in >> header;
			strm_out << header;

			while (strm_in >> data) {
				strm_out << data;
			}


			strm_in.close();
			strm_out.close();

			strm_in.open(EDAS_FileNamewPath_out.c_str(), std::ios::in);
			strm_out.open(EDAS_FileNamewPath_out_out.c_str(), std::ios::out);

			strm_in >> header;
			strm_out << header;

			strm_in.close();
			strm_out.close();
		}

		void runEGNOSIonoCorrectionModel(std::string EDAS_FileNamewPath) {
		
			EGNOSIonoCorrectionModel egnosIono;
			egnosIono.load(EDAS_FileNamewPath);

			gpstk::CommonTime firstEpoch = egnosIono.getFirstEpoch();
			gpstk::CommonTime lastEpoch = egnosIono.getLastEpoch();
			double timeIntervall = lastEpoch - firstEpoch;

			gpstk::Position RX(45.0,0.0,0.0, gpstk::Position::CoordinateSystem::Geocentric);

			gpstk::CommonTime epoch = firstEpoch + 6 * timeIntervall/10;
			double elevation = 0;
			double elevation_base = 5;
			double azimuth = 0;

			IonCorrandVar ic;
			for (size_t i = 0; i < 18; i++)
			{
				elevation = elevation_base + 5 * i;
				for (size_t i = 0; i < 36; i++)
				{
					azimuth = 10 * i;
					try
					{
						cout << "Azimuth: " << azimuth << std::endl;
						cout << "Elevation: " << elevation << std::endl;

						ic = egnosIono.getCorrection(epoch, RX, elevation, azimuth);

						cout << "Iono Correction in meter: " << ic.CorrinMeter << endl;
						cout << "Iono Variance in meter square: " << ic.Variance << endl;
						cout << endl;
					}
					catch (const std::exception& e)
					{
						std::cout << e.what() << endl;
						cout << endl;
					}
				}
			}
			
		}

		void compareIonexFiles(	std::string& ReferenceIonexFileNamewPath,
								std::string& TargetIonexFileNamewPath,
								std::string& IonexFileNamewPath_Out,
								double matchingIntervall) {

			gpstk::IonexStore ionoStoreRef, ionoStoreTarget;
			gpstk::IonexHeader ionoHeader1, ionoHeader2, ionoHeader_Out;

			try
			{
				ionoStoreRef.loadFile(ReferenceIonexFileNamewPath);
				std::cout << "IONEX files is loaded: "<< ReferenceIonexFileNamewPath << std::endl;
			}
			catch (const gpstk::FileMissingException &e)
			{
				throw domain_error(e.getText());
			}
			
			ionoHeader1 = ionoStoreRef.getHeader(ReferenceIonexFileNamewPath);

			try
			{
				ionoStoreTarget.loadFile(TargetIonexFileNamewPath);
				std::cout << "IONEX files is loaded: " << TargetIonexFileNamewPath << std::endl;
			}
			catch (const gpstk::FileMissingException &e)
			{
				std::cout << std::endl << "ERROR"<< endl;
				std::cout<< e.getText() << std::endl;
				return;
			}

			ionoHeader2 = ionoStoreTarget.getHeader(TargetIonexFileNamewPath);

			gpstk::IonexStream outIonexStoreStream;
			outIonexStoreStream.open(IonexFileNamewPath_Out.c_str(), std::ios::out);

			std::vector<std::string> names = ionoStoreTarget.getFileNames();
			ionoHeader_Out = ionoStoreTarget.getHeader(names[0]);
			ionoHeader_Out.commentList.push_back("MLH stands for Maximum Likelihood. Unitless.");
			ionoHeader_Out.commentList.push_back("MLH values in 0.0001. Max value is 9998.");
			ionoHeader_Out.commentList.push_back("Values greater than 0.9999 are downconverted to 0.9998");
			ionoHeader_Out.commentList.push_back("CH2 stands for Chi2 probability. Unitless.");
			ionoHeader_Out.commentList.push_back("CH2 values in 0.0001. Max value is 9998.");
			ionoHeader_Out.commentList.push_back("Values greater than 0.9999 are downconverted to 0.9998");
			outIonexStoreStream << ionoHeader_Out;

			gpstk::IonexStore::IonexMap::iterator it_ref;
			gpstk::IonexStore::IonexMap::iterator it_target;
			gpstk::IonexStore::IonexMap::iterator it_target_iterator;
			gpstk::CommonTime epoch_ref, epoch_target;

			gpstk::IonexHeader targetHeader = ionoStoreTarget.getHeader(names[0]);

			// Narrow difference ionex's region to Europe
			targetHeader.lat[0] = 90;
			targetHeader.lat[1] = 10;
			targetHeader.lat[2] = -abs(5);

			targetHeader.lon[0] = -60;
			targetHeader.lon[1] = 60;
			targetHeader.lon[2] = abs(5);

			int mapID = 0;

			std::vector<gpstk::IonexData> ionexDataVector;

			for (it_ref = ionoStoreRef.inxMaps.begin(); it_ref != ionoStoreRef.inxMaps.end(); it_ref++)	{

				epoch_ref = it_ref->first;

				it_target = ionoStoreTarget.inxMaps.find(epoch_ref);
				if (it_target == ionoStoreTarget.inxMaps.end()) {
					
					double timeDifference = matchingIntervall + 1;
					double temp_timeDifference = 0;

					bool atLeastOneMatchingEpochFound = false;

					for (it_target_iterator = ionoStoreTarget.inxMaps.begin(); it_target_iterator != ionoStoreTarget.inxMaps.end(); it_target_iterator++) {

						temp_timeDifference = abs(it_target_iterator->first - it_ref->first);
						if (temp_timeDifference <= matchingIntervall) {

							if (temp_timeDifference < timeDifference) {
								atLeastOneMatchingEpochFound = true;
								timeDifference = temp_timeDifference;
								it_target = it_target_iterator;
							}
							else {
								break;
							}
						}
						else {
							if (atLeastOneMatchingEpochFound == true)
								break;
						}
					}
				}
				
				if (it_target == ionoStoreTarget.inxMaps.end()) {
					gpstk::CivilTime unmatchedCivTime(epoch_ref);
					cout << "Times of map not matched in " << unmatchedCivTime.asString() << endl;
					continue;
				}
				else {

					epoch_target = it_target->first;
				}

				if (abs(epoch_ref - epoch_target) > matchingIntervall) {
					gpstk::CivilTime unmatchedCivTime(epoch_ref);
					cout << "Times of map not matched in " << unmatchedCivTime.asString() << endl;
					continue;
				}

				ionexDataVector = EGNOS_RUNNABLE_UTILITY::createDifferenceDataBlock(targetHeader,
																					ionoStoreRef,
																					ionoStoreTarget,
																					epoch_ref,
																					mapID);

				for (size_t index = 0; index < ionexDataVector.size(); index++){

					outIonexStoreStream << ionexDataVector[index];
				}
				
				mapID++;
			}

			outIonexStoreStream.close();
		}

		void processEMS(std::string EDAS_FileNamewPath,
						std::string Output_IonexFileNamewPath,
						std::string Output_IonexFileNamewPathLast,
						std::string Output_IonexFileNamewPath_Detailed,
						EGNOSMapType mapType,
						bool interPolationOn,
						gpstk::CommonTime firstUpdate,
						gpstk::CommonTime lastUpdate,
						double updateIntervalinSeconds) {


			if (EGNOS_RUNNABLE_UTILITY::checkArgs(	EDAS_FileNamewPath,
													Output_IonexFileNamewPath,
													Output_IonexFileNamewPathLast,
													Output_IonexFileNamewPath_Detailed,
													mapType,
													interPolationOn,
													updateIntervalinSeconds) == false) 
			{
				return;
			}

			// Make sur that the firstUpdate and time's time systems are the same and compatible to EMS data timesystem
			firstUpdate.setTimeSystem(gpstk::TimeSystem::GPS);
			lastUpdate.setTimeSystem(gpstk::TimeSystem::GPS);

			EGNOS_EMS_Parser::EGNOS_EMS_Stream EMSStream(EDAS_FileNamewPath.c_str());
			EGNOS_EMS_Parser::EGNOS_EMS_Data EData;

			EGNOS::IonosphericDelayCorrectionsMessageParser IonoGridPointParser;
			EGNOS::IonosphericGridPointMasksMessageParser IonoMaskParser;
			EGNOS::IGPMapStore igpMapStore;

			gpstk::CommonTime CurrentDataTime;
			gpstk::CommonTime LastUpdateTime;

			EGNOS::IGPMap IonoMap;
			EGNOS::VerticalIonoDelayInterpolator egnosInterPol;
			EGNOS::IonexCreator ionexWriter;
			ionexWriter.headerType = mapType; //EGNOS::europe5x5_tec; // europe5x5_tec | europe2_5x2_5_tec |europe1x1_tec
			EGNOS::IGPMediator IgpMediator;

			if (interPolationOn == true)
				ionexWriter.setInterpolator(egnosInterPol);

			double timOffset = 0;
			bool weHad18 = false;
			bool weHad26 = false;
			bool newData = false;
			bool hadFirstTimeData = false;

			int updateIndex = 0;
			while (EMSStream >> EData) {

				try	{
					CurrentDataTime = EData.messageTime;
				}
				catch (...)	{

					cout << "Time conversion failed so we skipped this epoch. Civil Time: " << EData.messageTime.asString() << endl;
					continue;
				}
				
				if (hadFirstTimeData == false) {

					LastUpdateTime = CurrentDataTime;
					gpstk::CivilTime firstCivilTime(CurrentDataTime);
					gpstk::CivilTime firstUpdateinCivilTime(firstUpdate);
					gpstk::CivilTime lastUpdateinCivilTime(lastUpdate);

					if (firstCivilTime > firstUpdate) {

						firstUpdateinCivilTime.year = firstCivilTime.year;
						firstUpdateinCivilTime.month = firstCivilTime.month;
						firstUpdateinCivilTime.day = firstCivilTime.day;
						firstUpdateinCivilTime.hour = firstCivilTime.hour;
						firstUpdateinCivilTime.minute = firstCivilTime.minute;
						firstUpdateinCivilTime.second = firstCivilTime.second;
						firstUpdate = firstUpdateinCivilTime;
					}
					
					if (lastUpdate == gpstk::CommonTime() || lastUpdate < firstUpdate) {

						lastUpdateinCivilTime.year = firstCivilTime.year;
						lastUpdateinCivilTime.month = firstCivilTime.month;
						lastUpdateinCivilTime.day = firstCivilTime.day + 1;
						lastUpdateinCivilTime.hour = firstCivilTime.hour;
						lastUpdateinCivilTime.minute = firstCivilTime.minute;
						lastUpdateinCivilTime.second = firstCivilTime.second;
						lastUpdate = lastUpdateinCivilTime;
					}

					hadFirstTimeData = true;
				}

				if (EData.messageId == 18) {

					IonoMaskParser += EData.message;
					//cout << IonoMaskParser << endl;
					weHad18 = true;
				}

				if (EData.messageId == 26) {

					IonoGridPointParser += EData.message;
					//cout << IonoGridPointParser << endl;
					weHad26 = true;
				}

				if (weHad18 || weHad26) {

					newData = false;

					IgpMediator.updateTime(CurrentDataTime);
					IgpMediator.setIGPCandidates(IonoGridPointParser.getIonosphericGridPoint());
					IgpMediator.updateIGPCandidate(IonoMaskParser);

					newData = IonoMap.updateMap(IgpMediator);

					//cout << IonoMap;

					weHad26 = false;
					weHad18 = false;
				}
					
				if (updateIntervalinSeconds > 0) {

					if (CurrentDataTime >= (firstUpdate + timOffset) && CurrentDataTime >= firstUpdate && CurrentDataTime <= lastUpdate) {

						timOffset += updateIntervalinSeconds;
						LastUpdateTime = CurrentDataTime;

						IonoMap.referenceTime = CurrentDataTime;
						igpMapStore.addMap(CurrentDataTime, IonoMap);

						if (newData == true) {
							gpstk::CivilTime clock(CurrentDataTime);
							cout << "Time: " << clock.asString() << ": IGP Map was added to IGPMapStore" << endl;
						}
						else {
							gpstk::CivilTime clock(CurrentDataTime);
							cout << "Time: " << clock.asString() << ": IGP Map was updated and added to IGPMapStore" << endl;
						}
						newData = false;
					}
							
				}
				else {
					if (newData == true) {
						igpMapStore.addMap(CurrentDataTime, IonoMap);
						gpstk::CivilTime clock(CurrentDataTime);
						cout << "Time: " << clock.asString() << ": IGP Map was updated and added to IGPMapStore" << endl;
						newData = false;
					}
						
				}

				if (newData == true) {

					if (Output_IonexFileNamewPath_Detailed != "") {
						updateIndex++;
						string ems_out_file_index = EGNOS_RUNNABLE_UTILITY::createStrFileIndex(updateIndex);
						std::string ionexFile_Out_Detailed = Output_IonexFileNamewPath_Detailed;
						ionexFile_Out_Detailed = ionexFile_Out_Detailed.insert(Output_IonexFileNamewPath_Detailed.size() - 4, ems_out_file_index.c_str());

						ionexWriter.setIonexCompatibleMap(IonoMap);
						ionexWriter.writeIGPMap2file(ionexFile_Out_Detailed);
						cout << "IGP Map was written to a standalone ionex file" << endl;
						newData = false;
					}	
				}	

			}

			EMSStream.close();
			//cout << IonoMap;

			if (Output_IonexFileNamewPathLast != "") {

				ionexWriter.setIonexCompatibleMap(IonoMap);
				ionexWriter.writeIGPMap2file(Output_IonexFileNamewPathLast);
			}

			if (Output_IonexFileNamewPath != "") {
				ionexWriter.setIonexCompatibleMap(igpMapStore);
				try
				{
					ionexWriter.writeIGPMap2file(Output_IonexFileNamewPath);
				}
				catch (const std::exception& e)
				{
					std::cout << std::endl << "Ionex file creation has failed" << std::endl;
					std::cout << "ErrorType: " << e.what() << std::endl << std::endl;
				}
				
			}

			return;
		}
	};
		
	namespace EGNOS_RUNNABLE_UTILITY
	{

		bool checkArgs(std::string EDAS_FileNamewPath,
			std::string Output_IonexFileNamewPath,
			std::string Output_IonexFileNamewPathLast,
			std::string Output_IonexFileNamewPath_Detailed,
			EGNOS::EGNOSMapType mapType,
			bool interPolationOn,
			unsigned int updateIntervalinSeconds) {

			if (EDAS_FileNamewPath == "") {
				std::cout << "First give me an EMS file" << std::endl;
				return false;
			}

			if (interPolationOn == true) {
				std::cout << "Enabled: Spatial interpolation" << std::endl;
			}
			else {
				std::cout << "Disabled: Spatial interpolation" << std::endl;
			}

			if (updateIntervalinSeconds == 0) {
				std::cout << "IGPMapStore is updated every time when we have an update from EGNOS" << std::endl;
			}
			else {
				std::cout << "IGPMapStore is updated in fix intervals. Interval: "<< updateIntervalinSeconds << " s" << std::endl;
			}

			std::cout << "Map Type: " << EGNOSMapType2String(mapType) << std::endl;

			if (Output_IonexFileNamewPath_Detailed == "") {
				std::cout << "Enabled:  Writing out each newly updated IGPMap in separate files during EMS file process." << endl;
			}
			else {
				std::cout << "Disabled: Writing out each newly updated IGPMap in separate files during EMS file process." << std::endl;
			}

			if (Output_IonexFileNamewPathLast != "") {
				std::cout << "Enabled: Last IGPMap will be written to a separate ionex file." << std::endl;
			}
			else {
				std::cout << "Disabled: Last IGPMap will be written to a separate ionex file." << std::endl;
			}

			if (Output_IonexFileNamewPath != "") {
				std::cout << "Enabled: All IGPMap will be written to the same ionex file." << std::endl;
			}
			else {
				std::cout << "Disabled: All IGPMap will be written to the same ionex file." << std::endl;
			}

			return true;
		}

		string createStrFileIndex(int index) {

			index = abs(index);

			string ems_out_file_index = std::to_string(index);

			if (index < 10) {
				ems_out_file_index = "0000" + ems_out_file_index;
			}
			else if (index >= 10 && index < 100) {
				ems_out_file_index = "000" + ems_out_file_index;
			}
			else if (index >= 100 && index < 1000) {
				ems_out_file_index = "00" + ems_out_file_index;
			}
			else if (index >= 1000 && index < 10000) {
				ems_out_file_index = "0" + ems_out_file_index;
			}
			else if (index >= 10000 && index < 100000) {
				// Do nothing
			}
			else {
				cout << "file index is to big" << endl;
			}

			return ems_out_file_index;
		}

		int calcDim(int lat1, int lat2, double dlat) {

			if (dlat == 0) {
				return 1;
			}
			else {
				return int(abs((int(lat2) - int(lat1)) / dlat) + 1);
			}
		}

		std::vector<gpstk::IonexData> createDifferenceDataBlock(	gpstk::IonexHeader &header,
																	gpstk::IonexStore &refreceStore,
																	gpstk::IonexStore &targetStore,
																	gpstk::CommonTime &epoch,
																	int mapID) {
			
			std::vector<gpstk::IonexData> rtv;
			gpstk::IonexData ionexDataTEC;
			gpstk::IonexData ionexDataRMS;
			gpstk::IonexData ionexMaxLikeliHood;
			gpstk::IonexData ionexChi2Prob;

			double lat1, lat2, dlat;
			double lon1, lon2, dlon;
			double hgt1, hgt2, dhgt;
			int dimlat, dimlon, dimhgt;

			lat1 = header.lat[0];
			lat2 = header.lat[1];
			dlat = header.lat[2];

			lon1 = header.lon[0];
			lon2 = header.lon[1];
			dlon = header.lon[2];

			hgt1 = header.hgt[0];
			hgt2 = header.hgt[1];
			dhgt = header.hgt[2];

			dimlat = calcDim(lat1, lat2, dlat);
			dimlon = calcDim(lon1, lon2, dlon);
			dimhgt = calcDim(hgt1, hgt2, dhgt);

			double numberOfValues = dimlat * dimlon;

			gpstk::Vector<double> values(numberOfValues);
			int counter = 0;

			double currLat = lat1;
			double currLon = lon1;
			double TECorRMS;
			
			gpstk::CommonTime initTime = refreceStore.getInitialTime();
			gpstk::CommonTime finalTime = refreceStore.getFinalTime();

			double diffTEC;
			double diffRMS;
			double maxLikeliHood;
			double Chi2Prob;

			gpstk::Triple TECRMS1, TECRMS2;

			gpstk::Vector<double> valuesTEC(numberOfValues);
			gpstk::Vector<double> valuesRMS(numberOfValues);
			gpstk::Vector<double> valuesMLH(numberOfValues);
			gpstk::Vector<double> valuesChi2(numberOfValues);
			gpstk::Position RX;

			while (counter < numberOfValues)
			{

				RX.setGeocentric(currLat, currLon, 0);

				try
				{
					try
					{
						TECRMS1 = refreceStore.getIonexValue(epoch, RX, 4);
						TECRMS2 = targetStore.getIonexValue(epoch, RX, 4);
					}
					catch (gpstk::Exception& e)
					{
						throw domain_error("TEC or RMS value are not available.");
					}

					diffTEC = TECRMS1[0] - TECRMS2[0];
					diffRMS = std::sqrt(TECRMS1[1] * TECRMS1[1] + TECRMS2[1] * TECRMS2[1]);

					maxLikeliHood = calcMaximumLikeliHood(TECRMS1[0], TECRMS1[1], TECRMS2[0], TECRMS2[1]);
					Chi2Prob = calcChi2Probability(TECRMS1[0], TECRMS1[1], TECRMS2[0], TECRMS2[1]);

					valuesTEC(counter) = diffTEC;
					valuesRMS(counter) = diffRMS;

					if (maxLikeliHood >= 0.9999) {
						valuesMLH(counter) = 0.9998;
					}
					else {

						valuesMLH(counter) = maxLikeliHood;
					}

					if (Chi2Prob >= 0.9999) {
						valuesChi2(counter) = 0.9998;
					}
					else {

						valuesChi2(counter) = Chi2Prob;
					}
				}
				catch (const std::domain_error& e)
				{
					valuesTEC(counter) = 999.9;
					valuesRMS(counter) = 999.9;
					valuesMLH(counter) = 0.9999;
					valuesChi2(counter) = 0.9999;
				}

				if (abs(currLon - lon2) < dlon) {
					currLon = lon1 - dlon;
					currLat += dlat;
				}
				currLon += dlon;
				counter++;
			}

			ionexChi2Prob.data = valuesChi2;
			ionexChi2Prob.mapID = mapID;

			ionexMaxLikeliHood.data = valuesMLH;
			ionexMaxLikeliHood.mapID = mapID;

			ionexDataTEC.data = valuesTEC;
			ionexDataTEC.mapID = mapID;

			ionexDataRMS.data = valuesRMS;
			ionexDataRMS.mapID = mapID;

			ionexChi2Prob.dim[0] = dimlat;
			ionexChi2Prob.dim[1] = dimlon;
			ionexChi2Prob.dim[2] = dimhgt;

			ionexMaxLikeliHood.dim[0] = dimlat;
			ionexMaxLikeliHood.dim[1] = dimlon;
			ionexMaxLikeliHood.dim[2] = dimhgt;

			ionexDataTEC.dim[0] = dimlat;
			ionexDataTEC.dim[1] = dimlon;
			ionexDataTEC.dim[2] = dimhgt;

			ionexDataRMS.dim[0] = dimlat;
			ionexDataRMS.dim[1] = dimlon;
			ionexDataRMS.dim[2] = dimhgt;

			ionexChi2Prob.exponent = -4;
			ionexChi2Prob.lat[0] = lat1;
			ionexChi2Prob.lat[1] = lat2;
			ionexChi2Prob.lat[2] = dlat;

			ionexMaxLikeliHood.exponent = -4;
			ionexMaxLikeliHood.lat[0] = lat1;
			ionexMaxLikeliHood.lat[1] = lat2;
			ionexMaxLikeliHood.lat[2] = dlat;

			ionexDataTEC.exponent = -1;
			ionexDataTEC.lat[0] = lat1;
			ionexDataTEC.lat[1] = lat2;
			ionexDataTEC.lat[2] = dlat;

			ionexDataRMS.exponent = -1;
			ionexDataRMS.lat[0] = lat1;
			ionexDataRMS.lat[1] = lat2;
			ionexDataRMS.lat[2] = dlat;

			ionexChi2Prob.lon[0] = lon1;
			ionexChi2Prob.lon[1] = lon2;
			ionexChi2Prob.lon[2] = dlon;

			ionexMaxLikeliHood.lon[0] = lon1;
			ionexMaxLikeliHood.lon[1] = lon2;
			ionexMaxLikeliHood.lon[2] = dlon;

			ionexDataTEC.lon[0] = lon1;
			ionexDataTEC.lon[1] = lon2;
			ionexDataTEC.lon[2] = dlon;

			ionexDataRMS.lon[0] = lon1;
			ionexDataRMS.lon[1] = lon2;
			ionexDataRMS.lon[2] = dlon;

			ionexChi2Prob.hgt[0] = hgt1;
			ionexChi2Prob.hgt[1] = hgt2;
			ionexChi2Prob.hgt[2] = dhgt;

			ionexMaxLikeliHood.hgt[0] = hgt1;
			ionexMaxLikeliHood.hgt[1] = hgt2;
			ionexMaxLikeliHood.hgt[2] = dhgt;

			ionexDataTEC.hgt[0] = hgt1;
			ionexDataTEC.hgt[1] = hgt2;
			ionexDataTEC.hgt[2] = dhgt;

			ionexDataRMS.hgt[0] = hgt1;
			ionexDataRMS.hgt[1] = hgt2;
			ionexDataRMS.hgt[2] = dhgt;

			ionexChi2Prob.valid = true;
			ionexMaxLikeliHood.valid = true;
			ionexDataTEC.valid = true;
			ionexDataRMS.valid = true;

			ionexChi2Prob.type.type			= "CH2";
			ionexMaxLikeliHood.type.type	= "MLH";
			ionexDataTEC.type.type			= "TEC";
			ionexDataRMS.type.type			= "RMS";

			ionexChi2Prob.type.units		= "10e-4";
			ionexMaxLikeliHood.type.units	= "10e-4";
			ionexDataTEC.type.units			= "10e-1 [TEC]";
			ionexDataRMS.type.units			= "10e-1 [TEC]";

			ionexChi2Prob.type.description		= "Chi2 Probability Map";
			ionexMaxLikeliHood.type.description = "Maximum Likelihood Map";
			ionexDataTEC.type.description		= "Total Electron Content Difference map";
			ionexDataRMS.type.description		= "Total Electron Content Difference map";

			ionexChi2Prob.time		= epoch;
			ionexMaxLikeliHood.time = epoch;
			ionexDataTEC.time		= epoch;
			ionexDataRMS.time		= epoch;

			rtv.push_back(ionexDataTEC);
			rtv.push_back(ionexDataRMS);
			rtv.push_back(ionexMaxLikeliHood);
			rtv.push_back(ionexChi2Prob);

			return rtv;
		}

		// Priori gaussian is given: [mean1, std1] ||  Likelihood gaussian is given: [mean2, std2]
		double calcMaximumLikeliHood(double mean1, double std1, double mean2, double std2) {
		
			// Caluclation 
			// Calculate the the mean and std parameters of the probability density of the posterior.
			// Normal[mean1, std1](mu) * Normal[mean2, std2](mu) where mu is coming from a weighted least square or bayesian inference or simple Kalman filter
			// Best Mean: (mean1^2*std2^2 + mean2^2*std1^2) / (std1^2 + std2^2)

			double bestMean = (mean1 * std::pow(std2, 2) + mean2 * std::pow(std1, 2)) / (std::pow(std1, 2) + std::pow(std2, 2));
			double mlh = stats::dnorm(bestMean, mean1, std1 ) * stats::dnorm(bestMean, mean2, std2 );

			// Alternative Calculation
			// MLH = Normal[mean1,sqrt(std1^2+std2^2)](mean2) * Normal[Best Mean, Best Std](Best Mean)
			// Best Mean: (mean1^2*std2^2 + mean2^2*std1^2) / (std1^2 + std2^2)
			// Best Std: (std2^2 * std1^2) / (std1^2 + std2^2)
			// double bestStd = std::sqrt( ( std::pow(std1, 2) * std::pow(std2, 2) ) / ( std::pow(std1, 2) + std::pow(std2, 2) ) );
			// double NormFactor = stats::dnorm(mean1, mean2, std::sqrt( std::pow(std1, 2) + std::pow(std2, 2) ) );
			// double MaxLikelihood = stats::dnorm(bestMean, bestMean, bestStd );
			// double mlh_alter = NormFactor * MaxLikelihood;

			return mlh;
		}

		double calcChi2Probability(double mean1, double std1, double mean2, double std2) {
		
			double bestMean = (mean1 * std::pow(std2, 2) + mean2 * std::pow(std1, 2)) / (std::pow(std1, 2) + std::pow(std2, 2));
			double summaOfNormedSquaredValues = std::pow((mean1 - bestMean), 2) / std::pow(std1, 2) + std::pow((mean2 - bestMean), 2) / std::pow(std2, 2);
			return probChi2_2D(summaOfNormedSquaredValues);
		}

		double probChi2_2D(double x) {
			
			return 0.5 * std::exp(-x/2);
		}

		std::string EGNOSMapType2String(EGNOS::EGNOSMapType mapType) {
		
			std::string mapTypeStr;
			switch (mapType)
			{
			case EGNOS::EGNOSMapType::world5x5_tec:
				mapTypeStr = "world5x5_tec";
				break;
			case EGNOS::EGNOSMapType::world5x5_meter:
				mapTypeStr = "world5x5_meter";
				break;
			case EGNOS::EGNOSMapType::world2_5x2_5_tec:
				mapTypeStr = "world2_5x2_5_tec";
				break;
			case EGNOS::EGNOSMapType::world2_5x2_5_meter:
				mapTypeStr = "world2_5x2_5_meter";
				break;
			case EGNOS::EGNOSMapType::world1x1_tec:
				mapTypeStr = "world1x1_tec";
				break;
			case EGNOS::EGNOSMapType::world1x1_meter:
				mapTypeStr = "world1x1_meter";
				break;
			case EGNOS::EGNOSMapType::europe5x5_tec:
				mapTypeStr = "europe5x5_tec";
				break;
			case EGNOS::EGNOSMapType::europe5x5_meter:
				mapTypeStr = "europe5x5_meter";
				break;
			case EGNOS::EGNOSMapType::europe2_5x2_5_tec:
				mapTypeStr = "europe2_5x2_5_tec";
				break;
			case EGNOS::EGNOSMapType::europe2_5x2_5_meter:
				mapTypeStr = "europe2_5x2_5_meter";
				break;
			case EGNOS::EGNOSMapType::europe1x1_tec:
				mapTypeStr = "europe1x1_tec";
				break;
			case EGNOS::EGNOSMapType::europe1x1_meter:
				mapTypeStr = "europe1x1_meter";
				break;
			default:
				mapTypeStr = "Unkown MapType";
				break;
			}

			return mapTypeStr;
		}
	};

	
};