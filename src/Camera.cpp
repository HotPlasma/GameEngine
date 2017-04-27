#include "Camera.h"

#define CAMERA_ROTATION 0.0025f
#define CAMERA_SPEED 900.0f

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

void Camera::processInput(const float kfTimeElapsed, const sf::Vector2f kMousePos, const sf::Vector2i kWindowSize)
{
	// Calculates the mouse movement
	sf::Vector2f delta(kMousePos - sf::Vector2f(kWindowSize.x * 0.5f, kWindowSize.y * 0.5f));

	rotate(delta.x*CAMERA_ROTATION, delta.y*CAMERA_ROTATION);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		move(glm::vec3(0.0f, 0.0f, -CAMERA_SPEED*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		move(glm::vec3(-CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		move(glm::vec3(0.0f, 0.0f, CAMERA_SPEED*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		move(glm::vec3(CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}
}