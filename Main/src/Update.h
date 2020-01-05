#pragma once
#include <opencv2\opencv.hpp>
#include <vector>
#include "Kalman.h"

class Update
{
public:
	Update();

	void updatePosition(Kalman& kalman);
	void updateRotation(Kalman& kalman);
	void updateKeyboard(std::vector<double> translation, std::vector<double> rotation);
	void updateMouse(cv::Mat deltaRotation);
	
};

