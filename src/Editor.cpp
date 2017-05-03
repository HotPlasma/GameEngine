
#include "Editor.h"

#define HAND_ROTATION 180.0f
#define HAND_SPEED 25.0f

#define CAMERA_ROTATION 0.0025f
#define CAMERA_SPEED 0.01f
#define CAMERA_ZOOM 5.0f

// Constructor
Editor::Editor(GLFWwindow *pWindow, sf::Vector2i windowSize)
{
	// Sets members with input
	m_pWindow = pWindow;
	m_windowSize = windowSize;

	// Updates Camera aspect ratio
	m_camera.setAspectRatio((float)windowSize.x / windowSize.y);

	// TEMPORARY - Need to read in a list of models to use
	// Creates a tree Model
	std::shared_ptr<Model> model = std::shared_ptr<Model>(new Model());
	model->setName("Tree");
	model->setFileLocation("assets/models/deadtree.obj");
	model->setTextureLocation("assets/textures/bark.bmp");
	model->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	model->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	model->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	model->setMaterial(1);
	// Populates the ModelSelection member with Models
	m_pModelSelection.push_back(model);

	// Creates a stump Model
	model = std::shared_ptr<Model>(new Model());
	model->setName("Stump");
	model->setFileLocation("assets/models/stump.obj");
	model->setTextureLocation("assets/textures/stump.bmp");
	model->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	model->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	model->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	model->setMaterial(1);
	// Populates the ModelSelection member with Models
	m_pModelSelection.push_back(model);

	// For every Model
	for (std::shared_ptr<Model> pModel : m_pModelSelection)
	{
		// Loads Model so it's ready for drawing
		pModel->loadModel();

		// Initialises Models
		pModel->initModel();
	}

	// Sets the first Model in the selection to selected
	m_pSelectedModel = m_pModelSelection.front();
}

// Void: Initialises the Editor Scene
void Editor::initScene(Freetype* pOverlay)
{
	// Get the Heads up display for the scene
	m_pHUD = pOverlay;

	// Enables OpenGL depth testing
	gl::Enable(gl::DEPTH_TEST);

	linkShaders();
}

