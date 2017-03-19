#include "World.h"

using std::string;
using std::ifstream;

World::World(sf::Vector2i windowSize)
{
	m_windowSize = windowSize;
}

void World::initScene(GLFWwindow *pWindow)
{
	// Enables OpenGL depth testing
	gl::Enable(gl::DEPTH_TEST);

	// Window pointer added to member
	m_pWindow = pWindow;

	// Sets the cursor to be hidden
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	linkMe(1, 2);
}

void World::setMousePos(sf::Vector2i mousepos)
{
	m_mousePos = mousepos;
}

void World::linkMe(GLint vertShader, GLint fragShader)
{
	try 
	{
		m_WorldShader.compileShader("Shaders/shader.vert");
		m_WorldShader.compileShader("Shaders/shader.frag");
		m_WorldShader.link();
		m_WorldShader.validate();
		m_WorldShader.use();
	}
	catch (GLSLProgramException & e) 
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void World::update(float t)
{
	// Creates camera and view using MVP

	// Allows first person view changing with mouse movement
	sf::Vector2i windowOrigin(m_windowSize.x * 0.5, m_windowSize.y * 0.5); // Middle of the screen

	float fYAngle = (windowOrigin - m_mousePos).x / 1000.0f;
	float fZAngle = (windowOrigin - m_mousePos).y / 1000.0f;

	m_camera.processUserInput(fYAngle, fZAngle); // Send mouse position data to be processed in order to move camera

	glm::mat4 V = glm::lookAt
	(
		m_camera.getPosition(), // Camera position
		m_camera.getView(), // Looking at
		m_camera.getUp() // Up
	);

	//std::cout << "X: " << FirstPersonView.GetCameraPos().x << " Y: " << FirstPersonView.GetCameraPos().y << " Z: " << FirstPersonView.GetCameraPos().z << std::endl;

	glm::mat4 P = glm::perspective(m_camera.getFOV(), (float)m_windowSize.x/m_windowSize.y, 1.f, 500.f); // Sets FOV and vision culls

	// Send data to shader for processing

	//GLuint viewMatrixID = gl::GetUniformLocation(m_programHandle, "mView");
	//GLuint projectionMatrixID = gl::GetUniformLocation(m_programHandle, "mProjection");

	
	//gl::UniformMatrix4fv(viewMatrixID, 1, gl::FALSE_, glm::value_ptr(V));
	m_WorldShader.setUniform("mView", V);
	//gl::UniformMatrix4fv(projectionMatrixID, 1, gl::FALSE_, glm::value_ptr(P));
	m_WorldShader.setUniform("mProjection", P);

	// Makes collectables rotate and bounce
	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		if (m_sceneReader.m_modelList.at(i).isCollectable()) // check if collectable
		{
			if (!m_sceneReader.m_modelList.at(i).getCollected()) // if collectable then slowly rotate and bob up and down
			{
				if (m_sceneReader.m_modelList.at(i).getPosition().y >= -2)
				{
					m_collectableSpeed = glm::vec3(0, -0.03, 0);
				}
				
				else if (m_sceneReader.m_modelList.at(i).getPosition().y <= -4)
				{
					m_collectableSpeed = glm::vec3(0, 0.03, 0);
				}

				//Set positions & rotations
				m_sceneReader.m_modelList.at(i).setPosition(m_sceneReader.m_modelList.at(i).getPosition() + m_collectableSpeed );
				m_sceneReader.m_modelList.at(i).setRotation(glm::vec3(45, m_sceneReader.m_modelList.at(i).getRotation().y + 5 * t, m_sceneReader.m_modelList.at(i).getRotation().z));
				// Get distance between player and collectable
				glm::vec3 distance = m_camera.getPosition() - m_sceneReader.m_modelList.at(i).getPosition(); // Work out distance between robot and a collectable

				if (sqrtf(powf(distance.x, 2.0f) + powf(distance.z, 2.0f)) < 5) // If collision with a collectable mark it as collected and stop drawing it
				{
					m_sceneReader.m_modelList.at(i).setCollected(true);
				}
			}
		}
	}

	// Resets cursor to the center of the window after cursor event
	glfwSetCursorPos(m_pWindow, getWindowSize().x*0.5, getWindowSize().y*0.5);
}

void World::modelUpdate(int index)
{
	//GLuint modelMatrixID = gl::GetUniformLocation(m_programHandle, "mModel");
	m_WorldShader.setUniform("mModel", m_sceneReader.m_modelList.at(index).m_M);
	//gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, glm::value_ptr(m_sceneReader.m_modelList.at(index).m_M));
}

void World::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	

	// Render all models in current scene
	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		if (!m_sceneReader.m_modelList.at(i).getCollected()) // Draw all items except collected collectables
		{
			m_sceneReader.m_modelList.at(i).buffer();
			modelUpdate(i);
			m_sceneReader.m_modelList.at(i).render();
		}
	}
}
