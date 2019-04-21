#include "ProtectionLevel_Main.hpp"



namespace EGNOS {
	namespace ProtectionLevel {

		static Eigen::MatrixXd createIonoCovMatrix(	gpstk::CommonTime &time, 
													gpstk::GPSEphemerisStore &bcestore, 
													vector<gpstk::SatID> &id, 
													EGNOS::IonoModel &pIonoModel, 
													gpstk::Position &Rx, 
													Eigen::VectorXd& corrVector);

		static std::vector<gpstk::CommonTime> createVectorofEpochforPLCalc(EGNOS::IonoModel &iono, int intervallBetweenEpochsinSecs);
		
		static void createVectorofCandidateSV(vector<gpstk::SatID> &prnVec);

		static void applyElevationMask(double elevationMask,
										gpstk::Position &R, 
										vector<gpstk::SatID> &prnVec, 
										gpstk::GPSEphemerisStore &bcestore, 
										gpstk::CommonTime &gpstime);

		static void applyIonoCorrection(gpstk::CommonTime &time,
										gpstk::GPSEphemerisStore &bcestore,
										gpstk::SatID &id,
										EGNOS::IonoModel &pIonoModel,
										gpstk::Position Rx, double &corrInMeter,
										double &varianceInMeter);

		void run_PL(std::string &ephData, string& ionexFile, std::string& EMSData, double elevationMask, IonoType ionoType) {

			int intervallBetweenEpochsinSecs = 600;
			gpstk::Position Rx(	{45, 15, 0},
								gpstk::Position::CoordinateSystem::Geodetic,
								NULL, gpstk::ReferenceFrame::WGS84);

			// Declaration of objects for storing ephemerides and handling RAIM
			gpstk::GPSEphemerisStore bcestoreGps; 
			gpstk::GalEphemerisStore bcestoreGal;
			gpstk::GloEphemerisStore bcestoreGlo;

			EGNOS::ZeroIonoModel noIonoModel;
			EGNOS::IonexModel ionexModel;
			EGNOS::EGNOSIonoCorrectionModel egnosIonoModel;

			EGNOS::IonoModel *ionoModel_PLEngine = &noIonoModel;

			gpstk::IonexStore ionoStore;

			std::vector<gpstk::CommonTime> epochsForPL;

			switch (ionoType) {
				case IonoType::egnos:
				{
					egnosIonoModel.updateIntervalinSeconds = intervallBetweenEpochsinSecs;
					egnosIonoModel.load(EMSData);
					epochsForPL = createVectorofEpochforPLCalc(egnosIonoModel, intervallBetweenEpochsinSecs);

					ionoModel_PLEngine = &egnosIonoModel;
					break;
				}
				case IonoType::ionex:
				{
					//ionoStore.loadFile(ionexFile);

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

					ionexModel.addIonexStore(ionoStore);

					epochsForPL = createVectorofEpochforPLCalc(ionexModel, intervallBetweenEpochsinSecs);
					ionoModel_PLEngine = &ionexModel;
					break;
				}
				case IonoType::zero:
				{
					epochsForPL = createVectorofEpochforPLCalc(noIonoModel, intervallBetweenEpochsinSecs);
					ionoModel_PLEngine = &noIonoModel;
					break;
				}
				default:
				{
					epochsForPL = createVectorofEpochforPLCalc(noIonoModel, intervallBetweenEpochsinSecs);
					ionoModel_PLEngine = &noIonoModel;
					break;
				}
			}

			std::cout << "IonoModel is set " << std::endl;
			gpstk::FFTextStream debugStrm;
		
			// Read nav file and store unique list of ephemerides
			//Rinex3NavStream rnffs(argv[2].c_str());    // Open ephemerides data file
			gpstk::Rinex3NavStream rnffs(ephData.c_str());    // Open ephemerides data file
			gpstk::Rinex3NavData rne;
			gpstk::Rinex3NavHeader hdr;

				// Let's read the header (may be skipped)
			rnffs >> hdr;

				// Storing the ephemeris in "bcstore"
			while (rnffs >> rne) {
				try {
					bcestoreGps.addEphemeris(rne);
				}
				catch (gpstk::Exception& e) {
					cerr << e;
				}
			}

			// Setting the criteria for looking up ephemeris
			bcestoreGps.SearchNear();
			
			EGNOS::ProtectionLevel::EGNOS_PL plEngine(bcestoreGps, bcestoreGal, bcestoreGlo);
			double hp_radius = 0;
			double probability_of_inner_circle = 0.99;
			for (int epochIndex = 0; epochIndex < epochsForPL.size(); epochIndex++)	{

				// Apply elevation mask
				vector<gpstk::SatID> prnVec;
				createVectorofCandidateSV(prnVec);

				applyElevationMask(	elevationMask,
									Rx,
									prnVec,
									bcestoreGps,
									epochsForPL[epochIndex]);

				for (size_t i = 0; i < prnVec.size(); i++) {

					std::cout << "prnVec: " << prnVec[i] << std::endl;
				}
				//Eigen::MatrixXd CovMatrix = Eigen::MatrixXd::Identity(prnVec.size(), prnVec.size());
				Eigen::VectorXd corrVector;
				Eigen::MatrixXd CovMatrix;
				try
				{
					CovMatrix = createIonoCovMatrix(epochsForPL[epochIndex], bcestoreGps, prnVec, *ionoModel_PLEngine, Rx, corrVector);
				}
				catch (const std::exception&)
				{
					continue;
				}
				
				for (size_t i = 0; i < prnVec.size(); i++){

					std::cout << "prnVec: " << prnVec[i] << std::endl;
				}
				
				std::cout << "CovMatrix: " << std::endl << CovMatrix << std::endl;

				hp_radius = plEngine.calculatePL(epochsForPL[epochIndex], Rx, prnVec, CovMatrix, probability_of_inner_circle);
				
				std::cout << "Radius of PL: "<< hp_radius << std::endl;
				//applyIonoCorrection(rod.time, bcestore, prnVec_gpstk, rangeVec_gpstk, *ionoModel_gpstk, Rx_gpstk);

			}  // End of 'while( roffs >> rod )'

			exit(0);
		}

