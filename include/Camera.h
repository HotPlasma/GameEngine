#pragma once

#include "PreHeader.h"

class Camera // Class Designed to hold my camera positions and navigate it on user input
{
	private:
		glm::vec3 m_position; // Position of the Camera
		glm::vec3 m_view; // View of the Camera
	
	public:
		Camera();
		glm::vec3 getPosition() { return m_position; }; // Returns Camera Pos
		glm::vec3 getView() { return m_view; }; // Returns Camera View
	
		void processUserInput(float fYAngle, float fZAngle); // Proccesses user input in order to move camera
};