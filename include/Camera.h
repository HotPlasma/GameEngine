#pragma once

#include "PreHeader.h"

class Camera // Class Designed to hold my camera positions and navigate it on user input
{
	private:
		float m_fFOV; // Camera field of view
		glm::vec3 m_position; // Position of the Camera
		glm::vec3 m_view; // View of the Camera
		glm::vec3 m_up; // Up direction of the Camera
	
	public:
		Camera();
		float getFOV() { return m_fFOV; }; // Returns Camera FOV
		glm::vec3 getPosition() { return m_position; }; // Returns Camera Pos
		glm::vec3 getView() { return m_view; }; // Returns Camera View
		glm::vec3 getUp() { return m_up; }; // Returns Camera Up
	
		void processUserInput(float fYAngle, float fZAngle); // Proccesses user input in order to move camera
};