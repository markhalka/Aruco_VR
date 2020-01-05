
#include "aruco.h"
#include "timers.h"
#include <fstream>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sstream>
#include <string>
#include <stdexcept>
#include <Eigen/Geometry>

#include "sglviewer.h"
#include "Utils.h"
#include "aruco_test_markermap.h"
using namespace cv;
using namespace aruco;
using namespace std;
bool started = false;
VideoCapture TheVideoCapturer;
Mat TheInputImage, TheInputImageCopy;
//void savePosesToFile(string filename, const std::map<int, cv::Mat>& fmp);
mutex m;
void update() {
		//cout << "here " << endl;
	Mat temp;
	while (started) {
		TheVideoCapturer.read(temp);
		//lock with mutex here
		m.lock();
		TheInputImage = temp;
		m.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}


int aruco_test_markermap::run(string markermap, float size, string video, string camera, Kalman& kalman, Update& phoneUpdate)
{
	string TheMarkerMapConfigFile;
	bool The3DInfoAvailable = false;
	float TheMarkerSize = -1;


	CameraParameters TheCameraParameters;
	MarkerMap TheMarkerMapConfig;
	MarkerDetector TheMarkerDetector;
	MarkerMapPoseTracker TheMSPoseTracker;
	int waitTime = 1;
	std::map<int, cv::Mat> frame_pose_map;  // set of poses and the frames they were detected
	sgl_OpenCV_Viewer Viewer;

	try
	{
	
		TheMarkerMapConfig.readFromFile(markermap);

		TheMarkerMapConfigFile = markermap;
		TheMarkerSize = size;
		// read from camera or from  file
	
			TheVideoCapturer.open(0); //video
		//	TheVideoCapturer.set(cv::CAP_PROP_AUTOFOCUS, 0);
			waitTime = 1;
		
	
		if (!TheVideoCapturer.isOpened())
			throw std::runtime_error("Could not open video");

		// read first image to get the dimensions
		TheVideoCapturer >> TheInputImage;

		// read camera parameters if passed
		TheCameraParameters.readFromXMLFile(camera);
		TheCameraParameters.resize(TheInputImage.size());
		// prepare the detector

		TheMarkerDetector.setDictionary(TheMarkerMapConfig.getDictionary());

	
		// prepare the pose tracker if possible
		// if the camera parameers are avaiable, and the markerset can be expressed in meters, then go

		if (TheMarkerMapConfig.isExpressedInPixels() && TheMarkerSize > 0)
			TheMarkerMapConfig = TheMarkerMapConfig.convertToMeters(TheMarkerSize);

		cout << "TheCameraParameters.isValid()=" << TheCameraParameters.isValid() << " " << TheMarkerMapConfig.isExpressedInMeters() << endl;

		if (TheCameraParameters.isValid() && TheMarkerMapConfig.isExpressedInMeters()) {
			TheMSPoseTracker.setParams(TheCameraParameters, TheMarkerMapConfig);
			TheMarkerSize = cv::norm(TheMarkerMapConfig[0][0] - TheMarkerMapConfig[0][1]);
		}

		// Create gui

		//Viewer.setParams(1.5, 1280, 960, "map_viewer", TheMarkerSize);
		char key = 0;
		int index = 0;
		// capture until press ESC or until the end of the video
		cout << "Press 's' to start/stop video" << endl;
	
		//create thread that calls update
		started = true;
		thread updateThread(update);
		do
		{
			//TheVideoCapturer.retrieve(TheInputImage);

			//mutex lock
			m.lock();
			TheInputImage.copyTo(TheInputImageCopy);
			m.unlock();
			
			vector<aruco::Marker> detected_markers = TheMarkerDetector.detect(TheInputImageCopy);
			// estimate 3d camera pose if possible
			if (TheMSPoseTracker.isValid())
				if (TheMSPoseTracker.estimatePose(detected_markers)) {
					//frame_pose_map.insert(make_pair(index, TheMSPoseTracker.getRTMatrix()));
				//	cout << TheMSPoseTracker.getRvec() << " " << TheMSPoseTracker.getTvec() << endl;
				//	cout << TheMSPoseTracker.getRvec().at<double>(2) << endl;
			//		cout << TheMSPoseTracker.getTvec() << endl;
				m.lock();
				kalman.kalmanPose(TheMSPoseTracker.getTvec(), TheMSPoseTracker.getRvec());
		
			
			  //cout << TheMSPoseTracker.getTvec() << " " << TheMSPoseTracker.getRvec() << endl;
				Mat temp = kalman.translation_estimated;
				Mat temp2 = kalman.rotation_estimated;
				//cout << TheMSPoseTracker.getTvec() << endl;
				cout << temp.at<double>(0) << "," << temp.at<double>(1) << "," << temp.at<double>(2) << " " << temp2.at<double>(0) << "," << temp2.at<double>(1) << "," << temp2.at<double>(2) << endl;
					m.unlock();
				
					//phoneUpdate.updateRotation(kalman);
			//	cout << TheMSPoseTracker.getRvec() << endl;
					phoneUpdate.updatePosition(kalman);
				}
			
		//	imshow("Frame", TheInputImageCopy);
			key = waitKey(1);
		//	for (auto idx : TheMarkerMapConfig.getIndices(detected_markers))
			//	detected_markers[idx].draw(TheInputImageCopy, Scalar(0, 0, 255), 1);

			// show  informatino
			//key = Viewer.show(TheMarkerMapConfig, TheMSPoseTracker.getRTMatrix(), TheInputImageCopy, waitTime);
		//	if (key == 's') waitTime = waitTime ? 0 : 0

		} while (key != 27);
		started = false;
	updateThread.join();

			
	}
	catch (std::exception& ex)
	{
		//here check if updateThread is still running if it is then join it
		
		cout << "Exception :" << ex.what() << endl;
	}
	
}



