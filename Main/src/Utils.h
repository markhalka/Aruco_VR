#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <map>
class Utils
{
public:

	static cv::Mat rot2euler(const cv::Mat & rotationMatrix);
    static cv::Mat euler2rot(const cv::Mat & euler);
	struct EulerAngles;
	static std::vector<double> getVectorNeg(std::vector<double>& input);
	static std::vector<double> getVectorSub(std::vector<double>& a, std::vector<double>& b);
	static double getVectorMag(std::vector<double> vec);
	static cv::Mat resizeImage(cv::Mat &in, float resizeFactor);
	static std::vector<double> rotationMatrixToEulerAngles(cv::Mat &R);
	static bool isRotationMatrix(cv::Mat &R);
};

