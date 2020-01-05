#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <vector>
#include <windows.data.json.h>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <cstring>
#include <curl.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <thread>
//#include <aruco\aruco.h>
#include "Utils.h"
#include "Inputs.h";
#include "Kalman.h"
#include "Update.h"
#include "Process.h"
#include "HTTP.h"
#include "Screen.h"
#include "mapper_from_video.h"
#include "aruco_calibration.h"
#include "aruco_test_markermap.h"

#define BUFSIZE 4096



bool getConnection(int mode, std::string& ip ) {
	//based on the mode, connect with the phone and controller
	//based on the name of the phone, or some special pin, look for that file in the computer
	switch (mode) {
	case 1:
		//this mode uses controller and phone
		break;
	case 2:
		//this mode just uses phone
		//search for the phone on the network
		//if found, search for files
		std::string a = "";
		std::string b = "";
		//checkFiles(a, b);

		//if the files aren't found, create them
		//aruco_calibration::run("0", 0.035, "phoneOutput");
		//mapper_from_video::run("markerset", "C:\\Users\\hithe\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\MainApp\\MainApp\\phoneOutput.yml", 0.06, "http://192.168.0.11:8080/video");
		break;
	}
	return true;
}


void checkFiles(std::string& phone, std::string& map) {
	//run file finder exe here using process 
	//if cant find (some identifier for phone)
	//create it and assign it to the string


	std::vector<std::string> found;

	Inputs::findFile(phone, found); 
	
	for (int i = 0; i < found.size(); i++) {
		std::cout << found[i] << std::endl;
	}
	//if cant find some identifier for the room create it
}


//update the imu
void updateIMU(HTTP& phoneIMU, Kalman& phoneKalman) {
	int count = 0;
	std::string s;

	//	phoneIMU.startProcess(s);
	//check if curl is setup correctly, otherwise call scurl.fail
	phoneIMU.startProcess(s);
	while (phoneIMU.checkProcess()) {
		phoneIMU.updateProcess(s);
		int start = 0;
		phoneIMU.parseHTTPdata(s, start, s.size(), false, phoneKalman);
		// std::cout << phoneKalman.translation_estimated.at<double>(0) << std::endl;
	}

}

//update the pose
void updateCamera(Kalman& phoneKalman, Update& phoneUpdate) { //dont need phoneUpdate anymore
//	aruco_test_markermap::run("markerset.yml", 0.06, "http://192.168.0.11:8080/video", "phoneOutput.yml", phoneKalman, phoneUpdate);

}
void start(std::string address) {
	//std::thread processThread;
	Kalman phoneKalman;
	HTTP phoneIMU;
	Update phoneUpdate;

	phoneKalman.initPosRot(0.125); //change this value
	double acc[3]{ 0,0,0 }; //fix that
	//phoneKalman.kalmanIMU(acc);
	//	std::thread imu(updateIMU, phoneIMU, phoneKalman);
	//	std::thread camera(updateCamera, phoneKalman, phoneUpdate);
	//aruco_test_markermap::run("markerset.yml", 0.06, "http://192.168.0.24:8080/video", "phoneOutput.yml", phoneKalman, phoneUpdate);

}

int main()
{

	Kalman phoneKalman;
	HTTP phoneIMU;
	Update phoneUpdate;
	
	phoneKalman.initPosRot(0.05); //change this value
	double acc[3]{ 0,0,0 }; //fix that
	cv::Mat zeros = cv::Mat::zeros(3, 3, CV_64F);
	phoneKalman.kalmanIMU(zeros, zeros);
	std::string s = "";
	//phoneIMU.startProcess(s);

	//std::thread imu(updateIMU, phoneIMU, phoneKalman);
//	std::thread camera(updateCamera, phoneKalman, phoneUpdate);
	//mapper_from_video::run("single", "computerOutput.yml", 0.06, "http://192.168.0.15:8080/video");
		aruco_test_markermap::run("C:\\Users\\hithe\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\MainApp\\MainApp\\single.yml", 0.06, "http://192.168.0.15:8080/video", "C:\\Users\\hithe\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\MainApp\\MainApp\\computerOutput.yml", phoneKalman, phoneUpdate);
		
	
		//	while (true) {
		//	phoneUpdate.updatePosition(phoneKalman); //make sure that everywhere kalman is mutex locked
		//	phoneUpdate.updateRotation(phoneKalman);
		//	Sleep(10);
			//just update it every 10ms, small enought to have no latency, but there's no point in updating it too frequently because too much change cant happen and its just a waste
	//	if (!phoneIMU.checkProcess()) {
		//	imu.join();
    //    }

	//camera.join();
	return 0;
} 
  

