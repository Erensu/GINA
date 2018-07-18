
#include "SecondDGNSS_code.h"
#include "utility.h"








namespace secondDGNSS_code {

#define MAXSATNUMBER 15
#define CONVERGENCE_LIMIT 1E-5

	using namespace utility_secondDGNSS_code;

	static int WNRover, WNBase;
	static double ToWRover, ToWBase;
	static int satIdRover[MAXSATNUMBER] = { -1 };
	static int satIdBase[MAXSATNUMBER] = { -1 };
	static double prRover[MAXSATNUMBER] = { -1 };
	static double prBase[MAXSATNUMBER] = { -1 };
	static int vectorSizeRover = 0;
	static int vectorSizeBase = 0;

	static double basePosition[3];

	static const double wie_e = 7.292115e-5;
	static const double c_mps = 299792458;


	static double roverPos[3] = { 0, 0, 0 };
	//static Matrix3d designMatrix;

	void get_Result(double sol[]) {

		sol[0] = roverPos[0];
		sol[1] = roverPos[1];
		sol[2] = roverPos[2];
	}

	void print_Result(void) {

		cout << "" << roverPos[0] << " ";
		cout << " " << roverPos[1] << " ";
		cout << " " << roverPos[2] << "	";
	}

	void get_Pos_Result(double sol[3]) {

		sol[0] = roverPos[0];
		sol[1] = roverPos[1];
		sol[2] = roverPos[2];
	}

	void set_position_of_Base(double pos[3]) {

		basePosition[0] = pos[0];
		basePosition[1] = pos[1];
		basePosition[2] = pos[2];
	}

	void set_position_of_Rover(double pos[3]) {

		roverPos[0] = pos[0];
		roverPos[1] = pos[1];
		roverPos[2] = pos[2];

		/*roverPos[0] = -2500946.0962;
		roverPos[1] = -4670472.9515;
		roverPos[2] = 3539500.5469;*/

	}

	void set_time_Rover(int wn, double tow) {
		WNRover = wn;
		ToWRover = tow;
	}

	void set_time_Base(int wn, double tow) {
		WNBase = wn;
		ToWBase = tow;
	}

	void set_satId_Rover(int* id, int size) {

		for (int i = 0; i < size; i++) {

			if (i >= MAXSATNUMBER) {
				break;
			}

			satIdRover[i] = id[i];
		}

		vectorSizeRover = size;
	}

	void set_satId_Base(int* id, int size) {

		for (int i = 0; i < size; i++) {

			if (i >= MAXSATNUMBER) {
				break;
			}

			satIdBase[i] = id[i];
		}

		vectorSizeBase = size;
	}

	void set_pseudoRange_Rover(double* psoudoRange, int size) {

		for (int i = 0; i < size; i++) {

			if (i >= MAXSATNUMBER) {
				break;
			}

			prRover[i] = psoudoRange[i];
		}

		vectorSizeRover = size;
	}

void set_pseudoRange_Base(double* psoudoRange, int size) {

	for (int i = 0; i < size; i++) {

		if (i >= MAXSATNUMBER) {
			break;
		}

		prBase[i] = psoudoRange[i];
	}

	vectorSizeBase = size;
}

void calculatePosition(void) {

	while (updatePosition() > CONVERGENCE_LIMIT);
	reset();
}

static double updatePosition(void) {

	double reference_observation;
	double observation;
	int rowIndexofMatrixEquation = 0;


	vector<int> SatIdv;
	vector<double> prvRover;
	vector<double> prvBase;
	int index_of_referenceSatId;
	int referenceSatId;

	if (!setCommonSatIdandPRNvector(SatIdv, prvRover, prvBase)) {
		cout <<"Error in the setCommonSatIdandPRNvector fcn" << endl;
		throw("Error in the setCommonSatIdandPRNvector fcn");
	}

	referenceSatId = getSatId_With_MaxElevation(SatIdv);
	
	MatrixXd covMatrix = MatrixXd::Zero(3, 3);
	MatrixXd designMatrix(SatIdv.size() - 1, 3);
	VectorXd y(SatIdv.size() - 1);
	VectorXd x(3);
	VectorXd best_row_of_the_first_Differenced_DesignMatrix = VectorXd::Zero(3);
	Vector3d row_ofDesignMatrix = VectorXd::Zero(3);
	Vector3d reference_row_ofDesignMatrix = VectorXd::Zero(3);
	
	for (int i = 0; i < SatIdv.size(); i++) {

		if (i == referenceSatId) {
			// Get the observation of the best Sat and design matrix row of the best Sat
			getOneRowofTheMatrixEquation(reference_row_ofDesignMatrix, reference_observation, prvRover[i], prvBase[i], SatIdv[i]);

			continue;
		}

		getOneRowofTheMatrixEquation(row_ofDesignMatrix, observation, prvRover[i], prvBase[i], SatIdv[i]);
		

		

		// Set up design matrix
		designMatrix(rowIndexofMatrixEquation, 0) = row_ofDesignMatrix(0);
		designMatrix(rowIndexofMatrixEquation, 1) = row_ofDesignMatrix(1);
		designMatrix(rowIndexofMatrixEquation, 2) = row_ofDesignMatrix(2);

		y(rowIndexofMatrixEquation) = observation;
		rowIndexofMatrixEquation++;
	}

	// Set up observetion vector 
	makefromFirstDiff2SecondDifferenceObservationVector(reference_observation, y);

	// Set up design matrix 
	makefromFirstDiff2SecondDifferenceDesignMatrix(best_row_of_the_first_Differenced_DesignMatrix, designMatrix);
		
	// Create covariance matrix
	covMatrix = designMatrix.transpose() * designMatrix;
	covMatrix = covMatrix.inverse();

	// Multiply Covariance matrix with A.' and PR residual matrix
	x = covMatrix * designMatrix.transpose() * y;

	// Update rover position and clock bias
	roverPos[0] = roverPos[0] + x(0);
	roverPos[1] = roverPos[1] + x(1);
	roverPos[2] = roverPos[2] + x(2);

	double res_norm = Norm(x);
	return res_norm;
}

