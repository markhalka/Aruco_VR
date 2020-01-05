
#include "aruco_calibration.h"
#include "aruco.h"
#include "calibrator.h"
#include <fstream>
#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sstream>

#if  CV_MAJOR_VERSION >= 4
#define CV_FOURCC(a,b,c,d) VideoWriter::fourcc(a,b,c,d)
#define CV_CAP_PROP_FOURCC cv::CAP_PROP_FOURCC
#define CV_CAP_PROP_AUTOFOCUS cv::CAP_PROP_AUTOFOCUS
#endif
using namespace std;
using namespace cv;
using namespace aruco;
#ifndef CV_CAP_PROP_AUTOFOCUS
#define CV_CAP_PROP_AUTOFOCUS -1
#endif

int aruco_calibration::run(string video, float size, string baseName)
{
	 float TheMarkerSize;
	 cv::VideoCapture TheVideoCapturer;
	 cv::Mat TheInputImage, TheInputImageCopy;
	 aruco::CameraParameters TheCameraParameters;
	 aruco::MarkerDetector TheMarkerDetector;
	 std::vector<std::vector<aruco::Marker> >  allMarkers;
	 std::string TheOutCameraParams;
	 aruco::CameraParameters camp;  // camera parameters estimated
	 aruco::Calibrator calibrator;
	try
	{
		bool isLive = false;

		TheVideoCapturer.open(video);
		TheVideoCapturer.set(CV_CAP_PROP_AUTOFOCUS, 0);
		//	TheVideoCapturer.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));

		isLive = true;

		if (!TheVideoCapturer.isOpened())
		{
			cerr << "Could not open video" << endl;
			return -1;
		}



		//configure the calibrator
		calibrator.setParams(TheInputImage.size(), size);

		// set specific parameters for this configuration
		TheMarkerDetector.setDictionary("ARUCO_MIP_36h12");
		TheMarkerDetector.setDetectionMode(aruco::DM_NORMAL); //this could also be fast
		cv::namedWindow("in", cv::WINDOW_NORMAL);
		cv::resizeWindow("in", 800, 600);

		char key = 0;
		int waitKeyTime = isLive ? 10 : -10;

		do
		{
			TheVideoCapturer.retrieve(TheInputImage);  
													
			vector<aruco::Marker> detected_markers = TheMarkerDetector.detect(TheInputImage);
			// print markers from the board
			for (auto m : detected_markers)
				m.draw(TheInputImage, Scalar(0, 0, 255), 1);

			// draw help
			cv::imshow("in", TheInputImage);

			key = cv::waitKey(waitKeyTime);   // wait for key to be pressed
			if (key == 'a') {
				calibrator.addView(detected_markers);
			}
			// set waitTime in start/stop mode
			if (key == 's') waitKeyTime *= -1;

		} while (key != 27 && TheVideoCapturer.grab());


		aruco::CameraParameters camp;
		if (calibrator.getCalibrationResults(camp)) {
			camp.saveToFile(baseName);
			cout << "results saved to " << baseName << endl;
		}
		else cerr << "Could not obtain calibration" << endl;
		cout << "Final error=" << calibrator.getReprjError() << endl;
	}
	catch (std::exception& ex)

	{
		cout << "Exception :" << ex.what() << endl;
	}
	return 0;
}

