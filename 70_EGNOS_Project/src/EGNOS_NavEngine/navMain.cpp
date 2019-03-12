﻿
#include "navMain.hpp"
#include "SaasTropModel.hpp"
#include "NeillTropModel.hpp"

#include "RTKPOST_Header.hpp"
#include "RTKPOST_Stream.hpp"
#include "RTKPOST_Data.hpp"

using namespace std;
using namespace gpstk;

int mainNavigationSolution(std::string& obsData, std::string &ephData, std::string& EMSData, std::string& rtkpost_out_gpstk, std::string& rtkpost_out_navEngine, std::string& errorFile)
{
	if (errorFile.empty() != true) {
		freopen(errorFile.c_str(), "w", stderr);
	}
	
	
	std::vector<RTKPOST_Parser::RTKPOST_Pos_Data> navData_RTKPOST;
	std::vector<RTKPOST_Parser::RTKPOST_Pos_Data> navData_GPSTK;

	
	
	// Declaration of objects for storing ephemerides and handling RAIM
	GPSEphemerisStore bcestore; // This is now static
	PRSolution2 raimSolver;

	// Object for void-type tropospheric model (in case no meteorological
	// RINEX is available)
	ZeroTropModel noTropModel;

	// Object for GG-type tropospheric model (Goad and Goodman, 1974)
	// Default constructor => default values for model
	SaasTropModel saasTropModel;
	NeillTropModel neilTropModel;

	saasTropModel.setDayOfYear(19);
	saasTropModel.setReceiverHeight(185);
	saasTropModel.setReceiverLatitude(47.48);
	saasTropModel.setReceiverLongitude(19.05);
	saasTropModel.setWeather(18,1013.25,50);

	// Pointer to one of the two available tropospheric models. It points
	// to the void model by default
	TropModel *tropModelPtr2gpstk = &noTropModel;
	TropModel *tropModelPtr2EGNOS = &noTropModel;

	EGNOS::EGNOS_UTILITY::SimpleNavSolver egnosNavSolver;
	EGNOS::EGNOSIonoCorrectionModel egnosIonoModel;
	EGNOS::IonoModel *ionoModel = NULL; //&egnosIonoModel;
	egnosNavSolver.elevetionMask = 0;

	if (ionoModel != NULL) {
		egnosIonoModel.updateIntervalinSeconds = 60;
		egnosIonoModel.load(EMSData);
	}
	
	std::cout << "IonoModel is loaded "<< std::endl;
	gpstk::FFTextStream debugStrm;

	//debugStrm.open()
	try
	{
		// Read nav file and store unique list of ephemerides
		//Rinex3NavStream rnffs(argv[2].c_str());    // Open ephemerides data file
		Rinex3NavStream rnffs(ephData.c_str());    // Open ephemerides data file
		Rinex3NavData rne;
		Rinex3NavHeader hdr;

		// Let's read the header (may be skipped)
		rnffs >> hdr;

		// Storing the ephemeris in "bcstore"
		while (rnffs >> rne) { 
			try { 
				bcestore.addEphemeris(rne); 
			}
			catch(Exception& e){
				cerr << e;
			}
		}

		// Setting the criteria for looking up ephemeris
		bcestore.SearchNear();
		egnosNavSolver.set_bcestore(bcestore);

		   // Open and read the observation file one epoch at a time.
		   // For each epoch, compute and print a position solution

		   //Rinex3ObsStream roffs(argv[1]);    // Open observations data file
		Rinex3ObsStream roffs(obsData);    // Open observations data file

		// In order to throw exceptions, it is necessary to set the failbit
		roffs.exceptions(ios::failbit);

		Rinex3ObsHeader roh;
		Rinex3ObsData rod;

		// Let's read the header
		roffs >> roh;

		// The following lines fetch the corresponding indexes for some
		// observation types we are interested in. Given that old-style
		// observation types are used, GPS is assumed.
		int indexP1;
		try
		{
			indexP1 = roh.getObsIndex("C1");
		}
		catch (...)
		{
			cerr << "The observation file doesn't have P1 pseudoranges." << endl;
			exit(1);
		}

		// Let's process one line of observation data

		while (roffs >> rod) // if you would you like to read all the line use this.
		//roffs >> rod;
		{
			// Apply editing criteria
			if (rod.epochFlag == 0 || rod.epochFlag == 1)  // Begin usable data
			{
				vector<SatID> prnVec;
				vector<double> rangeVec;

				// Define the "it" iterator to visit the observations PRN map. 
				// Rinex3ObsData::DataMap is a map from RinexSatID to
				// vector<RinexDatum>:
				//      std::map<RinexSatID, vector<RinexDatum> >
				Rinex3ObsData::DataMap::const_iterator it;

				// This part gets the PRN numbers and ionosphere-corrected
				// pseudoranges for the current epoch. They are correspondly fed
				// into "prnVec" and "rangeVec"; "obs" is a public attribute of
				// Rinex3ObsData to get the map of observations
				for (it = rod.obs.begin(); it != rod.obs.end(); it++)
				{

					// The RINEX file may have P1 observations, but the current
					// satellite may not have them.
					double P1(0.0);
					try
					{
						P1 = rod.getObs((*it).first, indexP1).data;
					}
					catch (...)
					{
						// Ignore this satellite if P1 is not found
						continue;
					}

					// Now, we include the current PRN number in the first part
					// of "it" iterator into the vector holding the satellites.
					// All satellites in view at this epoch that have P1 or P1+P2
					// observations will be included.
					prnVec.push_back((*it).first);

					// The same is done for the vector of doubles holding the
					// corrected ranges
					rangeVec.push_back(P1);

					// WARNING: Please note that so far no further correction
					// is done on data: Relativistic effects, tropospheric
					// correction, instrumental delays, etc.

				}  // End of 'for( it = rod.obs.begin(); it!= rod.obs.end(); ...'

				   // The default constructor for PRSolution2 objects (like
				   // "raimSolver") is to set a RMSLimit of 6.5. We change that
				   // here. With this value of 3e6 the solution will have a lot
				   // more dispersion.
				raimSolver.RMSLimit = 3e6;

				// Simple solution without RAIM
				raimSolver.NSatsReject = 0;

				// In order to compute positions we need the current time, the
				// vector of visible satellites, the vector of corresponding
				// ranges, the object containing satellite ephemerides, and a
				// pointer to the tropospheric model to be applied
				raimSolver.RAIMCompute(	rod.time,
										prnVec,
										rangeVec,
										bcestore,
										tropModelPtr2gpstk);
				
				// Note: Given that the default constructor sets public
				// attribute "Algebraic" to FALSE, a linearized least squares
				// algorithm will be used to get the solutions.
				// Also, the default constructor sets ResidualCriterion to true,
				// so the rejection criterion is based on RMS residual of fit,
				// instead of RMS distance from an a priori position.

				// If we got a valid solution, let's print it

				if (raimSolver.isValid())
				{
					// Vector "Solution" holds the coordinates, expressed in
					// meters in an Earth Centered, Earth Fixed (ECEF) reference
					// frame. The order is x, y, z  (as all ECEF objects)
					cerr << setprecision(12) << raimSolver.Solution[0] << " ";
					cerr << raimSolver.Solution[1] << " ";
					cerr << raimSolver.Solution[2];
					cerr << endl;
					
					RTKPOST_Parser::RTKPOST_Pos_Data data_from_gpstk;
					gpstk::Position posSolutionGPSTK = egnosNavSolver.transform2CommonPositionFormat(raimSolver.Solution[0], raimSolver.Solution[1], raimSolver.Solution[3]);
					Eigen::MatrixXd Cov_enu = egnosNavSolver.transformCovEcef2CovEnu(posSolutionGPSTK.geodeticLatitude(), posSolutionGPSTK.longitude(),raimSolver.Covariance);

					data_from_gpstk = egnosNavSolver.createRTKPOST_data(raimSolver.Solution[0],	
																		raimSolver.Solution[1], 
																		raimSolver.Solution[2], 
																		raimSolver.Nsvs, Cov_enu, rod.time);
					navData_GPSTK.push_back(data_from_gpstk);

				}  // End of 'if( raimSolver.isValid() )'
				else {
					cerr << "Invalid result - by gpstk" << endl;
				}

				////////////////////////////////
				// Simple navigation solution //
				////////////////////////////////

				// Calculate position
				if (egnosNavSolver.calculatePosition(rod.time, prnVec, rangeVec, tropModelPtr2EGNOS, ionoModel)) {
					// Print result
					egnosNavSolver.print_Result();
					navData_RTKPOST.push_back(egnosNavSolver.getRTKPOST_data());
				}
				else {
					cerr << "Invalid result - by EGNOS NavEngine" << endl;
				}

			} // End of 'if( rod.epochFlag == 0 || rod.epochFlag == 1 )'

		}  // End of 'while( roffs >> rod )'
		

		// Write NavEngine solution to file in rtkpost format
		RTKPOST_Parser::RTKPOST_Pos_Header rtkPost_header_navEngine;
		rtkPost_header_navEngine.datum = gpstk::ReferenceFrame::WGS84;
		rtkPost_header_navEngine.elevAngle = egnosNavSolver.elevetionMask;
		rtkPost_header_navEngine.ephemerisOpt = "broadcasted";
		rtkPost_header_navEngine.inpFiles.push_back(obsData);
		rtkPost_header_navEngine.inpFiles.push_back(ephData);
		rtkPost_header_navEngine.ionosOpt = "ems";
		rtkPost_header_navEngine.posMode = "single";
		rtkPost_header_navEngine.obsStart = navData_RTKPOST[0].dataTime;
		rtkPost_header_navEngine.obsEnd = navData_RTKPOST.back().dataTime;
		rtkPost_header_navEngine.programInfo = "GINA 1.0";
		rtkPost_header_navEngine.timeSys = gpstk::TimeSystem::GPS;
		rtkPost_header_navEngine.tropoOpt = tropModelPtr2EGNOS->name();
	
		RTKPOST_Parser::RTKPOST_Pos_Stream strm_out_navEngine(rtkpost_out_navEngine.c_str(), std::ios::out);
		strm_out_navEngine << rtkPost_header_navEngine;

		for (size_t i = 0; i < navData_RTKPOST.size(); i++)	{

				strm_out_navEngine << navData_RTKPOST[i];
		}

		strm_out_navEngine.close();
		////////////////////////////////////

		// Write GPSTK solution to file in rtkpost format
		RTKPOST_Parser::RTKPOST_Pos_Header rtkPost_header_gpstk;
		rtkPost_header_gpstk.datum = gpstk::ReferenceFrame::WGS84;
		rtkPost_header_gpstk.elevAngle = egnosNavSolver.elevetionMask;
		rtkPost_header_gpstk.ephemerisOpt = "broadcasted";
		rtkPost_header_gpstk.inpFiles.push_back(obsData);
		rtkPost_header_gpstk.inpFiles.push_back(ephData);
		rtkPost_header_gpstk.ionosOpt = "ems";
		rtkPost_header_gpstk.posMode = "single";
		rtkPost_header_gpstk.obsStart = navData_GPSTK[0].dataTime;
		rtkPost_header_gpstk.obsEnd = navData_GPSTK.back().dataTime;
		rtkPost_header_gpstk.programInfo = "GINA 1.0 gpstk's raw solution";
		rtkPost_header_gpstk.timeSys = gpstk::TimeSystem::GPS;
		rtkPost_header_gpstk.tropoOpt = tropModelPtr2gpstk->name();

		RTKPOST_Parser::RTKPOST_Pos_Stream strm_out_gpstk(rtkpost_out_gpstk.c_str(), std::ios::out);
		strm_out_gpstk << rtkPost_header_gpstk;

		for (size_t i = 0; i < navData_GPSTK.size(); i++) {

			strm_out_gpstk << navData_GPSTK[i];
		}

		strm_out_gpstk.close();

		////////////////////////////////////

	}
	catch (Exception& e)
	{
		cerr << e << endl;
	}
	catch (...)
	{
		cerr << "Caught an unexpected exception." << endl;
	}

	exit(0);

}
