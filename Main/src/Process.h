#pragma once
#include "Kalman.h"
#include <Windows.h>
#include <string>
#include <vector>
class Process
{
public:
	Process();
	~Process();


	//std::vector<std::string> Split(std::string line);

	//void CreateChildProcess();
	
	static void ErrorExit(PTSTR lpszFunction);
	void createProcess();

	static std::vector<std::string> Split(std::string line);
	static std::vector<std::string> SplitComma(std::string strToSplit);
	void ReadFromPipe(DWORD& dwRead, DWORD& dwWritten, CHAR chBuf[], BOOL bSuccess, HANDLE hParentStdOut, Kalman& kalman);
	void CreateChildProcess();
	int createReadableProcess();


};