	static int getSatId_With_MaxElevation(vector<int> &SatIdv) {
		double elevation, maxelevation;
		maxelevation = 0;
		int BestId = 0;
		double temp_satPos[3];

		for (size_t i = 0; i < SatIdv.size(); i++) {
			elevation = get_satPos(roverPos, WNRover, ToWRover, SatIdv[i], temp_satPos);
				if (elevation > maxelevation){
					maxelevation = elevation;
					BestId = i;
				}
			}

		return BestId;

	}

	static void makefromFirstDiff2SecondDifferenceObservationVector(double reference_observation, Eigen::VectorXd& observation_vector) {
	
		for (size_t i = 0; i < observation_vector.size(); i++) {

			observation_vector(i) -= reference_observation;
		}
	}

	static void makefromFirstDiff2SecondDifferenceDesignMatrix(Eigen::VectorXd& reference_row_of_the_first_Differenced_DesignMatrix, Eigen::MatrixXd designMatrix) {

		for (size_t i = 0; i < designMatrix.rows(); i++) {

			designMatrix(i, 0) -= reference_row_of_the_first_Differenced_DesignMatrix(0);
			designMatrix(i, 1) -= reference_row_of_the_first_Differenced_DesignMatrix(1);
			designMatrix(i, 2) -= reference_row_of_the_first_Differenced_DesignMatrix(2);
		}
	}

	static void getOneRowofTheMatrixEquation(Eigen::Vector3d& row_of_designMatrix, double& observation, double prvRover, double prvBase, int SatIdv) {

		double rhoRover, rhoBase;
		
		// Calculate Geometry Distance
		double satPos[3];
		calculateGeometryDistance(WNRover, ToWRover, SatIdv, roverPos, rhoRover, satPos);
		calculateGeometryDistance(WNBase, ToWBase, SatIdv, basePosition, rhoBase, satPos);
	
		row_of_designMatrix(0) = (roverPos[0] - satPos[0]) / rhoRover;
		row_of_designMatrix(1) = (roverPos[1] - satPos[1]) / rhoRover;
		row_of_designMatrix(2) = (roverPos[2] - satPos[2]) / rhoRover;

		observation = prvRover - prvBase - rhoRover + rhoBase;
	}

	static void reset(void) {

		for (int i = 0; i < MAXSATNUMBER; i++) {
			satIdRover[i] = 0;
			prRover[i] = 0;
			vectorSizeRover = 0;

			satIdBase[i] = 0;
			prBase[i] = 0;
			vectorSizeBase = 0;
		}
	}

	static int getNumberofVisibleSatelites(int WN, int ToW) {

		double temp_satPos[3];

		int numberofSats = 0;
		for (int Id = 0; Id < 33; Id++) {
			if (get_satPos(WN, ToW, Id, temp_satPos)) {
				numberofSats++;
			}
		}

		return numberofSats;
	}

	static void correctwSagnacEffect(double deltat, double* old_pos, double* new_pos) {

		Matrix3d rotMatrix = Matrix3d::Zero();

		rotMatrix << cos(wie_e*deltat), -sin(wie_e*deltat), 0,
			sin(wie_e*deltat), cos(wie_e*deltat), 0,
			0, 0, 1;

		Vector3d posV;
		posV << old_pos[0], old_pos[1], old_pos[2];

		posV = rotMatrix*posV;

		new_pos[0] = posV(0);
		new_pos[1] = posV(1);
		new_pos[2] = posV(2);

	}