// Void: Called on keyPress event
void Editor::keyPress(const int kiKey)
{
	// If Spacebar is pressed
	if (kiKey == GLFW_KEY_SPACE)
	{
		// Model is added to a vector of placed Models
		m_pModels.push_back(std::shared_ptr<Model>(new Model(*m_pSelectedModel.get())));
	}

	// If R key is pressed
	if (kiKey == GLFW_KEY_R)
	{
		// Resets the hand rotation vector
		m_handRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// TEMPORARY
	// If 1 key is pressed
	if (kiKey == GLFW_KEY_1)
		m_pSelectedModel = m_pModelSelection.at(0);
	// If 2 key is pressed
	if (kiKey == GLFW_KEY_2)
		m_pSelectedModel = m_pModelSelection.at(1);

	// If F5 key is pressed
	if (kiKey == GLFW_KEY_F5)
	{
		// Saves Scene to file
		save();
	}
}

// Void: Called on mouseScroll event
void Editor::mouseScroll(const double kdDelta)
{
	// Move in the Z axis 
	m_camera.move(glm::vec3(0.0f, 0.0f, -CAMERA_ZOOM*kdDelta));
}

// Void: Updates the Editor with elapsed time
void Editor::update(const float kfTimeElapsed)
{
	/////////////////// USER DISPLAY PROCESSING ///////////////////
	// Calculates the mouse movement
	sf::Vector2f delta(m_mousePos - m_lastMousePos);

	// If LMButton is down
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		// Applies movement of mouse to Camera rotation
		m_camera.rotate(-delta.x*CAMERA_ROTATION, -delta.y*CAMERA_ROTATION);
	}

	// If RMButton is down
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		// Applies movement of mouse to Camera movement
		m_camera.move(glm::vec3(-delta.x*CAMERA_SPEED, -delta.y*CAMERA_SPEED, 0.0f));
	}

	/////////////////// MODEL MANIPULAITON ///////////////////
	/////////////////// TRANSLATION ///////////////////
	/////////////////// X ///////////////////
	// If A key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		m_handPosition += m_camera.getXAxis() * glm::vec3(-HAND_SPEED*kfTimeElapsed, 0.0f, -HAND_SPEED*kfTimeElapsed);
	// If D key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		m_handPosition += m_camera.getXAxis() * glm::vec3(HAND_SPEED*kfTimeElapsed, 0.0f, HAND_SPEED*kfTimeElapsed);

	/////////////////// Y ///////////////////
	// If PageUp key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::PageUp))
		m_handPosition += m_camera.getYAxis() * glm::vec3(0.0f, HAND_SPEED*kfTimeElapsed, 0.0f);
	// If PageDown key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::PageDown))
		m_handPosition += m_camera.getYAxis() * glm::vec3(0.0f, -HAND_SPEED*kfTimeElapsed, 0.0f);

	/////////////////// Z ///////////////////
	// If W key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) 
		m_handPosition += m_camera.getZAxis() * glm::vec3(-HAND_SPEED*kfTimeElapsed, 0.0f, -HAND_SPEED*kfTimeElapsed);
	// If S key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		m_handPosition += m_camera.getZAxis() * glm::vec3(HAND_SPEED*kfTimeElapsed, 0.0f, HAND_SPEED*kfTimeElapsed);

	/////////////////// ROTATION ///////////////////
	/////////////////// X ///////////////////
	// If Up arrow is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		m_handRotation += glm::vec3(-HAND_ROTATION*kfTimeElapsed, 0.0f, 0.0f);
	// If Down arrow is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		m_handRotation += glm::vec3(HAND_ROTATION*kfTimeElapsed, 0.0f, 0.0f);

	/////////////////// Y ///////////////////
	// If Q key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		m_handRotation += glm::vec3(0.0f, -HAND_ROTATION*kfTimeElapsed, 0.0f);
	// If E key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
		m_handRotation += glm::vec3(0.0f, HAND_ROTATION*kfTimeElapsed, 0.0f);

	/////////////////// Z ///////////////////
	// If Left arrow is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		m_handRotation += glm::vec3(0.0f, 0.0f, HAND_ROTATION*kfTimeElapsed);
	// If Right arrow is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		m_handRotation += glm::vec3(0.0f, 0.0f, -HAND_ROTATION*kfTimeElapsed);

	//// Sets Model position to hand position
	m_pSelectedModel->setPosition(m_handPosition);
	//// Sets Model rotation to hand position
	m_pSelectedModel->setRotation(m_handRotation);

	// Sets last cursor position
	m_lastMousePos = m_mousePos;
}

// Void: Renders the Editor to display
void Editor::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Activates World shader
	m_spotlightShader.use();

	// Configures shader
	glm::mat4 model = glm::mat4(1.0);
	mat4 mv = m_camera.getView() * model;
	m_spotlightShader.setUniform("ModelViewMatrix", mv);
	m_spotlightShader.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	m_spotlightShader.setUniform("MVP", m_camera.getProjection() * mv);
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));
	m_spotlightShader.setUniform("M", model);
	m_spotlightShader.setUniform("V", m_camera.getView());
	m_spotlightShader.setUniform("P", m_camera.getProjection());

	// Renders the Model in hand
	m_pSelectedModel->buffer();

	// Passes Model transformation data to shader
	m_spotlightShader.setUniform("M", m_pSelectedModel->m_M);

	// Renders Model
	m_pSelectedModel->render();

	// Render all Models in the Scene
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// Buffers the Model
		pModel->buffer();
		
		// Passes Model transformation data to shader
		m_spotlightShader.setUniform("M", pModel->m_M);
	
		// Renders Model
		pModel->render();
	}

	// Activates FreeType shader
	m_freeType.use();
	// Configures projection
	m_freeType.setUniform("projection", glm::ortho(0.0f, float(m_windowSize.x), 0.f, float(m_windowSize.y)));

	// Defines position string
	std::string sPos; sPos += "Model Position: x("; sPos += std::to_string(m_handPosition.x); sPos += ") y("; sPos += std::to_string(m_handPosition.y); sPos += ") z("; sPos += std::to_string(m_handPosition.z); sPos += ")";
	// Outputs position string to HUD
	m_pHUD->RenderText(m_freeType.getHandle(), sPos, 100.f, 100.f, 1.0f, glm::vec3(0.7, 0.3f, 0.3f));

	// Defines position string
	std::string sRot; sRot += "Model Rotation: x("; sRot += std::to_string(m_handRotation.x); sRot += ") y("; sRot += std::to_string(m_handRotation.y); sRot += ") z("; sRot += std::to_string(m_handRotation.z); sRot += ")";
	// Outputs position string to HUD
	m_pHUD->RenderText(m_freeType.getHandle(), sRot, 100.f, 75.f, 1.0f, glm::vec3(0.3, 0.7f, 0.3f));
}

