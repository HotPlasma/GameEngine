#include "Camera.h"

// World coordinate System Axes
const glm::vec3 WORLDX = glm::vec3(1, 0, 0);
const glm::vec3 WORLDY = glm::vec3(0, 1, 0);
const glm::vec3 WORLDZ = glm::vec3(0, 0, 1);

Camera::Camera()
{
	m_fFOV = 90.0f;
	m_fNear = 1.0f;
	m_fFar = 1000.0f;

	// Sets Position default value
	m_position = glm::vec3(1.0f, 1.0f, 1.0f);
}

glm::quat fromAxisAngle(glm::vec3 axis, float angle)
{
	glm::quat rotation;

	// Applies the angle to each axis multiplied by the axis vec3 for 1/0
	rotation.w = cos(angle / 2);
	rotation.x = sin(angle / 2) * axis.x;
	rotation.y = sin(angle / 2) * axis.y;
	rotation.z = sin(angle / 2) * axis.z;

	return rotation;
}

void Camera::rotate(const float kfYaw, const float kfPitch)
{
	m_orientation = glm::normalize(fromAxisAngle(WORLDX, kfPitch) * m_orientation);
	m_orientation = glm::normalize(m_orientation * fromAxisAngle(WORLDY, kfYaw));

	updateView();
}

void Camera::move(const glm::vec3 kDisplacement)
{
	m_position += m_xAxis * kDisplacement.x;
	m_position += m_yAxis * -kDisplacement.y;
	m_position += m_zAxis * kDisplacement.z;

	// Now call updateView()
	updateView();
}

void Camera::updateView()
{
	// Sets camera projection
	m_projection = glm::perspective(m_fFOV, m_fAspectRatio, m_fNear, m_fFar);

	// Construct the view matrix from orientation quaternion and position vector

	// First get the matrix from the 'orientaation' Quaternion
	// This deals with the rotation and scale part of the view matrix
	m_view = glm::mat4_cast(m_orientation); // Rotation and Scale

	// Extract the camera coordinate axes from this matrix
	m_xAxis = glm::vec3(m_view[0][0], m_view[1][0], m_view[2][0]);
	m_yAxis = glm::vec3(m_view[0][1], m_view[1][1], m_view[2][1]);
	m_zAxis = glm::vec3(m_view[0][2], m_view[1][2], m_view[2][2]);

	// And use this and current camera position to set the translate part of the view matrix
	m_view[3][0] = -glm::dot(m_xAxis, m_position); //Translation x
	m_view[3][1] = -glm::dot(m_yAxis, m_position); //Translation y
	m_view[3][2] = -glm::dot(m_zAxis, m_position); //Translation z
}

glm::vec3 Camera::getDirection()
{
	// WRONG
	return glm::vec3(m_orientation.x, m_orientation.y, m_orientation.z);
}
