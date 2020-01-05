#include "Update.h"
#include "Utils.h"
#include "Inputs.h"
#include <numeric>
#include <string>
//
double offset = 0.1; //20 cm?
std::vector<std::pair<double, double>> movmentThreshold;
cv::Mat currEurler = cv::Mat::zeros(3, 1, CV_64F); //we only need two angles
cv::Mat prevEuler = cv::Mat::zeros(3, 1, CV_64F);
cv::Mat deltaEuler = cv::Mat::zeros(2, 1, CV_64F);
cv::Mat unitVector = cv::Mat(3, 1, CV_64FC1);

int count;
double height;
Update::Update()
{
	count = 0;
	height = 0;
	for (int i = 0; i < 3; i++) {
		movmentThreshold.push_back(std::pair<double, double>(0, 0));
		movmentThreshold[i].first = -offset;
		movmentThreshold[i].second = offset;

	}
	unitVector.at<double>(0) = 0;
	unitVector.at<double>(1) = 0;
	unitVector.at<double>(2) = -1;
}



void Update::updateKeyboard(std::vector<double> translationDirection, std::vector<double> directionVector) {
//	std::cout << translationDirection[0] << " " << translationDirection[1] << std::endl;
	std::vector<std::vector<std::string>> keys{ {"w" }, {"w","d"}, {"d"}, {"s","d"}, {"s"}, {"s","a"}, {"a"}, {"w","a"} };
	
	double dot = translationDirection[0] * directionVector[0] + translationDirection[1] * directionVector[1]; 
	double det = translationDirection[0] * directionVector[1] - translationDirection[1] * directionVector[0];


	double angle = 180/3.14158 * atan2(det, dot);

	angle += 22.5; //this offsets the std::vector to make the calculation easier

	while (angle < 0) {
		angle += 360;
	}

	while (angle > 360) {
		angle -= 360;
	}

	for (int i = 1; i <= 8; i++) {
		if (angle <= 45*i) {
			for (int j = 0; j < keys[i-1].size(); j++) {
			//	std::cout << keys[i-1][j];			
				//Inputs::Keyboard(keys[i-1][j][0]);
			}
			std::cout << std::endl;
			break;
		}
	}

}


bool wasJumping = false;
bool wasCrouching = false;
void Update::updatePosition(Kalman& kalman) {
	if (count < 10) {
		count++;
	}

	std::vector<double> translationVector(3, 0);
	std::vector<double> rotationVector(2, 0);
	cv::Mat endPoint = cv::Mat(3, 1, CV_64FC1);
	endPoint = kalman.rotation_mat * unitVector; //it only depend on one rotation
	//std::cout << endPoint << std::endl;//endPoint.at<double>(0) << " " << endPoint.at<double>(2) << std::endl;
	//ignore the y corrdinate
	//return;
	rotationVector[0] = endPoint.at<double>(1);
	rotationVector[1] = endPoint.at<double>(2);

	
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			continue;
			offset = 0.4;

		}
		if (kalman.translation_estimated.at<double>(i) >= movmentThreshold[i].second) {
	
			if (i == 1) {
				//this is the y component treat this differnetly
				//jump
				if (!wasJumping) {
					wasJumping = true;
					wasCrouching = false;
						std::cout << "space" << std::endl;
				}
				
		
				//Inputs::Keyboard(' ');
				if(count > 9)
				continue;
			}
			movmentThreshold[i].first = movmentThreshold[i].second - offset;
			movmentThreshold[i].second = movmentThreshold[i].second + offset;
	
		//	std::cout << "right ";

			translationVector[i] = 1; 
			
			updateKeyboard(std::vector<double>{ translationVector[0], translationVector[2] }, rotationVector); //not sure if this is right?
			translationVector[i] = 0;
		
		}
		else {
			if (i == 1) {
				wasJumping = false;
			}
		}
		
		if (kalman.translation_estimated.at<double>(i) <= movmentThreshold[i].first) {
			//call function
			//  std::cout << "left ";
	
			if (i == 1 ) {
				//y component treat this differently
				//crouch 
				//Inputs::Keyboard('1');
				if (!wasCrouching) {
					std::cout << "ctrl" << std::endl;
					wasCrouching = true;
					wasJumping = false;
				}			
				//find virtual keycode or char for ctrl
				if(count > 9)
				continue;
			}
			movmentThreshold[i].first = movmentThreshold[i].first - offset;
			movmentThreshold[i].second = movmentThreshold[i].first + offset;
		
			translationVector[i] = -1; 
			updateKeyboard(std::vector<double>{ translationVector[0], translationVector[2] }, rotationVector); //not sure if this is right?
			translationVector[i] = 0;
		
		}
		else {
			if(i==1)
			wasCrouching = false;
		
		}
	}
	
}

void Update::updateRotation(Kalman& kalman) {
	cv::Mat temp = kalman.rotation_mat * unitVector;

	deltaEuler.at<double>(0) = temp.at<double>(1) - prevEuler.at<double>(1);
	deltaEuler.at<double>(1) = temp.at<double>(2) - prevEuler.at<double>(2);	
	prevEuler = temp;
	updateMouse(deltaEuler);
}


//this could also be similar to updatekeyboard, where it has to cross a threshold
void Update::updateMouse(cv::Mat rotation) {
	//rotation has two components, yaw and pitch that corespond to x and y on screen
	int centerX = 640;
	int centerY = 360;
	std::vector<double> offset{ 100,100 };

	//Mouse move should be center x + rotation, center y + rotation amt
	Inputs::MouseMove(centerX + rotation.at<double>(0) * offset[0], centerY + rotation.at<double>(1) * offset[1]);
//	Inputs::DeltaMouseMove(rotation.at<double>(2) * offset[0], rotation.at<double>(1) * offset[1],0);
	//change in offset will be able to be changed manually
   
}