		static Eigen::MatrixXd createIonoCovMatrix(gpstk::CommonTime &time, gpstk::GPSEphemerisStore &bcestore, vector<gpstk::SatID> &id, EGNOS::IonoModel &pIonoModel, gpstk::Position &Rx, Eigen::VectorXd& corrVector) {
		
			vector<double> ionoCorrections;
			vector<double> ionoVariance;
			vector<gpstk::SatID> tempId = id;
			id.clear();

			double tempCorr;
			double tempVar;
			//applyIonoCorrection(time, bcestore, id, ionoCorrections, pIonoModel, Rx, &tempCorr, &tempVar);
			for (int i = 0; i < tempId.size(); i++){

				try
				{
					applyIonoCorrection(time, bcestore, tempId[i], pIonoModel, Rx, tempCorr, tempVar);
					ionoCorrections.push_back(tempCorr);
					ionoVariance.push_back(tempVar);
					id.push_back(tempId[i]);
				}
				catch (const std::exception& e)
				{
					//std::cout << e.what() << std::endl;
					continue;
				}
				
			}

			if (ionoCorrections.size() == 0) {
				throw domain_error("No corrections and variances had been calculated.");
			}

			Eigen::Map<Eigen::VectorXd> varVector(&ionoVariance[0], id.size());
			Eigen::MatrixXd CovMatrix = varVector.asDiagonal();

			return CovMatrix;
		}

		static std::vector<gpstk::CommonTime> createVectorofEpochforPLCalc(EGNOS::IonoModel &iono, int intervallBetweenEpochsinSecs) {
		
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

		static void applyElevationMask(double elevationMask, gpstk::Position &R, vector<gpstk::SatID> &prnVec, gpstk::GPSEphemerisStore &bcestore, gpstk::CommonTime &gpstime) {

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

		static void applyIonoCorrection(gpstk::CommonTime &time, gpstk::GPSEphemerisStore &bcestore, gpstk::SatID &id, EGNOS::IonoModel &pIonoModel, gpstk::Position Rx, double &corrInMeter, double &varianceInMeter ) {

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

			double el = Rx.elevationGeodetic(S);
			double az = Rx.azimuthGeodetic(S);

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
			catch (const std::exception&)
			{
				throw domain_error("Correction calculated has failed.");
			}
			
		}

	};
};