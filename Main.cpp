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
#include <opencv2/calib3d.hpp>
#include <vector>
#include <windows.data.json.h>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <sstream>
//#include <aruco\aruco.h>
#include <cstdlib>
#include <cstring>
#include <curl.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>

#include "Utils.h"
#include "Inputs.h";
#include "Kalman.h"
#include "Update.h"
#include "Process.h"
#include "HTTP.h"
#include "Screen.h"
#define BUFSIZE 4096



//have a bool as flag, when kalman is about to updated with image data, dont update it with imu data at the same time



 /*    INIT CONNECTION   */

//start udpdowna
//or just use its run method with port 8600

//check if the device camera config file is where its supposed to be, otherwise set it up
//check user preferences for device

 /********************/



/*  INIT SETUP */

//for the phone run the setup script, that will eighre store the files locally or send them to the computer, same for the controller



/*
for controller setup, use the markermapper library, give a calibration object (a cube with multiple boards), then run the calibration code for both cameras simultaniusly to determine their parameters and the relationship between them

*/




//further todo
//in process maybe change it so that it is not hardcoded in
//find a way to deal with different camera indexs
//try making http callback function static and using it like that
//

 



 //run pnp for both controllers simultaniously
 //then using the known position and rotation change, use a homogenous transformation from one camera to the other, compare the two estimated positions (if one camera has no position, then use the one with position), and average the two
 



//for controller prototype with just one camera because its cheaper
//the camera will have its own kalman, update and process functions
//for contorller only call mouseUpdate, and to move with joystick arduino will 








void getHTTPdata(CURLcode& res, CURL* curl) {
	res = curl_easy_perform(curl);
	/* Check for errors */
	if (res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
}


CURLcode res;
CURL *curl;
/*HANDLE g_hChildStd_IN_Rd;
HANDLE g_hChildStd_IN_Wr;
HANDLE g_hChildStd_OUT_Rd;
HANDLE g_hChildStd_OUT_Wr; */
//look at data again
// std::vector<double[3]> gyro;
//std::vector<double[3]> lin_accel;
int parseHTTPdata(std::string& s,  bool isGyro, Kalman& kalman) {
	//find "data", then find the second opening braces, split based on comas (use substring (1, length-1)


	double output[3]{ 0,0,0 };
	size_t pos = 0;
	size_t posEnd1 = 0;
	size_t posEnd2 = 0;
	// Repeat till end is reached
	while (pos >= 0 && pos < s.size())
	{
		//find the second opening braces
		 pos = s.find("[", pos + 1);
		 posEnd1 = s.find("[", pos + 2);
		 posEnd2 = s.find("]", posEnd1 + 1);
		//now plit it based on comas
		std::vector<std::string> values = Process::SplitComma(s.substr(posEnd1 + 1, posEnd2 - posEnd1 - 1));
		if (values.size() > 3 || values[0].find("L") != std::string::npos)
			continue;
		//call kalman imu function
		output[0] = stod(values[0]);
		output[1] = -stod(values[1]); //flip the y
		output[2] = stod(values[2]);
	/*	for (int i = 0; i < values.size(); i++) {
			
			output[i] = stod(values[i]);
			//std::cout << values[i] << " ";
		}
	*/

		kalman.kalmanIMU(output);


		pos = posEnd2 + 1;
	}
	return pos;
}

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
{
	size_t newLength = size*nmemb;
	try
	{
		s->append((char*)contents, newLength);
	}
	catch (std::bad_alloc &e)
	{
		//handle memory problem
		return 0;
	}
	return newLength;
}



void updateProcess(std::string& s) {
	s = "";
	res = curl_easy_perform(curl);
}

void fail() {
	fprintf(stderr, "curl_easy_perform() failed: %s\n",
		curl_easy_strerror(res));

	curl_easy_cleanup(curl);
}


void startProcess(std::string& s) {

	curl_global_init(CURL_GLOBAL_DEFAULT);

	DWORD dwRead, dwWritten;
	//CHAR chBuf[BUFSIZE];
	CHAR chBuf[4096];
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOuDt = GetStdHandle(STD_OUTPUT_HANDLE);

	curl = curl_easy_init();

	if (curl)
	{
		std::string atHome = "192.168.0.30:8080/sensors.json";
		std::string atSchool = "10.13.224.161:8080/sensors.json";
		curl_easy_setopt(curl, CURLOPT_URL, "10.13.224.88:8080/sensors.json");


		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		res = curl_easy_perform(curl);


	}
}

bool checkProcess() {
	return res == CURLE_OK;
}



  //kalman imu test
/*int main()
  {
	 Kalman phoneKalman;
	// HTTP phoneIMU;
	 Process mapperProcess;
	 Update phoneUpdate;
	 phoneKalman.initPosRot(0.125);
	 mapperProcess.createReadableProcess();
	  
	      int count = 0;
		  std::string s;//whats all this s is null
		  startProcess(s);
		//  phoneIMU.startProcess(s);
		  //check if curl is setup correctly, otherwise call scurl.fail
		
		  DWORD dwRead, dwWritten;
		  CHAR chBuf[BUFSIZE];
		  BOOL bSuccess = FALSE;
		  
		  HANDLE hParentStdOuDt = GetStdHandle(STD_OUTPUT_HANDLE);
		  int next = 0;

		  while (checkProcess() )
		  {
			 mapperProcess.ReadFromPipe(dwRead, dwWritten, chBuf, bSuccess, hParentStdOuDt, phoneKalman);
			
			//this may need to be changed in the future, it would be better to continully look for pose, and only use the best one a min of every x seconds
			  
			  parseHTTPdata(s, false, phoneKalman);
			  
			//  phoneUpdate.updatePosition(phoneKalman);
			  phoneUpdate.updateMouse(phoneKalman.rotationMat);
			
			  updateProcess(s);
		  }

	  std::cout << "Program finished!" << std::endl;

	  return 0;
  } */

int main() {
	Inputs::MouseMove(100, 100);

	return 0;
}
  
  


  