#include "World.h"

using std::string;
using std::ifstream;

#define COLLECTABLE_ROTATION 90.0f
#define COLLECTABLE_SPEED 7.5f

#define CAMERA_ROTATION 0.0025f
#define CAMERA_SPEED 50.0f

World::World(GLFWwindow *pWindow, sf::Vector2i windowSize)
{
	m_pWindow = pWindow;

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

	//m_pHUD->LoadHUDImage("assets/textures/Flag_of_Wales.png", vec3(500.0f, 500.0f, 0.0f), 0.0f, vec3(1.0f, 1.0f, 1.0f), TRUE);
}

void World::setLightParamaters(GLSLProgram *pShader, int i)
{
	vec3 worldLight = vec3(/*160.0f, 60.0f, -70.0f*/ 20.0f, 10.0f, 0.0f);
	pShader->setUniform("lightCutOff", glm::cos(glm::radians(50.0f)));
	pShader->setUniform("ligthOuterCutOff", glm::cos(glm::radians(56.0f)));
	pShader->setUniform("spotlightDriection", 0.0f, -0.1f, 0.0f /*m_camera.getDirection()*/);

	//set the ambient lighting for the scene
	pShader->setUniform("Ia", 0.2f, 0.2f, 0.2f);

	pShader->setUniform("lightPosition", /*m_camera.getPosition()*/worldLight);

	if (m_sceneReader.m_modelList.at(i).getMaterial() == 1) //Wooden material
	{
		m_spotlightShader.setUniform("Id", 0.5f, 0.5f, 0.5f);
		m_spotlightShader.setUniform("Is", 0.4f, 0.4f, 0.4f);
		m_spotlightShader.setUniform("Rd", 0.6f, 0.6f, 0.6f);
		m_spotlightShader.setUniform("Rs", 0.3f, 0.3f, 0.3f);
	}
	else if (m_sceneReader.m_modelList.at(i).getMaterial() == 2) //Metal material
	{
		m_spotlightShader.setUniform("Id", 0.7f, 0.7f, 0.7f);
		m_spotlightShader.setUniform("Is", 0.5f, 0.5f, 0.5f);
		m_spotlightShader.setUniform("Rd", 0.8f, 0.8f, 0.8f);
		m_spotlightShader.setUniform("Rs", 0.8f, 0.8f, 0.8f);
	}
	else if (m_sceneReader.m_modelList.at(i).getMaterial() == 3) //Deer material
	{
		m_spotlightShader.setUniform("Id", 0.5f, 0.5f, 0.5f);
		m_spotlightShader.setUniform("Is", 0.3f, 0.3f, 0.3f);
		m_spotlightShader.setUniform("Rd", 0.7f, 0.7f, 0.7f);
		m_spotlightShader.setUniform("Rs", 0.5f, 0.5f, 0.5f);
	}
	else if (m_sceneReader.m_modelList.at(i).getMaterial() == 4) //Skybox material
	{
		m_spotlightShader.setUniform("Id", 1.0f, 1.0f, 1.0f);
		m_spotlightShader.setUniform("Is", 0.0f, 0.0f, 0.0f);
		m_spotlightShader.setUniform("Rd", 0.0f, 0.0, 0.0f);
		m_spotlightShader.setUniform("Rs", 0.0f, 0.0f, 0.0f);
	}
}

void World::setMatrices(GLSLProgram * pShader, const mat4 kModel, const mat4 kView, const mat4 kProjection)
{
	mat4 mv = kView * kModel;
	pShader->setUniform("ModelViewMatrix", mv);
	pShader->setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	pShader->setUniform("MVP", kProjection * mv);
	mat3 normMat = glm::transpose(glm::inverse(mat3(kModel)));
	pShader->setUniform("M", kModel);
	pShader->setUniform("V", kView);
	pShader->setUniform("P", kProjection);
}

