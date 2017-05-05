#include "Camera.h"

// World coordinate System Axes
const glm::vec3 WORLDX = glm::vec3(1, 0, 0);
const glm::vec3 WORLDY = glm::vec3(0, 1, 0);
const glm::vec3 WORLDZ = glm::vec3(0, 0, 1);

Camera::Camera()
{
	// Sets axis to world
	m_xAxis = WORLDX;
	m_yAxis = WORLDY;
	m_zAxis = WORLDZ;

	// Sets orientation to 
	m_orientation = glm::quat(1.0, 0.0, 0.0, 0.0); 

	m_fFOV = 90.0f; // Camera Field of View
	m_fNear = 1.0f; // Near culling distance
	m_fFar = 1000.0f; // Far culling distance

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
	//Extract camera orientation and convert to Euler angles
	/*glm::vec3 eulerFromQuat;
	eulerFromQuat = glm::eulerAngles(m_orientation);



	glm::vec3 cameraLookAt = glm::vec3(glm::acos(glm::radians(eulerFromQuat.x)), glm::asin(glm::radians(eulerFromQuat.y)), glm::acos(glm::radians(eulerFromQuat.z)));




	return cameraLookAt;*/

	glm::vec4 test(1.0f, 1.0f, 1.0f, 1.0f);

	test = glm::vec4(test * m_view);

	glm::vec3 test2(test.x, test.y, test.z);

	return test2;
}
