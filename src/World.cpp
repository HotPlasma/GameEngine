#include "World.h"

using std::string;
using std::ifstream;

#define COLLECTABLE_ROTATION 90.0f
#define COLLECTABLE_SPEED 1.5f

#define CAMERA_ROTATION 0.0025f
#define CAMERA_SPEED 15.0f

World::World(GLFWwindow *pWindow, sf::Vector2i windowSize)
{
	// Sets members with input
	m_pWindow = pWindow;
	m_windowSize = windowSize;

	m_camera.setAspectRatio((float)windowSize.x / windowSize.y);

	// Sets Camera initital position 
	m_camera.setPosition(glm::vec3(0.0f, 5.0f, 0.0f));

	// Creates a skybox
	m_pSkybox = std::shared_ptr<Model>(new Model());
	m_pSkybox->setName("Skybox");
	m_pSkybox->setFileLocation("assets/models/skybox.obj");
	m_pSkybox->setTextureLocation("assets/textures/skybox.bmp");
	m_pSkybox->setPosition(m_camera.getPosition());
	m_pSkybox->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
	m_pSkybox->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	m_pSkybox->setMaterial(1);

	// Loads Model so it's ready for drawing
	m_pSkybox->loadModel();
	// Initialises Model
	m_pSkybox->initModel();
}

void World::initScene(Freetype* pOverlay)
{
	// Get the Heads up display for the scene
	m_pHUD = pOverlay;

	// Sets cursor style
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	linkShaders();

	// Stops rendered models from being transparent
	gl::Enable(gl::DEPTH_TEST);

	m_sceneReader = SceneReader("assets/scenes/Scene.xml");

	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		if (!m_sceneReader.m_modelList.at(i).isCollected()) // Draw all items except collected collectables
		{
			m_sceneReader.m_modelList[i].initModel();
		}
	}

	// Resets cursor to the center of the window
	glfwSetCursorPos(m_pWindow, getWindowSize().x*0.5f, getWindowSize().y*0.5f);
	m_mousePos = sf::Vector2f((float)getWindowSize().x*0.5f, (float)getWindowSize().y*0.5f);

	// Updates camera vision
	m_camera.updateView();
}

// Void: Called on key input event
void World::input_key(const int kiKey, const int kiAction)
{
	// If action is a key press
	if (kiAction == GLFW_PRESS)
	{
		// If Esc key pressed
		if (kiKey == GLFW_KEY_ESCAPE) m_intention = TO_MENU; // Switch to Menu Scene
	}
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


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
	{
		if (spareBatteries > 0)
		{
			if (batteryLife <= 80)
			{
				batteryLife += 20;
			    spareBatteries -= 1;
			}
		}
	}

	// Sticks the camera to y 5.0
	m_camera.setPosition(glm::vec3(m_camera.getPosition().x, 5.0f, m_camera.getPosition().z));

	//BATTERY AND SURVIVAL TIMERS
	sf::Time bTimer = batteryTimer.getElapsedTime();
	sf::Time lTimer = LevelTimer.getElapsedTime();

	//cout << bTimer.asSeconds() << endl;
	if (bTimer.asSeconds() >= 1)
	{
		if (batteryLife >= 1)
		{
			batteryLife -= 1;
			std::cout << "battery life = " << batteryLife << "%" << std::endl;
			
		}
		else
		{
			std::cout << "it was too dark, you lose" << std::endl;
		}
		batteryTimer.restart();
	}

	//cout << lTimer.asSeconds() << endl;
	if (lTimer.asSeconds() >= 1)
	{
		if (lCountdown >= 1)
		{
			lCountdown -= 1;
			lTime = std::to_string(lCountdown);
			
		}
		else
		{ 
			//std::cout << "you survived!" << std::endl;
			lTime = "You Survived!";
			//victory!}
	    }
		LevelTimer.restart();

	
}

	bLife = std::to_string(batteryLife);

	extraBatteries = std::to_string(spareBatteries);
	
	
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
			if (!m_sceneReader.m_modelList.at(i).isCollected())
			{
				// Rotates collectable
				m_sceneReader.m_modelList.at(i).setRotation(glm::vec3(0, m_sceneReader.m_modelList.at(i).getRotation().y + (COLLECTABLE_ROTATION*kfTimeElapsed), m_sceneReader.m_modelList.at(i).getRotation().z));
				
				// Get distance between player and collectable
				glm::vec3 distance = m_camera.getPosition() - m_sceneReader.m_modelList.at(i).getPosition(); // Work out distance between robot and a collectable

				// If collision with a collectable
				if (sqrtf(powf(distance.x, 2.0f) + powf(distance.z, 2.0f)) < 5) 
				{
					if (batteryLife <= 80)
					{
						batteryLife += 20;
						std::cout << "BATTERY LIFE INCREASED;" << " battery life = " << batteryLife << "%" << std::endl;
					}
					else
					{
						std::cout << "BATTERY LIFE ALREADY FULL- BATTERY STORED FOR WHEN YOU LOSE CHARGE" << std::endl;
						spareBatteries++;
						std::cout << "number of spare batteries= " << spareBatteries << std::endl;
						
					}
					// Marks it as collected
					m_sceneReader.m_modelList.at(i).setCollected(true);
				}
			}
		}
	}

	// Resets cursor position
	glfwSetCursorPos(m_pWindow, m_windowSize.x*0.5, m_windowSize.y*0.5);
}

