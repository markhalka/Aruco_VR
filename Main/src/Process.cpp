#include "Process.h"
#include <windows.data.json.h>
#include <iostream>
#include <sstream>
#include <strsafe.h>
#include <iterator>
#define BUFSIZE 4096

Process::Process()
{
}


Process::~Process()
{
}



HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

//HANDLE g_hInputFile = NULL;


//splits a string based on spaces
std::vector<std::string> Process::Split(std::string line) {
	std::istringstream iss{ line };
	return std::vector<std::string>(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
}


//splits a string based on commas
std::vector<std::string> Process::SplitComma(std::string strToSplit)
{
	std::stringstream ss(strToSplit);
	std::string item;
	std::vector<std::string> splittedStrings;
	while (getline(ss, item, ','))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}

void WriteToPipe(DWORD& dwWritten, HANDLE hParentStdOut, Kalman& kalman) {
	
	
		WriteFile(hParentStdOut,
			"Hello Pipe\n",
			12,   // = length of string + terminating '\0' !!!
			&dwWritten,
			NULL);
		CloseHandle(hParentStdOut);
}

//reads and parses from process, updates kalman based on rotation and translation
//rewrite this so that it doesnt return, but trys the next possible spot
void Process::ReadFromPipe(DWORD& dwRead, DWORD& dwWritten, CHAR chBuf[], BOOL bSuccess, HANDLE hParentStdOut, Kalman& kalman) {
	cv::Mat unitVector = cv::Mat::zeros(3, 1, CV_64FC1);
	unitVector.at<double>(0) = 0;
	unitVector.at<double>(1) = 0;
	unitVector.at<double>(2) = 1;

	try {


		bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
	}
	catch (...) {
		std::cout << " exception at read from pipe";
	}
	if (!bSuccess || dwRead == 0) return;

	std::string line = chBuf;
	//std::cout << "start: " << line << std::endl << std::endl;
	int pos = 0;
	int endPos = 0;

	std::vector<std::string> curr;
	//here maybe go throught the entire std::string
	//see how the std::string is handeld (is it overwritten everytime, or does it repeat information)
	std::string currLine = "";
	while (pos < line.size()) {


		if ((pos = line.find("[", pos+1)) != std::string::npos && (endPos = line.find("] [", pos + 3)) != std::string::npos && endPos > pos) {
			currLine = line.substr(pos + 1, endPos - pos - 1);
		//	std::cout << "line 1 " << currLine << std::endl;
			curr = SplitComma(currLine);
			if (curr.size() != 3)
				continue;

			cv::Mat rotation = cv::Mat::zeros(3, 1, CV_64FC1);

			rotation.at<double>(0) = (stod(curr[0]));
			rotation.at<double>(1) = (stod(curr[1]));
			rotation.at<double>(2) = (stod(curr[2].substr(0, curr[2].size() - 1)));

			kalman.rotation_estimated = rotation;
			//use the rotation matrix to rotate the origin vector and get the direction vector
			//update the direction vector in entity, for use in calculateDirection

			//if this doenst work, may try converting from spherical cordinates (x,y, 1) into cartesian cordinates





			pos = line.find("]", endPos + 3);
			//	if ((pos = line.find("]", endPos + 3)) != std::string::npos) {
					//here you can parse the input
			if (pos < endPos) {
				continue;
			}
			currLine = line.substr(endPos + 3, pos - endPos - 3); //check to make sure this gets rid of the brackets

		//	std::cout << "line2 " << currLine << std::endl;												  //split by commas
			curr = SplitComma(currLine);


			if (curr.size() != 3) //|| currLine.find('[') != std::string::npos);
				continue;

			double translation[3]{ 0,0,0 };

			translation[0] = (stod(curr[0]));
			translation[1] = (stod(curr[1]));
			translation[2] = (stod(curr[2].substr(0, curr[2].size() - 1)));

			//rotate the translation by 
			//	std::cout << translation[0] << std::endl;
			kalman.kalmanPose(translation);
		}
		else {
			break;
		}

	}
	if (!bSuccess) return;
}

//handles error in reading process
void Process::ErrorExit(PTSTR lpszFunction){
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(1);
}

//creates a new mapper process
void Process::createProcess()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	TCHAR tpath[] = TEXT("aruco_test_markermap.exe live:1 C:\\Users\\hithe\\Desktop\\MarkerMapper\\marker_mapper_1.0.12w64\\markerset.yml C:\\Users\\hithe\\Desktop\\MarkerMapper\\marker_mapper_1.0.12w64\\computerOut.yml -s=0.06");// C:\\Users\\hithe\\Desktop\MarkerMapper\\marker_mapper_1.0.12w64\\computerOut.yml - s = 0.06");
																																																									// Start the child process. 
	if (!CreateProcess("C:\\Users\\hithe\\Desktop\\MarkerMapper\\marker_mapper_1.0.12w64\\aruco_test_markermap.exe",   // No module name (use command line)
		tpath,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}





int Process::createReadableProcess()
{
	SECURITY_ATTRIBUTES saAttr;

	printf("\n->Start of parent execution.\n");

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 

	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
		Process::ErrorExit(TEXT("StdoutRd CreatePipe"));

	// Ensure the read handle to the pipe for STDOUT is not inherited.

	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
		Process::ErrorExit(TEXT("Stdout SetHandleInformation"));

	// Create a pipe for the child process's STDIN. 

	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
		Process::ErrorExit(TEXT("Stdin CreatePipe"));

	// Ensure the write handle to the pipe for STDIN is not inherited. 

	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
		Process::ErrorExit(TEXT("Stdin SetHandleInformation"));

	// Create the child process. 

	CreateChildProcess();

	// Get a handle to an input file for the parent. 
	// This example assumes a plain text file and uses std::string output to verify data flow. 

	TCHAR inputFile[] = "";

	/*g_hInputFile = CreateFile(
	inputFile,
	GENERIC_READ,
	0,
	NULL,
	OPEN_EXISTING,
	FILE_ATTRIBUTE_READONLY,
	NULL);

	if (g_hInputFile == INVALID_HANDLE_VALUE)
	ErrorExit(TEXT("CreateFile")); */

	// Write to the pipe that is the standard input for a child process. 
	// Data is written to the pipe's buffers, so it is not necessary to wait
	// until the child process is running before writing data.


	
	//WriteToPipe();
	//printf("\n->Contents of %s written to child STDIN pipe.\n", argv[1]);

	// Read from pipe that is the standard output for child process. 

	//	printf("\n->Contents of child process STDOUT:\n\n", argv[1]);
	//	ReadFromPipe(); this is going to be moved inside of the main while loop

	printf("\n->End of parent execution.\n");

	// The remaining open handles are cleaned up when this process terminates. 
	// To avoid resource leaks in a larger application, close handles explicitly. 

	return 0;
}



void Process::CreateChildProcess()
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
	TCHAR szCmdline[] = TEXT("child");
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 
	TCHAR tpath[] = TEXT("BoardDetectB.exe  http://192.168.0.15:8080/video C:/Users/hithe/Desktop/MarkerMapper/marker_mapper_1.0.12w64/computerOut.yml 0.06"); //TEXT("aruco_test_markermap.exe live:1 C:\\Users\\hithe\\Desktop\\MarkerMapper\\marker_mapper_1.0.12w64\\school.yml C:\\Users\\hithe\\Desktop\\MarkerMapper\\marker_mapper_1.0.12w64\\computerOut.yml -s 0.06");																																																								// Start the child process. 

	bSuccess = CreateProcess("C:\\Users\\hithe\\OneDrive\\Documents\\Visual Studio 2015\\Projects\\BoardDetectB\\x64\\Debug\\BoardDetectB.exe",//CreateProcess("C:\\Users\\hithe\\Desktop\\MarkerMapper\\marker_mapper_1.0.12w64\\aruco_test_markermap.exe",
		tpath,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

					   // If an error occurs, exit the application. 
	if (!bSuccess)
		Process::ErrorExit(TEXT("CreateProcess"));
	else
	{
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 

		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
	}
}




