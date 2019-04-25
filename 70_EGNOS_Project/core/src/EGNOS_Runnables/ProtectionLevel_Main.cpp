#include "ProtectionLevel_Main.hpp"
#include <algorithm>
#include <algorithm>   
#include <vector>     

namespace EGNOS {
	namespace ProtectionLevel {

		static void loadIonoModel(	IonoType IonoType,
									int intervallBetweenEpochsinSecs,
									std::string& ionexFile,
									std::string& EMSData,
									std::vector<gpstk::CommonTime> &epochsForPL,
									EGNOS::IonoModel **IonoModel_PLEngine);

		static void createIonexModel(std::string &ionexFile, 
									gpstk::IonexStore &ionoStore);

		static Eigen::MatrixXd createIonoCovMatrix(const gpstk::CommonTime &time,
			const gpstk::GPSEphemerisStore &bcestore,
			const vector<gpstk::SatID> &id,
			EGNOS::IonoModel &pIonoModel,
			const gpstk::Position &Rx,
			Eigen::VectorXd& corrVector);

		static Eigen::MatrixXd createIonoCovMatrix(	const gpstk::CommonTime &time,
													const gpstk::GPSEphemerisStore &bcestore,
													vector<gpstk::SatID> &id, 
													EGNOS::IonoModel &pIonoModel,
													const gpstk::Position &Rx,
													Eigen::VectorXd& corrVector,
													std::vector<ProtectionLevel_Parser::ProtectionLevel_Data::SatInfo> &satInfo);

		static std::vector<gpstk::CommonTime> createVectorofEpochforPLCalc(EGNOS::IonoModel &iono, int intervallBetweenEpochsinSecs);
		
		static void createVectorofCandidateSV(vector<gpstk::SatID> &prnVec);

		static void applyElevationMask( double elevationMask,
										const gpstk::Position &R, 
										vector<gpstk::SatID> &prnVec, 
										const gpstk::GPSEphemerisStore &bcestore,
										const gpstk::CommonTime &gpstime);

		static void applyIonoCorrection(const gpstk::CommonTime &time,
										const gpstk::GPSEphemerisStore &bcestore,
										gpstk::SatID &id,
										EGNOS::IonoModel &pIonoModel,
										const gpstk::Position Rx, 
										double &corrInMeter,
										double &varianceInMeter,
										double &el, 
										double &az);