void setLightParams(GLSLProgram *pShader, Camera *camera)
{
	pShader->setUniform("Material.Ka", glm::vec3(0.2f, 0.2f, 0.2f));
	pShader->setUniform("Material.Kd", glm::vec3(0.5f, 0.5f, 0.5f));
	pShader->setUniform("Material.Ks", glm::vec3(1.0f, 1.0f, 1.0f));
	pShader->setUniform("Material.Shininess", 20.0f);

	pShader->setUniform("Light.Intensity", glm::vec3(0.1f, 0.1f, 0.1f));
	pShader->setUniform("Light.Position", camera->getPosition());

	pShader->setUniform("Spotlight.Direction", camera->getPosition() + camera->getDirection());
	pShader->setUniform("Spotlight.CutOff", glm::cos(glm::radians(15.0f)));
	pShader->setUniform("Spotlight.OuterCutOff", glm::cos(glm::radians(22.5f)));

	//pShader->setUniform("light.direction", m_camera.getPosition().x + m_camera.getDirection().x, m_camera.getPosition().y + m_camera.getDirection().y, m_camera.getPosition().z + m_camera.getDirection().z);
	//pShader->setUniform("light.cutOff", glm::cos(glm::radians(25.5f)));
	//pShader->setUniform("light.outerCutOff", glm::cos(glm::radians(35.5f)));
	//
	//pShader->setUniform("light.ambient", 0.3f, 0.3f, 0.3f);
	//pShader->setUniform("light.diffuse", 0.5f, 0.5f, 0.5f);
	//pShader->setUniform("light.specular", 0.8f, 0.8f, 0.8f);
	//pShader->setUniform("light.constant", 1.0f);
	//pShader->setUniform("light.linear", 0.09f);
	//pShader->setUniform("light.quadratic", 0.032f);

	//if (m_sceneReader.m_modelList.at(i).getMaterial() == 1) //Wooden material
	//{
	//	m_spotlightShader.setUniform("Id", 0.5f, 0.5f, 0.5f);
	//	m_spotlightShader.setUniform("Is", 0.4f, 0.4f, 0.4f);
	//	m_spotlightShader.setUniform("Rd", 0.6f, 0.6f, 0.6f);
	//	m_spotlightShader.setUniform("Rs", 0.3f, 0.3f, 0.3f);
	//}
	//else if (m_sceneReader.m_modelList.at(i).getMaterial() == 2) //Metal material
	//{
	//	m_spotlightShader.setUniform("Id", 0.7f, 0.7f, 0.7f);
	//	m_spotlightShader.setUniform("Is", 0.5f, 0.5f, 0.5f);
	//	m_spotlightShader.setUniform("Rd", 0.8f, 0.8f, 0.8f);
	//	m_spotlightShader.setUniform("Rs", 0.8f, 0.8f, 0.8f);
	//}
	//else if (m_sceneReader.m_modelList.at(i).getMaterial() == 3) //Deer material
	//{
	//	m_spotlightShader.setUniform("Id", 0.5f, 0.5f, 0.5f);
	//	m_spotlightShader.setUniform("Is", 0.3f, 0.3f, 0.3f);
	//	m_spotlightShader.setUniform("Rd", 0.7f, 0.7f, 0.7f);
	//	m_spotlightShader.setUniform("Rs", 0.5f, 0.5f, 0.5f);
	//}
	//else if (m_sceneReader.m_modelList.at(i).getMaterial() == 4) //Skybox material
	//{
	//	m_spotlightShader.setUniform("Id", 1.0f, 1.0f, 1.0f);
	//	m_spotlightShader.setUniform("Is", 0.0f, 0.0f, 0.0f);
	//	m_spotlightShader.setUniform("Rd", 0.0f, 0.0, 0.0f);
	//	m_spotlightShader.setUniform("Rs", 0.0f, 0.0f, 0.0f);
	//}
}

void World::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// WORLD
	// Activates World shader
	m_spotlightShader.use();

	// Passes texture map to shader
	m_spotlightShader.setUniform("TextureMap", 0);
	// Sets shader view and projection
	m_spotlightShader.setUniform("V", m_camera.getView());
	m_spotlightShader.setUniform("P", m_camera.getProjection());
	// Configures lighting
	setLightParams(&m_spotlightShader, &m_camera);

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
		if (!m_sceneReader.m_modelList.at(i).isCollected())
		{
			// Renders the Model
			m_sceneReader.m_modelList.at(i).render(&m_spotlightShader, transMat);
		}
	}
	
	// Activates texture shader
	m_textureShader.use();

	// Passes texture map to shader
	m_textureShader.setUniform("TextureMap", 0);
	// Sets shader view and projection
	m_textureShader.setUniform("V", m_camera.getView());
	m_textureShader.setUniform("P", m_camera.getProjection());
	m_textureShader.setUniform("Light.Intensity", glm::vec3(0.1f, 0.1f, 0.1f));

	// Updates Skybox position
	m_pSkybox->setPosition(m_camera.getPosition());
	// Renders Skybox
	m_pSkybox->render(&m_textureShader, glm::mat4(1.0f));

	// Activates FreeType shader
	m_freeType.use();
	// Configures projection
	m_freeType.setUniform("projection", glm::ortho(0.0f, float(m_windowSize.x), 0.f, float(m_windowSize.y)));
	// Renders placeholder text to HUD
	m_pHUD->renderText(m_freeType.getHandle(), "spare batteries-   ('E' to use for 20% battery life)", 30.f, 70.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));
	m_pHUD->renderText(m_freeType.getHandle(), extraBatteries, 300.f, 70.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));
	m_pHUD->renderText(m_freeType.getHandle(), bLife, 1820.f, 70.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));
	m_pHUD->renderText(m_freeType.getHandle(), lTime, 1600.f, 1000.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));
	m_pHUD->renderText(m_freeType.getHandle(), "Battery Life-   %", 1610.f, 70.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));

}