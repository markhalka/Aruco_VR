#include "Screen.h"


Screen::Screen()
{
}


Screen::~Screen()
{
}

cv::Mat Screen::hwnd2mat(HWND hwnd) {

	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	cv::Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;  // get the height and width of the screen
	GetWindowRect(hwnd, &windowsize);

	srcheight = 1080;
	srcwidth = 1920;
	height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
	width = windowsize.right / 1;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

																									   // avoid memory leak
	DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(hwnd, hwindowDC);

	return src;
}

void Screen::DisplayMat(cv::Mat& image) {
	cv::Mat dst;
	//  normalize(image, dst, 0, 1, cv::NORM_MINMAX);
	imshow("test", image);
	cv::waitKey(0);
}

HWND hwndDesktop = GetDesktopWindow();

//add udp method to send mat to phone

void Screen::GetRawImageBytes(unsigned char* data, int width, int height)
{
	//Resize cv::Mat to match the array passed to it from C#
	cv::Mat _currentFrame = hwnd2mat(hwndDesktop); //was hDesktopWnd
	cv::Mat resizedMat(height, width, _currentFrame.type());
	cv::resize(_currentFrame, resizedMat, resizedMat.size(), cv::INTER_CUBIC);

	//You may anot need this line. Depends on what you are doing
	///	cv::imshow("Nicolas", resizedMat);

	//Convert from RGB to ARGB 
	cv::Mat argb_img;
	//cv::cvtColor(resizedMat, argb_img, CV_RGB2BGRA);
	cv::cvtColor(resizedMat, argb_img, 2);
	std::vector<cv::Mat> bgra;
	cv::split(argb_img, bgra);
	std::swap(bgra[0], bgra[3]);
	std::swap(bgra[1], bgra[2]);
	std::memcpy(data, argb_img.data, argb_img.total() * argb_img.elemSize());
}