		void run_PL(std::string &ephData, 
					string& ionexFile,
					std::string& EMSData, 
					std::string& PLwPath_out, 
					double elevationMask, 
					IonoType referenceIonoType, 
					IonoType targetIonoType, 
					double latgeodetic, 
					double lon, 
					double height, 
					int intervallBetweenEpochsinSecs_in, 
					double probability_of_inner_circle) {

		    double intervallBetweenEpochsinSecs = intervallBetweenEpochsinSecs_in;
			gpstk::Position Rx({ latgeodetic, lon, height },
				gpstk::Position::CoordinateSystem::Geodetic,
				NULL, gpstk::ReferenceFrame::WGS84);

			// Declaration of objects for storing ephemerides and handling RAIM
			gpstk::GPSEphemerisStore bcestoreGps; 
			gpstk::GalEphemerisStore bcestoreGal;
			gpstk::GloEphemerisStore bcestoreGlo;

			EGNOS::ZeroIonoModel *noIonoModel = new EGNOS::ZeroIonoModel();

			EGNOS::IonoModel *referenceIonoModel_PLEngine = noIonoModel;
			EGNOS::IonoModel *targetIonoModel_PLEngine = noIonoModel;

			std::vector<gpstk::CommonTime> epochsForPL;
			std::vector<gpstk::CommonTime> refEpochsForPL;


			loadIonoModel(	targetIonoType,
							intervallBetweenEpochsinSecs,
							ionexFile,
							EMSData,
							epochsForPL,
							&targetIonoModel_PLEngine);

			loadIonoModel(	referenceIonoType,
							intervallBetweenEpochsinSecs,
							ionexFile,
							EMSData,
							refEpochsForPL,
							&referenceIonoModel_PLEngine);

			std::cout << "IonoModel is set " << std::endl;
			
			// Read nav file and store unique list of ephemerides
			gpstk::Rinex3NavStream rnffs(ephData.c_str());    // Open ephemerides data file
			gpstk::Rinex3NavData rne;
			gpstk::Rinex3NavHeader hdr;

			// Let's read the header (may be skipped)
			rnffs >> hdr;

			if (hdr.valid == 0) {
				std::cout << "Error. Brdc ephemeris is not valid" << std::endl;
				std::exit(1);
			}

			// Storing the ephemeris in "bcstore"
			while (rnffs >> rne) {
				try {
					bcestoreGps.addEphemeris(rne);
				}
				catch (gpstk::Exception& e) {
					std::cout << e.getText();
					std::exit(1);
				}
			}
			// Setting the criteria for looking up ephemeris
			bcestoreGps.SearchNear();
			
			EGNOS::ProtectionLevel::EGNOS_PL plEngine(bcestoreGps, bcestoreGal, bcestoreGlo);
			double hp_radius = 0;

			ProtectionLevel_Parser::ProtectionLevel_Stream pl_strm_out(PLwPath_out.c_str(), std::ios::out);
			
			for (int epochIndex = 0; epochIndex < epochsForPL.size(); epochIndex++)	{

				// Apply elevation mask
				ProtectionLevel_Parser::ProtectionLevel_Data plData;
				vector<gpstk::SatID> prnVec;
				vector<gpstk::SatID> original_prnVec;
				vector<gpstk::SatID> excluded_prnVec;
				std::vector<gpstk::SatID>::iterator SatId_it;

				createVectorofCandidateSV(prnVec);

				applyElevationMask(	elevationMask,
									Rx,
									prnVec,
									bcestoreGps,
									epochsForPL[epochIndex]);
			
				Eigen::VectorXd corrVector;
				Eigen::MatrixXd CovMatrix;
				Eigen::MatrixXd WeightMatrix;
				try
				{
					original_prnVec = prnVec;
					if (targetIonoModel_PLEngine->name() == noIonoModel->name()) {
						std::cout << "Error. No target Iono model was set.";
						std::exit(1);
					}

					CovMatrix = createIonoCovMatrix(epochsForPL[epochIndex], bcestoreGps, prnVec, *targetIonoModel_PLEngine, Rx, corrVector, plData.satInfo);
					WeightMatrix = CovMatrix.inverse();

					for (int a = 0; a < original_prnVec.size(); a++){

						std::vector<gpstk::SatID>::iterator SatId_it = std::find(prnVec.begin(), prnVec.end(), original_prnVec[a]);
						if (SatId_it == prnVec.end()) {
							excluded_prnVec.push_back(original_prnVec[a]);
						}
							
					}

					if (referenceIonoModel_PLEngine->name() != noIonoModel->name()) {
						Eigen::VectorXd corrVector_reff;
						try
						{
							(void)createIonoCovMatrix(epochsForPL[epochIndex], bcestoreGps, prnVec, *referenceIonoModel_PLEngine, Rx, corrVector_reff, plData.satInfo);
							
							gpstk::CivilTime errorTime(epochsForPL[epochIndex]);
							if (corrVector_reff.size() != corrVector.size()) {
								
								std::cout << errorTime.asString() << std::endl;
								std::cout << "The reference iono corrections cannot be calculated entirely. Some iono value hadn't been calculated. " << std::endl;
								continue;
							}

							std::cout << errorTime.asString() << std::endl;
							std::cout << "diff corrVector = corrVector - corrVector_reff" << endl;
							for (int i = 0; i < corrVector.size(); i++)	{

								std::cout << corrVector(i) - corrVector_reff(i) << " " << corrVector(i) << " " << corrVector_reff(i) << " " << endl;
							}
						
							corrVector -= corrVector_reff;
						}
						catch (const std::exception& e)
						{
							gpstk::CivilTime errorTime(epochsForPL[epochIndex]);
							std::cout << errorTime.asString() << " ";
							std::cout << e.what() << std::endl;
							continue;
						}
					}
				}
				catch (const std::exception&e)
				{
					gpstk::CivilTime errorTime(epochsForPL[epochIndex]);
					std::cout << errorTime.asString() << " ";
					std::cout << e.what() << std::endl;
					continue;
				}
				
				hp_radius = plEngine.calculatePL(epochsForPL[epochIndex], Rx, prnVec, CovMatrix, probability_of_inner_circle);
				
				try
				{
					Eigen::Vector3d enuError = plEngine.calculatePositionError_in_enu(epochsForPL[epochIndex], Rx, prnVec, corrVector, WeightMatrix);

					plData.posError = enuError.norm();
					plData.horizontalPosError = std::sqrt(std::pow(enuError(0), 2) + std::pow(enuError(1), 2));
					plData.verticalPosError = std::abs(enuError(2));
				}
				catch (const std::domain_error& e)
				{
					plData.posError = ProtectionLevel_Parser::ProtectionLevel_Data::UNVALID_PL_DATA;
					plData.horizontalPosError = ProtectionLevel_Parser::ProtectionLevel_Data::UNVALID_PL_DATA;
					plData.verticalPosError = ProtectionLevel_Parser::ProtectionLevel_Data::UNVALID_PL_DATA;
				}
				
				plData.dataTime = epochsForPL[epochIndex];
				plData.posData = Rx;
				plData.elevationMask = elevationMask;
				plData.Covariance_enu = plEngine.PosCovMatrix;
				plData.Covariance_ecef = plEngine.PosCovMatrix_ecef;
				plData.probabilityOfIntegrity = probability_of_inner_circle;
				plData.includedSatIds = prnVec;
				plData.excludedSatIds = excluded_prnVec;
				plData.HPL = hp_radius;
				
				pl_strm_out << plData;
			} 

			pl_strm_out.close();

			// release memory
			delete referenceIonoModel_PLEngine;
			delete targetIonoModel_PLEngine;
			std::exit(0);
		}