void World::update(const float kfTimeElapsed)
{
	/////////////////// USER DISPLAY PROCESSING ///////////////////
	// Calculates the mouse movement
	sf::Vector2f delta(m_mousePos - sf::Vector2f(m_windowSize.x * 0.5f, m_windowSize.y * 0.5f));

	m_camera.rotate(delta.x*CAMERA_ROTATION, delta.y*CAMERA_ROTATION);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, -CAMERA_SPEED*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_camera.move(glm::vec3(-CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, CAMERA_SPEED*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_camera.move(glm::vec3(CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}

	// Sticks the camera to y 0.0
	m_camera.setPosition(glm::vec3(m_camera.getPosition().x, 0.0f, m_camera.getPosition().z));

	/////////////////// COLLECTABLE BOBBING ///////////////////
	// If collectables are moving up and offset is greater than upper bound
	if (m_collectGoingUp && m_collectYOffset >= m_collectBounds.upper())
	{
		// Move collectable down
		m_collectGoingUp = false;
	}

	// If collectables are moving down and offset is less than lower bound
	if (!m_collectGoingUp && m_collectYOffset <= m_collectBounds.lower())
	{
		// Move collectable up
		m_collectGoingUp = true;
	}

	// Increments offset up or down
	if (m_collectGoingUp) { m_collectYOffset += COLLECTABLE_SPEED*kfTimeElapsed; }
	else if (!m_collectGoingUp) { m_collectYOffset -= COLLECTABLE_SPEED*kfTimeElapsed; }

	// Clamps offset to bounds
	m_collectYOffset = glm::clamp(m_collectYOffset, m_collectBounds.lower(), m_collectBounds.upper());

	// For all Models
	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		// If collectable
		if (m_sceneReader.m_modelList.at(i).isCollectable())
		{
			// If not collected
			if (!m_sceneReader.m_modelList.at(i).getCollected())
			{
				// Rotates collectable
				m_sceneReader.m_modelList.at(i).setRotation(glm::vec3(0, m_sceneReader.m_modelList.at(i).getRotation().y + (COLLECTABLE_ROTATION*kfTimeElapsed), m_sceneReader.m_modelList.at(i).getRotation().z));
				
				// Get distance between player and collectable
				glm::vec3 distance = m_camera.getPosition() - m_sceneReader.m_modelList.at(i).getPosition(); // Work out distance between robot and a collectable

				// If collision with a collectable
				if (sqrtf(powf(distance.x, 2.0f) + powf(distance.z, 2.0f)) < 5) 
				{
					// Marks it as collected
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

	// WORLD
	// Activates World shader
	m_spotlightShader.use();
	setMatrices(&m_spotlightShader, glm::mat4(1.0f), m_camera.getView(), m_camera.getProjection());
	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		// Defines a transformation matrix that does nothing
		glm::mat4 transMat(1.0f);

		// If Model is collectable
		if (m_sceneReader.m_modelList.at(i).isCollectable())
		{
			// Adds the Yoffset to the transformation matrix
			transMat = glm::mat4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, m_collectYOffset, 0, 1 
			);
		}

		// If Model is not collected
		if (!m_sceneReader.m_modelList.at(i).getCollected())
		{
			// Buffers the Model
			m_sceneReader.m_modelList.at(i).buffer();
      // Configures lighting
			setLightParamaters(&m_spotlightShader, i);
			// Sets the Model transformation matrix
			m_spotlightShader.setUniform("M", m_sceneReader.m_modelList.at(i).m_M * transMat);
			// Renders the Model
			m_sceneReader.m_modelList.at(i).render();
		}
	}

	// Activates FreeType shader
	m_freeType.use();
	// Configures projection
	m_freeType.setUniform("projection", glm::ortho(0.0f, float(m_windowSize.x), 0.f, float(m_windowSize.y)));
	// Renders placeholder text to HUD
	m_pHUD->RenderText(m_freeType.getHandle(), "Placeholder", 100.f, 100.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));
}