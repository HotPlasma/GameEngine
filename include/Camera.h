#pragma once

#include "PreHeader.h"

class Camera // Class Designed to hold my camera positions and navigate it on user input
{
private:
	glm::vec3 CameraPos; // Position of the Camera
	glm::vec3 CameraView; // View of the Camera
public:
	Camera();
	glm::vec3 GetCameraPos(); // Returns Camera Pos
	glm::vec3 GetCameraView(); // Returns Camera View

	void ProcessUserInput(float yAngle, float zAngle); // Proccesses user input in order to move camera
};