		static void loadIonoModel(	IonoType IonoType, 
									int intervallBetweenEpochsinSecs, 
									std::string& ionexFile,
									std::string& EMSData,
									std::vector<gpstk::CommonTime> &epochsForPL,
									EGNOS::IonoModel **IonoModel_PLEngine) {
		
			switch (IonoType) {
				case IonoType::egnos:
				{

					EGNOS::EGNOSIonoCorrectionModel *egnosIonoModel = new EGNOS::EGNOSIonoCorrectionModel();
					egnosIonoModel->updateIntervalinSeconds = intervallBetweenEpochsinSecs;
					egnosIonoModel->load(EMSData);
					epochsForPL = createVectorofEpochforPLCalc(*egnosIonoModel, intervallBetweenEpochsinSecs);

					*IonoModel_PLEngine = egnosIonoModel;
					break;
				}
				case IonoType::ionex:
				{
					gpstk::IonexStore ionoStore;
					try
					{
						createIonexModel(ionexFile, ionoStore);
						EGNOS::IonexModel *ionexModel = new EGNOS::IonexModel();
						ionexModel->addIonexStore(ionoStore);

						epochsForPL = createVectorofEpochforPLCalc(*ionexModel, intervallBetweenEpochsinSecs);
						*IonoModel_PLEngine = ionexModel;
					}
					catch (gpstk::Exception& e)
					{
						std::cout << e.what() << std::endl;
					}
					break;
				}
				case IonoType::zero:
				{
					break;
				}
				default:
				{
					break;
				}
			}

		}

		static void createIonexModel(std::string &ionexFile, gpstk::IonexStore &ionoStore) {
		
				// Stream creation
				gpstk::IonexStream strm(ionexFile.c_str(), std::ios::in);

				if (!strm) {

					gpstk::FileMissingException e("File " + ionexFile +
						" could not be opened.");
					GPSTK_THROW(e);
				}

				// create the header object
				gpstk::IonexHeader header;
				strm >> header;

				if (!header.valid) {

					gpstk::FileMissingException e("File " + ionexFile +
						" could not be opened. Check again " +
						"the path or the name provided!");
					GPSTK_THROW(e);
				}

				header.firstEpoch.setTimeSystem(gpstk::TimeSystem::GPS);
				header.lastEpoch.setTimeSystem(gpstk::TimeSystem::GPS);

				// keep an inventory of the loaded files 
				ionoStore.addFile(ionexFile, header);

				// this map is useful in finding DCB value
				ionoStore.inxDCBMap[header.firstEpoch] = header.svsmap;

				// object data. If valid, add to the map
				gpstk::IonexData iod;
				while (strm >> iod && iod.isValid()) {

					iod.time.setTimeSystem(gpstk::TimeSystem::GPS);
					ionoStore.addMap(iod);
				}
		}

