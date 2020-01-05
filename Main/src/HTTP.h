#pragma once
#include <curl.h>
#include <Windows.h>
#include <string>
#include "Kalman.h"
class HTTP
{
public:
	HTTP();
	~HTTP();
	
	CURLcode res;
	CURL *curl;
	HANDLE g_hChildStd_IN_Rd;
	HANDLE g_hChildStd_IN_Wr;
	HANDLE g_hChildStd_OUT_Rd;
	HANDLE g_hChildStd_OUT_Wr;
	void getHTTPdata(CURLcode& res, CURL* curl);
	int parseHTTPdata(std::string& s, int start, int end, bool isGyro, Kalman& kalman);
	static size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s);
	void startProcess(std::string&);
	bool checkProcess();
	void updateProcess(std::string& s);
	void fail();
};

