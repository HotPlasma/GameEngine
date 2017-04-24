#include "World.h"

using std::string;
using std::ifstream;

#define MOVE_VELOCITY 50.0f
#define ROTATE_VELOCITY 0.0025f

World::World(sf::Vector2i windowSize)
{
	m_windowSize = windowSize;

	m_camera.setAspectRatio((float)windowSize.x / windowSize.y);
}

void World::initScene(Freetype* pOverlay)
{
	m_pHUD = pOverlay; // Get the Heads up display for the scene

	linkShaders();
	// Stops rendered models from being transparent
	gl::Enable(gl::DEPTH_TEST);

	m_sceneReader = SceneReader("assets/scenes/Scene.xml");

	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		if (!m_sceneReader.m_modelList.at(i).getCollected()) // Draw all items except collected collectables
		{
			m_sceneReader.m_modelList[i].initModel();
		}
		/*world.ModelList[i].DrawModel(true, true);*/
	}
}

void World::setMousePos(GLFWwindow *pWindow, sf::Vector2f mousepos)
{
	m_pWindow = pWindow;
	m_mousePos = mousepos;
}

void World::linkShaders()
{
	try
	{
		// Shader which allows first person camera and textured objects
		m_worldShader.compileShader("Shaders/shader.vert");
		m_worldShader.compileShader("Shaders/shader.frag");
		m_worldShader.link();
		m_worldShader.validate();
		m_worldShader.use();
	}
	catch (GLSLProgramException & e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	try
	{
		// Shader which allows heads up display
		m_freeType.compileShader("Shaders/freetype.vert");
		m_freeType.compileShader("Shaders/freetype.frag");
		m_freeType.link();
		m_freeType.validate();
		m_freeType.use();
	}
	catch (GLSLProgramException & e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void World::setMatrices(GLSLProgram * pShader, mat4 model, mat4 view, mat4 projection)
{
	mat4 mv = view * model;
	pShader->setUniform("ModelViewMatrix", mv);
	pShader->setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	pShader->setUniform("MVP", projection * mv);
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));
	pShader->setUniform("M", model);
	pShader->setUniform("V", view);
	pShader->setUniform("P", projection);
}

void World::keyPress(const int kiKey)
{
	//if (kiKey == GLFW_KEY_W)
	//{
	//	m_camera.move(glm::vec3(0.0f, 0.0f, -MOVE_VELOCITY));
	//}
	//
	//if (kiKey == GLFW_KEY_A)
	//{
	//	m_camera.move(glm::vec3(-MOVE_VELOCITY, 0.0f, 0.0f));
	//}
	//
	//if (kiKey == GLFW_KEY_S)
	//{
	//	m_camera.move(glm::vec3(0.0f, 0.0f, MOVE_VELOCITY));
	//}
	//
	//if (kiKey == GLFW_KEY_D)
	//{
	//	m_camera.move(glm::vec3(MOVE_VELOCITY, 0.0f, 0.0f));
	//}
}

void World::update(float fTimeElapsed)
{
	// Calculates the mouse movement
	sf::Vector2f delta(m_mousePos - sf::Vector2f(m_windowSize.x * 0.5f, m_windowSize.y * 0.5f));

	m_camera.rotate(delta.x*ROTATE_VELOCITY, delta.y*ROTATE_VELOCITY);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, -MOVE_VELOCITY*fTimeElapsed));
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_camera.move(glm::vec3(-MOVE_VELOCITY*fTimeElapsed, 0.0f, 0.0f));
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, MOVE_VELOCITY*fTimeElapsed));
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_camera.move(glm::vec3(MOVE_VELOCITY*fTimeElapsed, 0.0f, 0.0f));
	}

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
				m_sceneReader.m_modelList.at(i).setRotation(glm::vec3(0, m_sceneReader.m_modelList.at(i).getRotation().y + 5, m_sceneReader.m_modelList.at(i).getRotation().z));
				// Get distance between player and collectable
				glm::vec3 distance = m_camera.getPosition() - m_sceneReader.m_modelList.at(i).getPosition(); // Work out distance between robot and a collectable

				if (sqrtf(powf(distance.x, 2.0f) + powf(distance.z, 2.0f)) < 5) // If collision with a collectable mark it as collected and stop drawing it
				{
					m_sceneReader.m_modelList.at(i).setCollected(true);
				}
			}
		}
	}
}

void World::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	m_worldShader.use();
	setMatices(&m_worldShader, glm::mat4(1.0f), m_camera.getView(), m_camera.getProjection());
	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		if (!m_sceneReader.m_modelList.at(i).getCollected()) // Draw all items except collected collectables
		{
			m_sceneReader.m_modelList.at(i).buffer();
			m_worldShader.setUniform("M", m_sceneReader.m_modelList.at(i).m_M);
			m_sceneReader.m_modelList.at(i).render();
		}
	}
	/*m_freeType.use();
	m_freeType.setUniform("projection", glm::ortho(0.0f, 1920.0f, 0.f, 1080.f));
	m_pHUD->RenderText(m_freeType.getHandle(), "Collectable Collected", 100.f, 100.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));*/
}