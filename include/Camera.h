#pragma once

#include "PreHeader.h"

class Camera // Class Designed to hold my camera positions and navigate it on user input
{
	private:
		float m_fFOV; // Camera field of view
		float m_fNear; // Near culling distance
		float m_fFar; // Far culling distance
		float m_fAspectRatio; // Camera aspect ratio

		//The camera coordinate axes
		glm::vec3 m_xAxis;
		glm::vec3 m_yAxis;
		glm::vec3 m_zAxis;

		glm::vec3 m_position; // Position of the Camera
		glm::quat m_orientation; // Orientation of the Camera
		glm::mat4 m_view; // View of the Camera
		glm::mat4 m_projection; // Projection of the Camera
	
	public:
		Camera();

		glm::vec3 getPosition() { return m_position; }; // Returns Camera Pos
		void setPosition(const glm::vec3 kPosition) { m_position = kPosition; }; // Sets Camera Pos

		float getFOV() { return m_fFOV; }; // Returns Camera FOV
		void setFOV(const float kfFOV) { m_fFOV = kfFOV; }; // Sets Camera FOV

		glm::mat4 getView() { return m_view; }; // Returns Camera View
		glm::mat4 getProjection() { return m_projection; }; // Returns Camera Projection

		void updateView(); // Updates Camera

		void rotate(const float kfYaw, const float kfPitch); // Rotates camera
		void move(const glm::vec3 kDisplacement); // Moves camera
};