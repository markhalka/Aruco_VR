#include "Utils.h"




Utils::Utils()
{
}


Utils::~Utils()
{
}

 cv::Mat Utils::rot2euler(const cv::Mat & rotationMatrix)
{
	cv::Mat euler(3, 1, CV_64F);

	double m00 = rotationMatrix.at<double>(0, 0);
	double m02 = rotationMatrix.at<double>(0, 2);
	double m10 = rotationMatrix.at<double>(1, 0);
	double m11 = rotationMatrix.at<double>(1, 1);
	double m12 = rotationMatrix.at<double>(1, 2);
	double m20 = rotationMatrix.at<double>(2, 0);
	double m22 = rotationMatrix.at<double>(2, 2);

	double bank, attitude, heading;

	// Assuming the angles are in radians.
	if (m10 > 0.998) { // singularity at north pole
		bank = 0;
		attitude = CV_PI / 2;
		heading = atan2(m02, m22);
	}
	else if (m10 < -0.998) { // singularity at south pole
		bank = 0;
		attitude = -CV_PI / 2;
		heading = atan2(m02, m22);
	}
	else
	{
		bank = atan2(-m12, m11);
		attitude = asin(m10);
		heading = atan2(-m20, m00);
	}

	euler.at<double>(0) = bank;
	euler.at<double>(1) = attitude;
	euler.at<double>(2) = heading;

	return euler;
}

 cv::Mat Utils::euler2rot(const cv::Mat & euler)
{
	cv::Mat rotationMatrix(3, 3, CV_64F);

	double bank = euler.at<double>(0);
	double attitude = euler.at<double>(1);
	double heading = euler.at<double>(2);

	// Assuming the angles are in radians.
	double ch = cos(heading);
	double sh = sin(heading);
	double ca = cos(attitude);
	double sa = sin(attitude);
	double cb = cos(bank);
	double sb = sin(bank);

	double m00, m01, m02, m10, m11, m12, m20, m21, m22;

	m00 = ch * ca;
	m01 = sh*sb - ch*sa*cb;
	m02 = ch*sa*sb + sh*cb;
	m10 = sa;
	m11 = ca*cb;
	m12 = -ca*sb;
	m20 = -sh*ca;
	m21 = sh*sa*cb + ch*sb;
	m22 = -sh*sa*sb + ch*cb;

	rotationMatrix.at<double>(0, 0) = m00;
	rotationMatrix.at<double>(0, 1) = m01;
	rotationMatrix.at<double>(0, 2) = m02;
	rotationMatrix.at<double>(1, 0) = m10;
	rotationMatrix.at<double>(1, 1) = m11;
	rotationMatrix.at<double>(1, 2) = m12;
	rotationMatrix.at<double>(2, 0) = m20;
	rotationMatrix.at<double>(2, 1) = m21;
	rotationMatrix.at<double>(2, 2) = m22;

	return rotationMatrix;
}

cv::Mat Utils::resizeImage(cv::Mat &in, float resizeFactor) {
	if (fabs(1 - resizeFactor)<1e-3)return in;
	float nc = float(in.cols)*resizeFactor;
	float nr = float(in.rows)*resizeFactor;
	cv::Mat imres;
	cv::resize(in, imres, cv::Size(nc, nr));

	return imres;
}

double Utils::getVectorMag(std::vector<double> vec) {
	int total = 0;
	for (auto i : vec) {
		total += pow(i, 2);
	}
	return sqrt(total);
}

std::vector<double> Utils::getVectorSub(std::vector<double>& a, std::vector<double>& b) {
	std::vector<double> output = a;
	for (int i = 0; i < output.size(); i++) {
		output[i] -= b[i];
	}
	return output;
}

std::vector<double> Utils::getVectorNeg(std::vector<double>& input) {
	std::vector<double> output = input;
	for (int i = 0; i < input.size(); i++) {
		output[i] = -output[i];
	}
	return output;
}


struct Utils::EulerAngles
{
double roll, pitch, yaw;
};

/*EulerAngles Utils::ToEulerAngles(Quaternion q)
{
EulerAngles angles;

// roll (x-axis rotation)
double sinr_cosp = +2.0 * (q.w * q.x + q.y * q.z);
double cosr_cosp = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
angles.roll = atan2(sinr_cosp, cosr_cosp);

// pitch (y-axis rotation)
double sinp = +2.0 * (q.w * q.y - q.z * q.x);
if (fabs(sinp) >= 1)
angles.pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
else
angles.pitch = asin(sinp);

// yaw (z-axis rotation)
double siny_cosp = +2.0 * (q.w * q.z + q.x * q.y);
double cosy_cosp = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
angles.yaw = atan2(siny_cosp, cosy_cosp);

return angles;
}
*/

