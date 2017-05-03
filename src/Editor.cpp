
#include "Editor.h"

#define POSITION_SPEED 12.5f
#define ROTATION_SPEED 90.0f
#define SCALE_SPEED 2.5f

#define CAMERA_ROTATION 0.0015f
#define CAMERA_SPEED 0.01f
#define CAMERA_ZOOM 3.5f

// Constructor
Editor::Editor(GLFWwindow *pWindow, const sf::Vector2i kWindowSize)
{
	// Sets members with input
	m_pWindow = pWindow;
	m_windowSize = kWindowSize;

	// Updates Camera aspect ratio
	m_camera.setAspectRatio((float)kWindowSize.x / kWindowSize.y);

	// Sets Camera initital position 
	m_camera.setPosition(glm::vec3( 0.0f, 15.0f, 40.0f));

	// TEMPORARY - Need to read in a list of models to use
	// Creates a tree Model
	std::shared_ptr<Model> pModel = std::shared_ptr<Model>(new Model());
	pModel->setName("Tree");
	pModel->setFileLocation("assets/models/deadtree.obj");
	pModel->setTextureLocation("assets/textures/bark.bmp");
	pModel->setMaterial(1);

	// Loads Model so it's ready for drawing
	pModel->loadModel();
	// Initialises Model
	pModel->initModel();
	
	// Sets the first Model in the selection to selected
	m_selection.m_pModel = pModel;

	// Sets default transformation mode
	m_transformMode = TRANSLATE;
}

// Void: Initialises the Editor Scene
void Editor::initScene(Freetype* pOverlay)
{
	// Get the Heads up display for the scene
	m_pHUD = pOverlay;

	// Enables OpenGL depth testing
	gl::Enable(gl::DEPTH_TEST);

	// Defines HUD buttons
	m_buttons.m_pCollectable = std::shared_ptr<Button>(new Button(m_windowSize.x*0.006f + 211.0f*0.5f, m_windowSize.y*0.466f + 56.0f*0.5f, "assets/UI/Editor/CollectableFalse.png", "assets/UI/Editor/CollectableTrue.png", glm::vec3(211.0f, 56.0f, 1.0f), pOverlay));
	m_buttons.m_pAI = std::shared_ptr<Button>(new Button(m_windowSize.x*0.006f + 211.0f*0.5f, m_windowSize.y*0.406f + 56.0f*0.5f, "assets/UI/Editor/AIFalse.png", "assets/UI/Editor/AITrue.png", glm::vec3(211.0f, 56.0f, 1.0f), pOverlay));
	m_buttons.m_pTranslateMode = std::shared_ptr<Button>(new Button(m_windowSize.x*0.006f + 211.0f*0.5f, m_windowSize.y*0.266f + 56.0f*0.5f, "assets/UI/Editor/Translation.png", "assets/UI/Editor/TranslationHover.png", glm::vec3(211.0f, 56.0f, 1.0f), pOverlay));
	m_buttons.m_pRotateMode = std::shared_ptr<Button>(new Button(m_windowSize.x*0.006f + 211.0f*0.5f, m_windowSize.y*0.206f + 56.0f*0.5f, "assets/UI/Editor/Rotation.png", "assets/UI/Editor/RotationHover.png", glm::vec3(211.0f, 56.0f, 1.0f), pOverlay));
	m_buttons.m_pScaleMode = std::shared_ptr<Button>(new Button(m_windowSize.x*0.006f + 211.0f*0.5f, m_windowSize.y*0.146f + 56.0f*0.5f, "assets/UI/Editor/Scale.png", "assets/UI/Editor/ScaleHover.png", glm::vec3(211.0f, 56.0f, 1.0f), pOverlay));
	m_buttons.m_pSave = std::shared_ptr<Button>(new Button(m_windowSize.x*0.006f + 211.0f*0.5f, m_windowSize.y*0.006f + 56.0f*0.5f, "assets/UI/Editor/SaveScene.png", "assets/UI/Editor/SaveSceneHover.png", glm::vec3(211.0f, 56.0f, 1.0f), pOverlay));

	linkShaders();
}

