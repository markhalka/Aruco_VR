#pragma once
#include <opencv2\opencv.hpp>

//kalman is not static because there will be 2 instances for the phone and controller
class Kalman
{
public:
	cv::Mat rotation_mat = cv::Mat(3, 3, CV_64FC1);
	cv::Mat translation_estimated = cv::Mat(3, 1, CV_64FC1);
	cv::Mat rotation_estimated = cv::Mat(3, 1, CV_64FC1);
	cv::Mat inputControl = cv::Mat(6, 1, CV_64FC1);
	cv::Mat measurements = cv::Mat(6, 1, CV_64FC1);
	cv::Mat camMatrix = cv::Mat(3, 3, CV_64FC1);
	cv::Mat distCoeffs = cv::Mat(1, 5, CV_64FC1);
	cv::VideoCapture inputVideo = cv::VideoCapture();
	cv::KalmanFilter KF = cv::KalmanFilter();

	void updateKalmanFilter(cv::KalmanFilter &KF, cv::Mat &measurement,
		cv::Mat &translation_estimated, cv::Mat &rotation_estimated);

	 void IMUMeasurments(cv::Mat &measurements,
		const cv::Mat &translation_measured, const cv::Mat &rotation_measured);

	 void initKalmanFilter(cv::KalmanFilter &KF, int nStates, int nMeasurements, int nInputs, double dt);
	 bool readCameraParameters(std::string filename, cv::Mat &camMatrix, cv::Mat &distCoeffs);
	 void predictKalmanFilter(cv::KalmanFilter &KF, cv::Mat &controlInput);
	 int initPosRot(double inputDT);

	 void fillMeasurements(cv::Mat &measurements,
		const cv::Mat &translation_measured, const cv::Mat &rotation_measured);
	
	 void kalmanIMU(cv::Mat tvec, cv::Mat rvec);
	 void kalmanPose(cv::Mat tvec, cv::Mat rvec);
	
};