// Void: Saves the Scene to XML file
void Editor::save()
{
	std::cerr << "[EDITOR] Saving to file..." << std::endl;

	// Defines new XML Document
	tinyxml2::XMLDocument document;

	// Creates an XML declaration
	//tinyxml2::XMLDeclaration* declaration = new tinyxml2::XMLDeclaration("1.0", "", "");

	// Creates <Scene> within the document
	tinyxml2::XMLNode* pScene = document.NewElement("Scene");
	document.InsertFirstChild(pScene);

	// For all Models in the Editor
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// Defines new element for <Object>
		tinyxml2::XMLNode* pObject = document.NewElement("Object");
		pScene->InsertEndChild(pObject);

		// Defines new element for <Name>
		tinyxml2::XMLElement* pName = document.NewElement("Name");
		// Sets <Name> value to the Model name
		pName->SetText(pModel->getName().c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pName);

		// Defines new element for <OBJLocation>
		tinyxml2::XMLElement* pObjLoc = document.NewElement("OBJLocation");
		// Sets <OBJLocation> value to the Model file location
		pObjLoc->SetText(pModel->getFileLocation().c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pObjLoc);

		// Defines new element for <TexLocation>
		tinyxml2::XMLElement* pTexLoc = document.NewElement("TexLocation");
		// Sets <TexLocation> value to the Model texture location
		pTexLoc->SetText(pModel->getTexFileLocation().c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pTexLoc);

		// Defines new element for <Translation>
		tinyxml2::XMLElement* pTrans = document.NewElement("Translation");
		// Defines string for Translation data
		std::string sTData = std::to_string(pModel->getPosition().x); sTData += " "; sTData += std::to_string(pModel->getPosition().y); sTData += " "; sTData += std::to_string(pModel->getPosition().z);
		// Sets <Translation> value to the Model position
		pTrans->SetText(sTData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pTrans);

		// Defines new element for <Rotation>
		tinyxml2::XMLElement* pRot = document.NewElement("Rotation");
		// Defines string for Rotation data
		std::string sRData = std::to_string(pModel->getRotation().x); sRData += " "; sRData += std::to_string(pModel->getRotation().y); sRData += " "; sRData += std::to_string(pModel->getRotation().z);
		// Sets <Rotation> value to Model rotation
		pRot->SetText(sRData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pRot);

		// Defines new element for <Scale>
		tinyxml2::XMLElement* pScale = document.NewElement("Scale");
		// Defines string for Scale data
		std::string sSData = std::to_string(pModel->getScale().x); sSData += " "; sSData += std::to_string(pModel->getScale().y); sSData += " "; sSData += std::to_string(pModel->getScale().z);
		// Sets <Scale> value to the Model scale
		pScale->SetText(sSData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pScale);

		// Defines new element for <Material>
		tinyxml2::XMLElement* pMaterial = document.NewElement("Material");
		// Sets <Material> value to Model scale
		pMaterial->SetText(pModel->getMaterial());
		// Inserts element into <Object>
		pObject->InsertEndChild(pMaterial);

		// Defines new element for <Collectable>
		tinyxml2::XMLElement* pCollectable = document.NewElement("Collectable");
		// Sets <Collectable> value to Model name
		pCollectable->SetText(pModel->isCollectable());
		// Inserts element into <Object>
		pObject->InsertEndChild(pCollectable);
	}

	// Saves the document
	document.SaveFile(m_sFilepath.c_str());

	std::cerr << "[EDITOR] File saved." << std::endl;
}