// Void: Called on key input event
void Editor::input_key(const int kiKey, const int kiAction)
{
	// If action is a key press
	if (kiAction == GLFW_PRESS)
	{
		// If Enter is pressed
		if (kiKey == GLFW_KEY_ENTER)
		{
			// Model is added to a vector of placed Models
			m_pModels.push_back(std::shared_ptr<Model>(new Model(*m_selection.m_pModel.get())));
		}

		// If R key is pressed
		if (kiKey == GLFW_KEY_R)
		{
			// If mode is Translate
			if (m_transformMode == TRANSLATE)
			{
				// Resets the hand position vector
				m_selection.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			// If mode is Rotate
			else if (m_transformMode == ROTATE)
			{
				// Resets the hand rotation vector
				m_selection.m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			// If mode is Scale
			else if (m_transformMode == SCALE)
			{
				// Resets the hand scale vector
				m_selection.m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
			}
		}
	}
}

// Void: Called on mouseButton input event
void Editor::input_button(const int kiButton, const int kiAction)
{
	// If action is a button press
	if (kiAction == GLFW_PRESS)
	{
		// If left mouse button is clicked
		if (kiButton == GLFW_MOUSE_BUTTON_LEFT)
		{
			// If Collectable button is clicked
			if (m_buttons.m_pCollectable->mouseOver(m_mousePos, m_windowSize.y))
			{
				// Toggle Collectable
				m_selection.m_bCollectable = !m_selection.m_bCollectable;

				// Switches standard and hover texture indices
				unsigned int uiTextureIndex = m_buttons.m_pCollectable->getTextureIndex();
				m_buttons.m_pCollectable->setTextureIndex(m_buttons.m_pCollectable->getHoverTextureIndex());
				m_buttons.m_pCollectable->setHoverTextureIndex(uiTextureIndex);
			}
			// If AI button is clicked
			if (m_buttons.m_pAI->mouseOver(m_mousePos, m_windowSize.y))
			{
				// Toggle AI
				m_selection.m_bAI = !m_selection.m_bAI;

				// Switches standard and hover texture indices
				unsigned int uiTextureIndex = m_buttons.m_pAI->getTextureIndex();
				m_buttons.m_pAI->setTextureIndex(m_buttons.m_pAI->getHoverTextureIndex());
				m_buttons.m_pAI->setHoverTextureIndex(uiTextureIndex);
			}
			// If TranslateMode button is clicked
			if (m_buttons.m_pTranslateMode->mouseOver(m_mousePos, m_windowSize.y))
			{
				// Mode switched to Translate
				m_transformMode = TRANSLATE;
			}
			// If RotateMode button is clicked
			if (m_buttons.m_pRotateMode->mouseOver(m_mousePos, m_windowSize.y))
			{
				// Mode switched to Rotate
				m_transformMode = ROTATE;
			}
			// If ScaleMode button is clicked
			if (m_buttons.m_pScaleMode->mouseOver(m_mousePos, m_windowSize.y))
			{
				// Mode switched to Scale
				m_transformMode = SCALE;
			}
			// If Save button is clicked
			if (m_buttons.m_pSave->mouseOver(m_mousePos, m_windowSize.y))
			{
				// Saves Scene to file
				save();
			}
		}
	}
}

// Void: Called on mouseScroll input event
void Editor::input_scroll(const double kdDelta)
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
	if (m_transformMode == TRANSLATE)
	{
		/////////////////// X ///////////////////
		// If A key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			m_selection.m_position += m_camera.getXAxis() * glm::vec3(-POSITION_SPEED*kfTimeElapsed, 0.0f, -POSITION_SPEED*kfTimeElapsed);
		// If D key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			m_selection.m_position += m_camera.getXAxis() * glm::vec3(POSITION_SPEED*kfTimeElapsed, 0.0f, POSITION_SPEED*kfTimeElapsed);

		/////////////////// Y ///////////////////
		// If Spacebar is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			m_selection.m_position += m_camera.getYAxis() * glm::vec3(0.0f, POSITION_SPEED*kfTimeElapsed, 0.0f);
		// If LShift is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
			m_selection.m_position += m_camera.getYAxis() * glm::vec3(0.0f, -POSITION_SPEED*kfTimeElapsed, 0.0f);

		/////////////////// Z ///////////////////
		// If W key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			m_selection.m_position += m_camera.getZAxis() * glm::vec3(-POSITION_SPEED*kfTimeElapsed, 0.0f, -POSITION_SPEED*kfTimeElapsed);
		// If S key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			m_selection.m_position += m_camera.getZAxis() * glm::vec3(POSITION_SPEED*kfTimeElapsed, 0.0f, POSITION_SPEED*kfTimeElapsed);
	}

	/////////////////// ROTATION ///////////////////
	if (m_transformMode == ROTATE)
	{
		/////////////////// X ///////////////////
		// If W key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			m_selection.m_rotation += glm::vec3(-ROTATION_SPEED*kfTimeElapsed, 0.0f, 0.0f);
		// If S key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			m_selection.m_rotation += glm::vec3(ROTATION_SPEED*kfTimeElapsed, 0.0f, 0.0f);

		/////////////////// Y ///////////////////
		// If Q key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
			m_selection.m_rotation += glm::vec3(0.0f, -ROTATION_SPEED*kfTimeElapsed, 0.0f);
		// If E key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
			m_selection.m_rotation += glm::vec3(0.0f, ROTATION_SPEED*kfTimeElapsed, 0.0f);

		/////////////////// Z ///////////////////
		// If A key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			m_selection.m_rotation += glm::vec3(0.0f, 0.0f, -ROTATION_SPEED*kfTimeElapsed);
		// If D key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			m_selection.m_rotation += glm::vec3(0.0f, 0.0f, ROTATION_SPEED*kfTimeElapsed);
	}

	/////////////////// SCALE ///////////////////
	if (m_transformMode == SCALE)
	{
		/////////////////// X ///////////////////
		// If W key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			m_selection.m_scale += glm::vec3(-SCALE_SPEED*kfTimeElapsed, 0.0f, 0.0f);
		// If S key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			m_selection.m_scale += glm::vec3(SCALE_SPEED*kfTimeElapsed, 0.0f, 0.0f);

		/////////////////// Y ///////////////////
		// If Spacebar is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			m_selection.m_scale += glm::vec3(0.0f, SCALE_SPEED*kfTimeElapsed, 0.0f);
		// If LShift is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
			m_selection.m_scale += glm::vec3(0.0f, -SCALE_SPEED*kfTimeElapsed, 0.0f);

		/////////////////// Z ///////////////////
		// If A key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			m_selection.m_scale += glm::vec3(0.0f, 0.0f, -SCALE_SPEED*kfTimeElapsed);
		// If D key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			m_selection.m_scale += glm::vec3(0.0f, 0.0f, SCALE_SPEED*kfTimeElapsed);
	}

	//// Sets Model position to selection value
	m_selection.m_pModel->setPosition(m_selection.m_position);
	//// Sets Model rotation to selection value
	m_selection.m_pModel->setRotation(m_selection.m_rotation);
	//// Sets Model scale to selection value
	m_selection.m_pModel->setScale(m_selection.m_scale);

	// Sets last cursor position
	m_lastMousePos = m_mousePos;

	// Checks whether buttons are hovered
	m_buttons.m_pCollectable->mouseOver(m_mousePos, m_windowSize.y);
	m_buttons.m_pAI->mouseOver(m_mousePos, m_windowSize.y);
	m_buttons.m_pTranslateMode->mouseOver(m_mousePos, m_windowSize.y);
	m_buttons.m_pRotateMode->mouseOver(m_mousePos, m_windowSize.y);
	m_buttons.m_pScaleMode->mouseOver(m_mousePos, m_windowSize.y);
	m_buttons.m_pSave->mouseOver(m_mousePos, m_windowSize.y);
}

// Void: Renders the Editor to display
void Editor::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Activates World shader
	m_worldShader.use();

	// Sets shader MVP
	glm::mat4 model = glm::mat4(1.0);
	mat4 mv = m_camera.getView() * model;
	m_worldShader.setUniform("ModelViewMatrix", mv);
	m_worldShader.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	m_worldShader.setUniform("MVP", m_camera.getProjection() * mv);
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));
	m_worldShader.setUniform("M", model);
	m_worldShader.setUniform("V", m_camera.getView());
	m_worldShader.setUniform("P", m_camera.getProjection());

	// Renders the Model in hand
	m_selection.m_pModel->buffer();

	// Passes Model transformation data to shader
	m_worldShader.setUniform("M", m_selection.m_pModel->m_M);

	// Renders Model
	m_selection.m_pModel->render();

	// Render all Models in the Scene
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// Buffers the Model
		pModel->buffer();
		
		// Passes Model transformation data to shader
		m_worldShader.setUniform("M", pModel->m_M);
	
		// Renders Model
		pModel->render();
	}

	// Draws HUD buttons
	m_buttons.m_pCollectable->render(&m_imageType, m_windowSize);
	m_buttons.m_pAI->render(&m_imageType, m_windowSize);
	m_buttons.m_pTranslateMode->render(&m_imageType, m_windowSize);
	m_buttons.m_pRotateMode->render(&m_imageType, m_windowSize);
	m_buttons.m_pScaleMode->render(&m_imageType, m_windowSize);
	m_buttons.m_pSave->render(&m_imageType, m_windowSize);

	// Activates FreeType shader
	m_freeType.use();
	// Configures projection
	m_freeType.setUniform("projection", glm::ortho(0.0f, float(m_windowSize.x), 0.0f, float(m_windowSize.y)));

	// Defines position string
	std::string sPos; sPos += "Model Position: x("; sPos += std::to_string(m_selection.m_position.x); sPos += ") y("; sPos += std::to_string(m_selection.m_position.y); sPos += ") z("; sPos += std::to_string(m_selection.m_position.z); sPos += ")";
	// Outputs position string to HUD
	m_pHUD->RenderText(m_freeType.getHandle(), sPos, m_windowSize.x*0.006f, m_windowSize.y*0.964f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	// Defines rotation string
	std::string sRot; sRot += "Model Rotation: x("; sRot += std::to_string(m_selection.m_rotation.x); sRot += ") y("; sRot += std::to_string(m_selection.m_rotation.y); sRot += ") z("; sRot += std::to_string(m_selection.m_rotation.z); sRot += ")";
	// Outputs rotation string to HUD
	m_pHUD->RenderText(m_freeType.getHandle(), sRot, m_windowSize.x*0.006f, m_windowSize.y*0.934f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	// Defines scale string
	std::string sScale; sScale += "Model Scale: x("; sScale += std::to_string(m_selection.m_scale.x); sScale += ") y("; sScale += std::to_string(m_selection.m_scale.y); sScale += ") z("; sScale += std::to_string(m_selection.m_scale.z); sScale += ")";
	// Outputs scale string to HUD
	m_pHUD->RenderText(m_freeType.getHandle(), sScale, m_windowSize.x*0.006f, m_windowSize.y*0.904f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
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
		std::string sTData = std::to_string(m_selection.m_position.x); sTData += " "; sTData += std::to_string(m_selection.m_position.y); sTData += " "; sTData += std::to_string(m_selection.m_position.z);
		// Sets <Translation> value to the selection position
		pTrans->SetText(sTData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pTrans);

		// Defines new element for <Rotation>
		tinyxml2::XMLElement* pRot = document.NewElement("Rotation");
		// Defines string for Rotation data
		std::string sRData = std::to_string(m_selection.m_rotation.x); sRData += " "; sRData += std::to_string(m_selection.m_rotation.y); sRData += " "; sRData += std::to_string(m_selection.m_rotation.z);
		// Sets <Rotation> value to selection rotation
		pRot->SetText(sRData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pRot);

		// Defines new element for <Scale>
		tinyxml2::XMLElement* pScale = document.NewElement("Scale");
		// Defines string for Scale data
		std::string sSData = std::to_string(m_selection.m_scale.x); sSData += " "; sSData += std::to_string(m_selection.m_scale.y); sSData += " "; sSData += std::to_string(m_selection.m_scale.z);
		// Sets <Scale> value to selection scale
		pScale->SetText(sSData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pScale);

		// Defines new element for <Material>
		tinyxml2::XMLElement* pMaterial = document.NewElement("Material");
		// Sets <Material> value to selection material
		pMaterial->SetText(m_selection.m_uiMaterial);
		// Inserts element into <Object>
		pObject->InsertEndChild(pMaterial);

		// Defines new element for <Collectable>
		tinyxml2::XMLElement* pCollectable = document.NewElement("Collectable");
		// Sets <Collectable> value to selection collectable setting
		pCollectable->SetText(m_selection.m_bCollectable);
		// Inserts element into <Object>
		pObject->InsertEndChild(pCollectable);

		// Defines new element for <AI>
		tinyxml2::XMLElement* pCollectable = document.NewElement("AI");
		// Sets <AI> value to selection AI setting
		pCollectable->SetText(m_selection.m_bAI);
		// Inserts element into <Object>
		pObject->InsertEndChild(pCollectable);
	}

	// Saves the document
	document.SaveFile(m_sFilepath.c_str());

	std::cerr << "[EDITOR] File saved." << std::endl;
}