	static double calculateDistance(double* rover, double* sat) {

		double dist = 0;
		for (int a = 0; a < 3; a++) {
			dist += pow((rover[a] - sat[a]), 2);
		}

		return sqrt(dist);
	}

	static double Norm(double* x) {

		return sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
	}

	static double Norm(VectorXd x) {

		return sqrt(x(0) * x(0) + x(1) * x(1) + x(2) * x(2));
	}

	static bool setCommonSatIdandPRNvector(vector<int> &SatIdv, vector<double> &prvRover, vector<double> &prvBase) {

		double temp_satPos[3];
		double tempToW;
		int tempWN;

		checkSatValidity(WNRover, ToWRover, satIdRover);
		checkSatValidity(WNBase, ToWBase, satIdBase);

		for (size_t itRover = 0; itRover < vectorSizeRover; itRover++) {
			for (size_t itBase = 0; itBase < vectorSizeBase; itBase++) {
				if (satIdRover[itRover] == satIdBase[itBase] && satIdRover[itRover] > 0) {
					SatIdv.push_back(satIdRover[itRover]);
					prvRover.push_back(prRover[itRover]);
					prvBase.push_back(prBase[itBase]);
				}
			}
		}

		if (SatIdv.size() == SatIdv.size() == SatIdv.size() && SatIdv.size() >= 4) {
			return true;
		}
		else { 
			return false; 
		}
	}

	static void addTime2ToW_WeekRollOverChecked(int &WN, const double t_in, double &t_out, const double deltaT) {

		double week = 604800;

		if (deltaT > week || deltaT < -week) {
			throw("In checkForGpsWeekRollOver the delta time is greater than 604800 or less than -604800. The function cannot handle this case. This function shall be updated to handle this case.");
		}

		t_out = t_in + deltaT;

		if (t_out > week) {
			t_out -= week;
			WN += 1;
		}

		if (t_out < 0) {
			t_out += week;
			WN -= 1;
		}

	}

	static void checkSatValidity(int WN, double ToW, int satId[]) {

		double temp_satPos[3];
		double tempToW;
		int tempWN;
		int vectorSize;
		double maxTransmissionTime = 0.086;

		vectorSize = sizeof(satId) / sizeof(int);

		for (size_t i = 0; i < vectorSize; i++) {

			if (satId[i] < 1) {
				continue;
			}

			// Get i. satelite position at given time
			tempToW = ToW;
			tempWN = WN;
			addTime2ToW_WeekRollOverChecked(tempWN, ToW, tempToW, -maxTransmissionTime);

			if (get_satPos(tempWN, tempToW, satId[i], temp_satPos) == 0) {
				satId[i] = 0;
			}
		}

	}

	static void calculateGeometryDistance(int WN, double ToW, int SatId, double Pos[], double &geometryDistance, double satPos[]) {

		double temp_iter_satPos[3];
		double time_of_transmission, time_of_arrival;
		double travelTime = 0.0;
		double temp_dist;

		int WN_TransmissionTime;
		double ToW_TransmissionTime;

		// Get i. satelite position at given time
		if (get_satPos(WN, ToW, SatId, satPos) == 0) {
			throw("After satellite sanity check this error shall never happen. \n WNRover: %lf \n ToWRover: %lf \n SatId: %d", WN, ToW, SatId);
		}

		temp_iter_satPos[0] = satPos[0];
		temp_iter_satPos[1] = satPos[1];
		temp_iter_satPos[2] = satPos[2];

		time_of_arrival = ToW;
		time_of_transmission = ToW;
		WN_TransmissionTime = WN;

		temp_dist = calculateDistance(Pos, temp_iter_satPos);

		travelTime = temp_dist / c_mps;

		get_satPos(WN, time_of_arrival, SatId, satPos);

		addTime2ToW_WeekRollOverChecked(WN_TransmissionTime, time_of_arrival, time_of_transmission, -travelTime);	//time_of_transmission = time_of_arrival - travelTime;

		get_satPos(WN_TransmissionTime, time_of_transmission, SatId, satPos);

		correctwSagnacEffect(time_of_transmission - time_of_arrival, satPos, temp_iter_satPos);

		satPos[0] = temp_iter_satPos[0];
		satPos[1] = temp_iter_satPos[1];
		satPos[2] = temp_iter_satPos[2];

		// Geometry distance
		geometryDistance = calculateDistance(Pos, satPos);
		ToW_TransmissionTime = time_of_transmission;
	}

}