		static Eigen::MatrixXd createIonoCovMatrix(	const gpstk::CommonTime &time, 
													const gpstk::GPSEphemerisStore &bcestore, 
													vector<gpstk::SatID> &id, 
													EGNOS::IonoModel &pIonoModel, 
													const gpstk::Position &Rx,
													Eigen::VectorXd& corrVector, 
													std::vector<ProtectionLevel_Parser::ProtectionLevel_Data::SatInfo> &satInfo) {

			vector<double> ionoCorrections;
			vector<double> ionoVariance;
			vector<gpstk::SatID> tempId = id;
			id.clear();

			double tempCorr;
			double tempVar;
			//applyIonoCorrection(time, bcestore, id, ionoCorrections, pIonoModel, Rx, &tempCorr, &tempVar);
			for (int i = 0; i < tempId.size(); i++) {

				double elevation;
				double azimuth;
				try
				{
					applyIonoCorrection(time, bcestore, tempId[i], pIonoModel, Rx, tempCorr, tempVar, elevation, azimuth);
					ionoCorrections.push_back(tempCorr);
					ionoVariance.push_back(tempVar);
					id.push_back(tempId[i]);

					ProtectionLevel_Parser::ProtectionLevel_Data::SatInfo info;

					SlantIonoDelay_Input inputData;
					IonosphericGridPoint igpPP;
					SlantIonoDelay slantCalculator;

					inputData.RoverPos.rlat = Rx.geodeticLatitude();
					inputData.RoverPos.rlon = Rx.longitude();
					inputData.RoverPos.rheight = Rx.height();
					inputData.SatVisibility.elevationOfSatId = elevation;
					inputData.SatVisibility.azimuthOfSatId = azimuth;

					(void)slantCalculator.getSlantFactorandPP(inputData, igpPP.lat, igpPP.lon);

					info.satId = tempId[i];

					info.ionoCorr_meter = tempCorr;
					info.ionoCorr_meter_valid = true;

					info.ionoRMS_meter = sqrt(tempVar);
					info.ionoRMS_meter_valid = true;

					info.az_deg = azimuth;
					info.az_deg_valid = true;

					info.el_deg = elevation;
					info.el_deg_valid = true;

					info.ippLat = igpPP.lat;
					info.ippLat_valid = true;

					info.ippLon = igpPP.lon;
					info.ippLon_valid = true;

					satInfo.push_back(info);
				}
				catch (const std::exception& e)
				{
					ProtectionLevel_Parser::ProtectionLevel_Data::SatInfo info;
					try
					{
						gpstk::Xvt SVxvt;
						SVxvt = bcestore.getXvt(tempId[i], time);
						gpstk::Position S(SVxvt);
						elevation = Rx.elevationGeodetic(S);
						azimuth = Rx.azimuthGeodetic(S);
					}
					catch (gpstk::Exception& e) {

						info.az_deg_valid = false;
						info.el_deg_valid = false;
						info.ippLat_valid = false;
						info.ippLon_valid = false;

						continue;
					}

					SlantIonoDelay_Input inputData;
					IonosphericGridPoint igpPP;
					SlantIonoDelay slantCalculator;

					inputData.RoverPos.rlat = Rx.geodeticLatitude();
					inputData.RoverPos.rlon = Rx.longitude();
					inputData.RoverPos.rheight = Rx.height();
					inputData.SatVisibility.elevationOfSatId = elevation;
					inputData.SatVisibility.azimuthOfSatId = azimuth;

					(void)slantCalculator.getSlantFactorandPP(inputData, igpPP.lat, igpPP.lon);

					info.satId = tempId[i];

					info.ionoCorr_meter_valid = false;
					info.ionoRMS_meter_valid = false;

					info.az_deg = azimuth;
					info.az_deg_valid = true;

					info.el_deg = elevation;
					info.el_deg_valid = true;

					info.ippLat = igpPP.lat;
					info.ippLat_valid = true;

					info.ippLon = igpPP.lon;
					info.ippLon_valid = true;

					satInfo.push_back(info);
					continue;
				}

			}

			if (ionoCorrections.size() == 0) {
				throw domain_error("No corrections and variances had been calculated.");
			}



			Eigen::Map<Eigen::VectorXd> corrVector_local(&ionoCorrections[0], id.size());
			corrVector = corrVector_local;
			Eigen::Map<Eigen::VectorXd> varVector(&ionoVariance[0], id.size());
			Eigen::MatrixXd CovMatrix = varVector.asDiagonal();

			return CovMatrix;
		}

