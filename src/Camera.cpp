#include "Camera.h"

Camera::Camera()
{
	m_fFOV = 90.0f;

	// Sets Pos, View and Up to default values
	m_position = glm::vec3(1.0f, 1.0f, 1.0);
	m_view = glm::vec3(0.0f, 1.0f, 0.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::processUserInput(float fYAngle, float fZAngle) 
{
	glm::vec3 viewVec = m_view - m_position;
	
	// Allows looking around with the mouse
	m_view.x = m_position.x + (float)cosf(-fYAngle) * viewVec.x - sinf(-fYAngle) * viewVec.z;
	m_view.y += fZAngle * 2.0f; // Allows looking up/down in the y axis
	m_view.z = m_position.z + (float)sinf(-fYAngle) * viewVec.x + cosf(-fYAngle) * viewVec.z;
	
	float fMovementSpeed = 0.1f; // Fraction which limits movement speed
	
							// Sprint functionality
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
	{
		fMovementSpeed = 0.5;
	}
	else
	{
		fMovementSpeed = 0.1;
	}

	// Forwards movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_position.x += ((float)cosf(-fYAngle) * viewVec.x - sinf(-fYAngle) * viewVec.z) * fMovementSpeed;
		m_position.z += ((float)sinf(-fYAngle) * viewVec.x + cosf(-fYAngle) * viewVec.z) * fMovementSpeed;
		m_view.x += ((float)cosf(-fYAngle) * viewVec.x - sinf(-fYAngle) * viewVec.z) * fMovementSpeed;
		m_view.z += ((float)sinf(-fYAngle) * viewVec.x + cosf(-fYAngle) * viewVec.z) * fMovementSpeed;
	}
	// Backwards movement
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		 m_position.x -= ((float)cosf(-fYAngle) * viewVec.x - sinf(-fYAngle) * viewVec.z) * fMovementSpeed;
		 m_position.z -= ((float)sinf(-fYAngle) * viewVec.x + cosf(-fYAngle) * viewVec.z) * fMovementSpeed;
		 m_view.x -= ((float)cosf(-fYAngle) * viewVec.x - sinf(-fYAngle) * viewVec.z)* fMovementSpeed;
		 m_view.z -= ((float)sinf(-fYAngle) * viewVec.x + cosf(-fYAngle) * viewVec.z) * fMovementSpeed;
	}
	
	 // Strafing right movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_position.z += ((float)cosf(-fYAngle) * viewVec.x - sinf(-fYAngle) * viewVec.z) * fMovementSpeed;
		m_position.x -= ((float)sinf(-fYAngle) * viewVec.x + cosf(-fYAngle) * viewVec.z) * fMovementSpeed;
		m_view.z += ((float)cosf(-fYAngle) * viewVec.x - sinf(-fYAngle) * viewVec.z)* fMovementSpeed;
		m_view.x -= ((float)sinf(-fYAngle) * viewVec.x + cosf(-fYAngle) * viewVec.z) * fMovementSpeed;
	}

	// Strafing left movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_position.z -= ((float)cosf(-fYAngle) * viewVec.x - sinf(-fYAngle) * viewVec.z) * fMovementSpeed;
		m_position.x += ((float)sinf(-fYAngle) * viewVec.x + cosf(-fYAngle) * viewVec.z) * fMovementSpeed;
		m_view.z -= ((float)cosf(-fYAngle) * viewVec.x - sinf(-fYAngle) * viewVec.z)* fMovementSpeed;
		m_view.x += ((float)sinf(-fYAngle) * viewVec.x + cosf(-fYAngle) * viewVec.z) * fMovementSpeed;
	}
}