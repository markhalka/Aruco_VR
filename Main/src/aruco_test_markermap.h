
#pragma once
#include <string>
#include "Kalman.h"
#include "Update.h"

class aruco_test_markermap
{
public:

	static int run(std::string markermap, float size, std::string video, std::string camera, Kalman& kalman, Update& phoneUpdate);
};

