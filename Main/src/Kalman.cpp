#include "Kalman.h"
#include "Utils.h"



	void Kalman::updateKalmanFilter(cv::KalmanFilter &KF, cv::Mat &measurement,
		cv::Mat &translation_estimated, cv::Mat &rotation_estimated)
	{

		cv::Mat estimated = KF.correct(measurement);

		// Estimated translation
		translation_estimated.at<double>(0) = estimated.at<double>(0);
		translation_estimated.at<double>(1) = estimated.at<double>(1);
		translation_estimated.at<double>(2) = estimated.at<double>(2);
	
		// Estimated euler angles
		rotation_estimated.at<double>(0) = estimated.at<double>(6);
		rotation_estimated.at<double>(1) = estimated.at<double>(7);
		rotation_estimated.at<double>(2) = estimated.at<double>(8);
	}


	void Kalman::IMUMeasurments(cv::Mat &measurements,
		const cv::Mat &translation_measured, const cv::Mat &rotation_measured)
	{
		// Convert rotation matrix to euler angles
		cv::Mat measured_eulers(3, 1, CV_64F);

		measured_eulers = Utils::rot2euler(rotation_measured);

		// Set measurement to predict
		measurements.at<double>(0) = translation_measured.at<double>(0); // x
		measurements.at<double>(1) = translation_measured.at<double>(1); // y
		measurements.at<double>(2) = translation_measured.at<double>(2); // z
		measurements.at<double>(3) = measured_eulers.at<double>(0);      // roll
		measurements.at<double>(4) = measured_eulers.at<double>(1);      // pitch
		measurements.at<double>(5) = measured_eulers.at<double>(2);      // yaw
	}


	void Kalman::initKalmanFilter(cv::KalmanFilter &KF, int nStates, int nMeasurements, int nInputs, double dt)
	{

		KF.init(nStates, nMeasurements, nInputs, CV_64F);                 // init Kalman Filter

		cv::setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-2));       // set process noise
		cv::setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-3));   // set measurement noise
		cv::setIdentity(KF.errorCovPost, cv::Scalar::all(1));             // error covariance

																		  /** DYNAMIC MODEL **/

																		  //  [1 0 0 dt  0  0 dt2   0   0 0 0 0  0  0  0   0   0   0]
																		  //  [0 1 0  0 dt  0   0 dt2   0 0 0 0  0  0  0   0   0   0]
																		  //  [0 0 1  0  0 dt   0   0 dt2 0 0 0  0  0  0   0   0   0]
																		  //  [0 0 0  1  0  0  dt   0   0 0 0 0  0  0  0   0   0   0]
																		  //  [0 0 0  0  1  0   0  dt   0 0 0 0  0  0  0   0   0   0]
																		  //  [0 0 0  0  0  1   0   0  dt 0 0 0  0  0  0   0   0   0]
																		  //  [0 0 0  0  0  0   1   0   0 0 0 0  0  0  0   0   0   0]
																		  //  [0 0 0  0  0  0   0   1   0 0 0 0  0  0  0   0   0   0]
																		  //  [0 0 0  0  0  0   0   0   1 0 0 0  0  0  0   0   0   0]
																		  //  [0 0 0  0  0  0   0   0   0 1 0 0 dt  0  0 dt2   0   0]
																		  //  [0 0 0  0  0  0   0   0   0 0 1 0  0 dt  0   0 dt2   0]
																		  //  [0 0 0  0  0  0   0   0   0 0 0 1  0  0 dt   0   0 dt2]
																		  //  [0 0 0  0  0  0   0   0   0 0 0 0  1  0  0  dt   0   0]
																		  //  [0 0 0  0  0  0   0   0   0 0 0 0  0  1  0   0  dt   0]
																		  //  [0 0 0  0  0  0   0   0   0 0 0 0  0  0  1   0   0  dt]
																		  //  [0 0 0  0  0  0   0   0   0 0 0 0  0  0  0   1   0   0]
																		  //  [0 0 0  0  0  0   0   0   0 0 0 0  0  0  0   0   1   0]
																		  //  [0 0 0  0  0  0   0   0   0 0 0 0  0  0  0   0   0   1]

		KF.transitionMatrix.at<double>(0, 3) = dt;
		KF.transitionMatrix.at<double>(1, 4) = dt;
		KF.transitionMatrix.at<double>(2, 5) = dt;

		// orientation
		KF.transitionMatrix.at<double>(6, 9) = dt;
		KF.transitionMatrix.at<double>(7, 10) = dt;
		KF.transitionMatrix.at<double>(8, 11) = dt;
		KF.transitionMatrix.at<double>(9, 9) = 0;
		KF.transitionMatrix.at<double>(10, 10) = 0;
		KF.transitionMatrix.at<double>(11, 11) = 0;


		/** MEASUREMENT MODEL **/

		//  [1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]
		//  [0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]
		//  [0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]
		//  [0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0]
		//  [0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0]
		//  [0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0]

		/*new measurment model*/



		KF.measurementMatrix.at<double>(0, 0) = 1;  // x
		KF.measurementMatrix.at<double>(1, 1) = 1;  // y
		KF.measurementMatrix.at<double>(2, 2) = 1;  // z
													
		KF.measurementMatrix.at<double>(3, 6) = 1;  // roll
		KF.measurementMatrix.at<double>(4, 7) = 1; // pitch
		KF.measurementMatrix.at<double>(5, 8) = 1; // yaw
												   //ax, ay, az
												   //wx, wy, wz

												   /** CONTROL MODEL **/
												   // [dt2 0 0]
												   // [0 dt2 0]
												   // [0 0 dt2]
												   // [dt 0 0]
												   // [0 dt 0]
												   // [0 0 dt]
												   // [1 0 0]
												   // [0 1 0]
												   // [0 0 1]
												   // [0 0 0]
												   // [0 0 0]
												   // [0 0 0]
												   // [0 0 0]
												   // [0 0 0]
												   // [0 0 0]
												   // [0 0 0]
												   // [0 0 0]
												   // [0 0 0]

												   //position
		KF.controlMatrix.at<double>(0, 0) = 0.5*pow(dt, 2);
		KF.controlMatrix.at<double>(1, 1) = 0.5*pow(dt, 2);
		KF.controlMatrix.at<double>(2, 2) = 0.5*pow(dt, 2);
		KF.controlMatrix.at<double>(3, 0) = dt;
		KF.controlMatrix.at<double>(4, 1) = dt;
		KF.controlMatrix.at<double>(5, 2) = dt;

		KF.controlMatrix.at<double>(9, 3) = 1;
		KF.controlMatrix.at<double>(10, 4) = 1;
		KF.controlMatrix.at<double>(11, 5) = 1;

	}


	bool Kalman::readCameraParameters(std::string filename, cv::Mat &camMatrix, cv::Mat &distCoeffs) {
		cv::FileStorage fs(filename, cv::FileStorage::READ);
		if (!fs.isOpened())
			return false;
		fs["camera_matrix"] >> camMatrix;
		fs["distortion_coefficients"] >> distCoeffs;
		return true;
	}

	void Kalman::predictKalmanFilter(cv::KalmanFilter &KF, cv::Mat &controlInput) {
		cv::Mat prediction = KF.predict(controlInput);
	
		Kalman::translation_estimated.at<double>(0) = prediction.at<double>(0);
		Kalman::translation_estimated.at<double>(1) = prediction.at<double>(1);
		Kalman::translation_estimated.at<double>(2) = prediction.at<double>(2);

		cv::Mat eulers_estimated(3, 1, CV_64F);

		eulers_estimated.at<double>(0) = prediction.at<double>(6);
		eulers_estimated.at<double>(1) = prediction.at<double>(7);
		eulers_estimated.at<double>(2) = prediction.at<double>(8);
		
		rotation_estimated = eulers_estimated;
	}
	int Kalman::initPosRot(double inputDT) {

		Kalman::measurements.setTo(cv::Scalar(0));

		int	nStates = 12;
		int	nMeasurements = 6;
		int	nInputs = 6;

		initKalmanFilter(Kalman::KF, nStates, nMeasurements, nInputs, inputDT);

		return 1;
	}


	void Kalman::fillMeasurements(cv::Mat &measurements,
		const cv::Mat &translation_measured, const cv::Mat &rotation_measured)
	{
		// Convert rotation matrix to euler angles
		cv::Mat measured_eulers(3, 1, CV_64F);
		measured_eulers = Utils::rot2euler(rotation_measured);

		// Set measurement to predict
		measurements.at<double>(0) = translation_measured.at<double>(0); // x
		measurements.at<double>(1) = translation_measured.at<double>(1); // y
		measurements.at<double>(2) = translation_measured.at<double>(2); // z
		measurements.at<double>(3) = measured_eulers.at<double>(0);      // roll
		measurements.at<double>(4) = measured_eulers.at<double>(1);      // pitch
		measurements.at<double>(5) = measured_eulers.at<double>(2);      // yaw
	}

	//add imu data to kalman filter
	void Kalman::kalmanIMU(cv::Mat tvec, cv::Mat rvec) {

		Kalman::inputControl.at<double>(0) = tvec.at<double>(0);
		Kalman::inputControl.at<double>(1) = tvec.at<double>(1);
		Kalman::inputControl.at<double>(2) = tvec.at<double>(2);

		Kalman::inputControl.at<double>(3) = rvec.at<double>(0);
		Kalman::inputControl.at<double>(4) = rvec.at<double>(1);
		Kalman::inputControl.at<double>(5) = rvec.at<double>(2);

		predictKalmanFilter(Kalman::KF, Kalman::inputControl);
	}

	void Kalman::kalmanPose(cv::Mat tvec, cv::Mat rvec) {

		Kalman::measurements.at<double>(0) = tvec.at<double>(1); //x ,y, z??
		Kalman::measurements.at<double>(1) = tvec.at<double>(0);
		Kalman::measurements.at<double>(2) = -tvec.at<double>(2); //z is inverted??

		cv::Mat R;
		cv::Rodrigues(rvec, R);
		for (int i = 0; i < 9; i++) {
			Kalman::rotation_mat.at<double>(i) = R.at<double>(i);
		}
		
		cv::Mat eulers = Utils::rot2euler(R);
		Kalman::measurements.at<double>(3) = eulers.at<double>(0);
		Kalman::measurements.at<double>(4) = eulers.at<double>(1);
		Kalman::measurements.at<double>(5) = eulers.at<double>(2);
		updateKalmanFilter(Kalman::KF, Kalman::measurements, Kalman::translation_estimated, Kalman::rotation_estimated);
	}






