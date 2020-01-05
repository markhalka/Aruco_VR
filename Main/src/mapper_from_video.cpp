#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <aruco/cameraparameters.h>
#include <aruco/dictionary.h>
#include <memory>
#include <opencv2/imgproc/imgproc.hpp>
#include "markermapper.h"
#include "debug.h"
#include "aruco/markerlabeler.h"
#include "mapper_from_video.h"

using namespace std;


int mapper_from_video::run(string baseName, string camera, float size, string video ) {

	try {

		string outBaseName = baseName;
		aruco::CameraParameters Camera;
		Camera.readFromXMLFile(camera);
		float markerSize = size;
		int ref_Marker_Id = -1;
		int frameIncrement = 1;
		int waitTime = 5;
		std::shared_ptr<aruco_mm::MarkerMapper> AMM;
		AMM = aruco_mm::MarkerMapper::create();
		AMM->setParams(Camera, markerSize, ref_Marker_Id);
	
			AMM->getMarkerDetector().setDictionary("ARUCO_MIP_36h12");

		char key = 0;
		int frame = 0;

			cv::Mat image, image2, img_resized;
			cv::VideoCapture vcap;
			vcap.open(0); //video
		
			if (!vcap.isOpened()) { cerr << "Could not open input" << endl; return -1; }
			//wait until valid images came
			while (image.empty()) { vcap.grab(); vcap.retrieve(image); }

			do {
				vcap.retrieve(image);
				image.copyTo(image2);

				frame++;
				if (frame%frameIncrement != 0) continue;
				AMM->process(image, frame);
				AMM->drawDetectedMarkers(image2);
				cv::resize(image2, img_resized, cv::Size(min(image2.cols, 1080), min(image2.rows, 720)));
				cv::imshow("image", image2);
				key = cv::waitKey(waitTime);

			
			} while (vcap.grab() && key != 27);
			cout << "Finish processing video." << endl;
		
		cout << "Starting optimization." << endl;


		AMM->optimize();

		AMM->saveToPcd(outBaseName + ".pcd", true);
		AMM->saveFrameSetPosesToFile(outBaseName + ".log");
		AMM->getMarkerMap().saveToFile(outBaseName + ".yml");
		AMM->getCameraParams().saveToFile(outBaseName + "-cam.yml");

	}
	catch (std::exception &ex) {
		cerr << ex.what() << endl;
	}

	return 0;

}

