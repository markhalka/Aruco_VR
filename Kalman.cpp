#include "Kalman.h"
#include "Utils.h"



Kalman::Kalman()
{
}


Kalman::~Kalman()
{
}

	void Kalman::updateKalmanFilter(cv::KalmanFilter &KF, cv::Mat &measurement,
		cv::Mat &translation_estimated, cv::Mat &rotation_estimated)
	{

		cv::Mat estimated = KF.correct(measurement);

		// Estimated translation
		translation_estimated.at<double>(0) = estimated.at<double>(0);
		translation_estimated.at<double>(1) = estimated.at<double>(1);
		translation_estimated.at<double>(2) = estimated.at<double>(2);

	
		// Estimated euler angles
		/*cv::Mat eulers_estimated(3, 1, CV_64F);
		eulers_estimated.at<double>(0) = estimated.at<double>(6);
		eulers_estimated.at<double>(1) = estimated.at<double>(7);
		eulers_estimated.at<double>(2) = estimated.at<double>(8);

		// Convert estimated quaternion to rotation matrix
		rotation_estimated = euler2rot(eulers_estimated); */

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
		//	KF.transitionMatrix.at<double>(3, 6) = dt;
		//	KF.transitionMatrix.at<double>(4, 7) = dt;
		//	KF.transitionMatrix.at<double>(5, 8) = dt;
		//	KF.transitionMatrix.at<double>(0, 6) = 0.5*pow(dt, 2);
		//	KF.transitionMatrix.at<double>(1, 7) = 0.5*pow(dt, 2);
		//	KF.transitionMatrix.at<double>(2, 8) = 0.5*pow(dt, 2);

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
													//	KF.measurementMatrix.at<double>(0, 6) = 1;  // ax
													//	KF.measurementMatrix.at<double>(1, 7) = 1;  // ay
													//	KF.measurementMatrix.at<double>(2, 8) = 1;  // az

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
		//	KF.controlMatrix.at<double>(6, 0) = 1;
		//	KF.controlMatrix.at<double>(7, 1) = 1;
		//	KF.controlMatrix.at<double>(8, 2) = 1;

		//orientation
		//	KF.controlMatrix.at<double>(6, 3) = dt;
		//	KF.controlMatrix.at<double>(7, 4) = dt;
		//	KF.controlMatrix.at<double>(8, 5) = dt;
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

		// Convert estimated quaternion to rotation matrix
		Kalman::rotation_estimated = Utils::euler2rot(eulers_estimated);
	}
	int Kalman::initPosRot(double inputDT) {

		Kalman::measurements.setTo(cv::Scalar(0));

		/*	int markersX = 5;
		int markersY = 7;
		float markerLength = 0.16f; //check this
		float markerSeparation = 0.006f; //check this
		int dictionaryId = 4;
		bool showRejected = false;
		bool refindStrategy = false;
		int camId = 0;
		String CameraParamsFile = "C:\\Users\\hithe\\Desktop\\ArucoLibrary\\build\\bin\\Debug\\camParams.yml";
		String DetectorParamsFile = "C:/Users/hithe/Desktop/DetectParams.yml";
		Kalman::TheCameraParameters.readFromXMLFile(CameraParamsFile);
		//read and setup camera and detector parameters

		Kalman::MDetector.setDetectionMode(aruco::DM_FAST, 0.02f);
		Kalman::MDetector.setDictionary("ARUCO_MIP_36h12");// get dictionary index

		Kalman::inputVideo.open(camId);

		Kalman::inputVideo >> Kalman::TheInputImage;

		Kalman::TheCameraParameters.resize(Kalman::TheInputImage.size());
		Kalman::FDetector.setConfiguration("FRACTAL_2L_6");
		Kalman::FDetector.setParams(Kalman::TheCameraParameters, 0.106); */

		//re-write with aruco videoCapture
		//write getTransformation matrix with aruco markers
		//than test with fractal markers
		//initKalmanFilt
		int	nStates = 12;
		int	nMeasurements = 6;
		int	nInputs = 6;

		initKalmanFilter(Kalman::KF, nStates, nMeasurements, nInputs, inputDT);

		return 1;
	}

	void Kalman::getParams(double* fparams, double* cparams, double* fov) {
		double returnFparams[2] = { Kalman::camMatrix.at<double>(0,0), Kalman::camMatrix.at<double>(1,1) };
		double returnCparams[2] = { Kalman::camMatrix.at <double>(0,2), Kalman::camMatrix.at<double>(1,2) };
		fparams[0] = returnFparams[0];
		fparams[1] = returnFparams[1];
		cparams[0] = returnCparams[0];
		cparams[1] = returnCparams[1];
		cv::Size size = Kalman::TheInputImage.size();//this might be wrong
		double focalLength, aspectRatio;
		cv::Point2d principlePoint;
		calibrationMatrixValues(Kalman::camMatrix, size, 0.004, 0.004, fov[0], fov[1], focalLength, principlePoint, aspectRatio);
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
	void Kalman::kalmanIMU(double* acc, double* gyro) {
		double dt = 0.125;

		//update control matrix here
		/*Kalman::measurements.at<double>(0) = acc[0];
		Kalman::measurements.at<double>(1) = acc[1];
		Kalman::measurements.at<double>(2) = acc[2];

		Kalman::measurements.at<double>(3) = gyro[0];
		Kalman::measurements.at<double>(4) = gyro[1];
		Kalman::measurements.at<double>(5) = gyro[2];*/
		//	Kalman::measurements.at<double>(6) = Kalman::KF.statePre.at<double>(0) + Kalman::KF.statePre.at<double>(3)*dt + Kalman::KF.statePre.at<double>(6)*0.5*dt*dt;
		Kalman::inputControl.at<double>(0) = acc[0];
		Kalman::inputControl.at<double>(1) = acc[1];
		Kalman::inputControl.at<double>(2) = acc[2];

		Kalman::inputControl.at<double>(3) = gyro[0];
		Kalman::inputControl.at<double>(4) = gyro[1];
		Kalman::inputControl.at<double>(5) = gyro[2];

		predictKalmanFilter(Kalman::KF, Kalman::inputControl);

		//updateKalmanFilter(Kalman::KF, Kalman::measurements, Kalman::translation_estimated, Kalman::rotation_estimated);

	}


	void Kalman::kalmanIMU(double* acc) {
		Kalman::inputControl.at<double>(0) = acc[0];
		Kalman::inputControl.at<double>(1) = acc[1];
		Kalman::inputControl.at<double>(2) = acc[2];

		predictKalmanFilter(Kalman::KF, Kalman::inputControl);
	}

	//add opencv image data to kalman filter

	void Kalman::kalmanPose(double* tvec, double* rvec) {

		Kalman::measurements.at<double>(0) = tvec[0];
		Kalman::measurements.at<double>(1) = tvec[1];
		Kalman::measurements.at<double>(2) = tvec[2];


		Kalman::measurements.at<double>(3) = rvec[0];
		Kalman::measurements.at<double>(4) = rvec[1];
		Kalman::measurements.at<double>(5) = rvec[2];

		updateKalmanFilter(Kalman::KF, Kalman::measurements, Kalman::translation_estimated, Kalman::rotation_estimated);

	}

	void Kalman::kalmanPose(double tvec[]) {

		Kalman::measurements.at<double>(0) = tvec[0];
		Kalman::measurements.at<double>(1) = tvec[1];
		Kalman::measurements.at<double>(2) = tvec[2];

		updateKalmanFilter(Kalman::KF, Kalman::measurements, Kalman::translation_estimated, Kalman::rotation_estimated);
	}

	//return pose from kalman filter
	void Kalman::getTransformationIMU(double* ptr) {

		cv::Mat R = Kalman::rotation_estimated;

		for (int i = 0; i < 3; i++) {
			ptr[i] = R.at<double>(0, i);
		}
		ptr[3] = Kalman::translation_estimated.at<double>(0);
		//ptr[3] = tvec[0];

		for (int i = 0; i < 3; i++) {
			ptr[i + 4] = R.at<double>(1, i);
		}
		ptr[7] = Kalman::translation_estimated.at<double>(1);
		//ptr[7] = tvec[1];

		for (int i = 0; i < 3; i++) {
			ptr[i + 8] = R.at<double>(2, i);
		}
		ptr[11] = Kalman::translation_estimated.at<double>(2);
		//	ptr[11] = tvec[2];

		for (int i = 0; i < 3; i++) {
			ptr[i + 12] = 0;
		}
		ptr[15] = 1;

	}


	//this goes in kalman?





