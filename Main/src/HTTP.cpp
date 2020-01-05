#include "HTTP.h"
#include "Process.h"
#include <mutex>


HTTP::HTTP()
{
}


HTTP::~HTTP()
{
}


void HTTP::getHTTPdata(CURLcode& res, CURL* curl) {
	res = curl_easy_perform(curl);
	/* Check for errors */
	if (res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
}


std::mutex m2;
int HTTP::parseHTTPdata(std::string& s, int start, int end, bool isGyro, Kalman& kalman) {
	//find "data", then find the second opening braces, split based on comas (use substring (1, length-1)
	
	double output[3]{ 0,0,0 };
	size_t pos = start;
	size_t posEnd1 = start;
	size_t posEnd2 = start;


	while (pos >= 0 && pos < end)
	{

		//find the second opening braces
		pos = s.find("[", pos + 1);
		posEnd1 = s.find("[", pos + 2);
		posEnd2 = s.find("]", posEnd1 + 1);
		//now plit it based on comas
		std::vector<std::string> values = Process::SplitComma(s.substr(posEnd1 + 1, posEnd2 - posEnd1 - 1));
		if (values.size() > 3 || values[0].find("L") != std::string::npos)
			continue;

		cv::Mat translationVector = cv::Mat(3, 1, CV_64F);
		//call kalman imu function
		translationVector.at<double>(0) = -stod(values[0]); //i think x is fliped too
		translationVector.at<double>(1) = -stod(values[1]); //flip the y
		translationVector.at<double>(2) = stod(values[2]);
		
		cv::Mat rotationVector = cv::Mat(3, 1, CV_64F);
		cv::Mat newRotation = cv::Mat::zeros(3, 3, CV_64F);//kalman.rotation_mat * rotationVector;
	//	m2.lock();
		cv::Mat newTranslation = kalman.rotation_mat * translationVector; //check if this should be the inverse
		m2.lock();

		kalman.kalmanIMU(newTranslation, newRotation);
		m2.unlock();
		//either use the rotation vector (probably a quaternion) use it to rotate the unit vector (find a rotation vector), then rotate the rotation vector by the rotation matrix to find the new rotation vector in aruco coordinates

		pos = posEnd2 + 1;
		Sleep(0.05);
	}
	return pos;
}

size_t HTTP::CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
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



void HTTP::updateProcess(std::string& s) {
	s = "";
	res = curl_easy_perform(curl);
}

void HTTP::fail() {
	fprintf(stderr, "curl_easy_perform() failed: %s\n",
		curl_easy_strerror(res));

	curl_easy_cleanup(curl);
}


void HTTP::startProcess(std::string& s) {

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
		curl_easy_setopt(curl, CURLOPT_URL, "192.168.0.15:8080/sensors.json");


		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HTTP::CurlWrite_CallbackFunc_StdString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		res = curl_easy_perform(curl);		
	

	}
}

bool HTTP::checkProcess() {
	return res == CURLE_OK;
}

