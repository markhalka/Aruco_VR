#include "HTTP.h"
#include "Process.h"



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



//look at data again
// std::vector<double[3]> gyro;
//std::vector<double[3]> lin_accel;
int HTTP::parseHTTPdata(std::string& s, int pos, bool isGyro, Kalman& kalman) {
	//find "data", then find the second opening braces, split based on comas (use substring (1, length-1)


	double output[3]{ 0,0,0 };

	// Repeat till end is reached
	while (pos < s.size())
	{
		//find the second opening braces
		size_t posBraces = s.find("[", pos + 1);
		size_t posBraces2 = s.find("[", posBraces + 2);
		size_t posEndBraces = s.find("]", pos + 1);
		//now plit it based on comas
		std::vector<std::string> values = Process::SplitComma(s.substr(posBraces2 + 1, posEndBraces - posBraces2 - 1));
		if (values.size() > 3)
			continue;
		//call kalman imu function
		for (int i = 0; i < values.size(); i++) {
			//output[i] = stod(values[i]);
			std::cout << values[i] << " ";
		}
		std::cout << std::endl;
		kalman.kalmanIMU(output);

		// Get the next occurrence from the current position
		pos = posEndBraces + 1;
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
		curl_easy_setopt(curl, CURLOPT_URL, "10.13.224.88:8080/sensors.json");


		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		res = curl_easy_perform(curl);		
	

	}
}

bool HTTP::checkProcess() {
	return res == CURLE_OK;
}

