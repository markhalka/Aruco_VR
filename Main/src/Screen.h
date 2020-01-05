#pragma once
#include <opencv2\opencv.hpp>
#include <Windows.h>
class Screen
{
public:

	cv::Mat hwnd2mat(HWND hwnd);
	void DisplayMat(cv::Mat& image);
};