		static Eigen::MatrixXd createIonoCovMatrix(	const gpstk::CommonTime &time, 
													const gpstk::GPSEphemerisStore &bcestore, 
													const vector<gpstk::SatID> &id, 
													EGNOS::IonoModel &pIonoModel, 
													const gpstk::Position &Rx, 
													Eigen::VectorXd& corrVector) {
			
			std::vector<ProtectionLevel_Parser::ProtectionLevel_Data::SatInfo> satInfo;
			Eigen::MatrixXd returnCovMatrix;
			vector<gpstk::SatID> local_id = id;
			returnCovMatrix = createIonoCovMatrix(time, bcestore, local_id, pIonoModel, Rx, corrVector, satInfo);

			return returnCovMatrix;
		}

		static std::vector<gpstk::CommonTime> createVectorofEpochforPLCalc(	EGNOS::IonoModel &iono, 
																			int intervallBetweenEpochsinSecs) {
		
			std::vector<gpstk::CommonTime> epochs;
			std::vector<gpstk::CommonTime> firs_and_last = iono.getFirstandLastEpoch();
			int timeOffsetinSecs = 0;

			if (firs_and_last.size() != 2)	{

				return epochs;
			}

			gpstk::CommonTime firsEpoch = firs_and_last[0];
			gpstk::CommonTime lastEpoch = firs_and_last[1];

			while (firsEpoch + timeOffsetinSecs <= lastEpoch) {
			
				epochs.push_back(firsEpoch + timeOffsetinSecs);
				timeOffsetinSecs += intervallBetweenEpochsinSecs;
			}

			return epochs;	
		}

		static void createVectorofCandidateSV(vector<gpstk::SatID> &prnVec) {

			const gpstk::SatID::SatelliteSystem satSys = gpstk::SatID::SatelliteSystem::systemGPS;
		
			for (int svId = 1; svId < 33; svId++)	{

				prnVec.push_back(gpstk::SatID(svId, satSys));
			}
			
		}

		static void applyElevationMask(	double elevationMask, 
										const gpstk::Position &R,
										vector<gpstk::SatID> &prnVec, 
										const gpstk::GPSEphemerisStore &bcestore, 
										const gpstk::CommonTime &gpstime) {

			vector<gpstk::SatID> prnVec_old;
			vector<double> rangeVec_old;

			prnVec_old = prnVec;
			prnVec.clear();

			gpstk::Xvt satPos;
			double elev = 0;

			for (size_t i = 0; i < prnVec_old.size(); i++){

				gpstk::Xvt satPos;

				try {

					satPos = bcestore.getXvt(prnVec_old[i], gpstime);

				}
				catch (gpstk::Exception& e) {
					throw domain_error("Sat position cannot be calculated");
				}

				gpstk::Position S(satPos);
				elev = R.elevation(S);

				if (elev >= elevationMask) {
					prnVec.push_back(prnVec_old[i]);
				}
			}

		}

		static void applyIonoCorrection(	const gpstk::CommonTime &time, 
											const gpstk::GPSEphemerisStore &bcestore, 
											gpstk::SatID &id, 
											EGNOS::IonoModel &pIonoModel, 
											const gpstk::Position Rx, 
											double &corrInMeter, 
											double &varianceInMeter,
											double &el, 
											double &az) {

			vector<double> corrAndVariance;
			gpstk::Xvt SVxvt;

			try 
			{
				SVxvt = bcestore.getXvt(id, time);
			}
			catch (gpstk::Exception& e) {
				throw domain_error("Sat position cannot be calculated");
			}

			gpstk::Position S(SVxvt);

			el = Rx.elevationGeodetic(S);
			az = Rx.azimuthGeodetic(S);

			if (el < 0.0) {
				throw domain_error("Elevation is negative");
			}

			EGNOS::IonCorrandVar iCorrVar = { 0 };
			double ic = 0;
			try
			{
				iCorrVar = pIonoModel.getCorrection(time, Rx, el, az);
				corrInMeter = iCorrVar.CorrinMeter;
				varianceInMeter = iCorrVar.Variance;
			}
			catch (const gpstk::Exception& e)
			{
				throw domain_error(e.getText());
			}
			catch (const std::exception&e)
			{
				throw domain_error(e.what());
			}
			
		}

	};
};