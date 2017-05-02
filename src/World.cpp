#include "World.h"

using std::string;
using std::ifstream;

#define COLLECTABLE_ROTATION 90.0f
#define COLLECTABLE_SPEED 7.5f

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

	m_Player = Model("assets/models/Player.obj", "", glm::vec3(10, -5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 1);

	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		if (!m_sceneReader.m_modelList.at(i).getCollected()) // Draw all items except collected collectables
		{
			m_sceneReader.m_modelList[i].initModel();
		}
		/*world.ModelList[i].DrawModel(true, true);*/
	}

	m_Player.loadModel();
	m_Player.initModel();
	m_Player.setVisable(true);

	//// Initial position and orientation of the collision body
	//rp3d::Vector3 initPosition(m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);
	//rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	//rp3d::Transform transform(initPosition, initOrientation);
	//// Create a collision body in the world

	//CameraBody = m_CollisonWorld.createCollisionBody(transform);

	//rp3d::CylinderShape cylinderShape(1.0, 3.0);

	//CameraBody->addCollisionShape(&cylinderShape, rp3d::Transform::identity());

	//for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	//{

	//	rp3d::ConcaveMeshShape MeshShape = new rp3d::ConcaveMeshShape(&mPhysicsTriangleMesh);
	//	ModelBodies.resize(ModelBodies.size() + 1);
	//	ModelBodies.at(i) = m_CollisonWorld.createCollisionBody(rp3d::Transform(rp3d::Vector3(m_sceneReader.m_modelList.at(i).getPosition().x, m_sceneReader.m_modelList.at(i).getPosition().y, m_sceneReader.m_modelList.at(i).getPosition().z), rp3d::Quaternion::identity()));
	//	ModelBodies.at(i)->addCollisionShape(&MeshShape, rp3d::Transform::identity());
	//}

	//m_CollisonWorld.testCollision(CameraBody, CameraCallback);

	

	//m_CollisonWorld.
//	HUD->LoadHUDImage("assets/textures/Flag_of_Wales.png", 500.f, 500.f, -90, 30.0f);
}

void World::setMousePos(sf::Vector2f mousepos)
{
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
	catch (GLSLProgramException & e)
	{
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

	try
	{
		// Shader which allows heads up display
		m_imageType.compileShader("Shaders/image.vert");
		m_imageType.compileShader("Shaders/image.frag");
		m_imageType.link();
		m_imageType.validate();
	}
	catch (GLSLProgramException & e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
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
	// Updates Camera with user input
	m_camera.processInput(kfTimeElapsed, m_mousePos, m_windowSize);
	// Sticks the camera to y 0.0
	m_camera.setPosition(glm::vec3(m_camera.getPosition().x, 0.0f, m_camera.getPosition().z));

//	CameraBody->setTransform(rp3d::Transform(rp3d::Vector3(m_camera.getPosition().x, 0.0f, m_camera.getPosition().z), rp3d::Quaternion::identity()));

	//rp3d::Vector3 position(m_camera.getPosition().x, 0, m_camera.getPosition().z);
	//rp3d::Quaternion orientation = rp3d::Quaternion::identity();
	//rp3d::Transform newTransform(position, orientation);
	// Move the collision body
	//CameraBody->setTransform(newTransform);
	
	

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
	
	m_Player.setPosition(glm::vec3(m_camera.getPosition().x, -5, m_camera.getPosition().z));

	for (int j = 5; j < m_sceneReader.m_modelList.size(); j++)
	{
		if (m_Player.getCollisionBox().right > m_sceneReader.m_modelList.at(j).getCollisionBox().left &&
			m_Player.getCollisionBox().left < m_sceneReader.m_modelList.at(j).getCollisionBox().right &&
			m_Player.getCollisionBox().top > m_sceneReader.m_modelList.at(j).getCollisionBox().bottom &&
			m_Player.getCollisionBox().bottom < m_sceneReader.m_modelList.at(j).getCollisionBox().top &&
			m_Player.getCollisionBox().back > m_sceneReader.m_modelList.at(j).getCollisionBox().front &&
			m_Player.getCollisionBox().front < m_sceneReader.m_modelList.at(j).getCollisionBox().back)
		{
			cout << "Collision with " << m_sceneReader.m_modelList.at(j).getName() << endl;
		}
	}
	
}

void World::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// WORLD
	m_worldShader.use();
	setMatrices(&m_worldShader, glm::mat4(1.0f), m_camera.getView(), m_camera.getProjection());

	m_Player.buffer();
	m_worldShader.setUniform("M", m_Player.m_M);
	//m_Player.render();

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
			// Sets the Model transformation matrix
			m_worldShader.setUniform("M", m_sceneReader.m_modelList.at(i).m_M * transMat);
			// Renders the Model
			m_sceneReader.m_modelList.at(i).render();
		}
	}



	// HUD
	m_freeType.use();
	m_freeType.setUniform("projection", glm::ortho(0.0f, 1920.0f, 0.f, 1080.f));
	m_pHUD->RenderText(m_freeType.getHandle(), "Collectable Collected", 100.f, 100.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));
}