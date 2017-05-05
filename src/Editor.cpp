
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
	m_camera.setPosition(glm::vec3(0.0f, 15.0f, 40.0f));

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

// Void: Initialises the Editor Scene
void Editor::initScene(Freetype* pOverlay)
{
	// Get the Heads up display for the scene
	m_pHUD = pOverlay;

	// Enables OpenGL depth testing
	gl::Enable(gl::DEPTH_TEST);

	// Defines new TextBox for user input
	m_textBox = std::shared_ptr<TextBox>(new TextBox(glm::vec2(m_windowSize.x*0.2f, m_windowSize.y*0.5f)));

	// Defines button sizing
	glm::vec2 buttonSize_Small(200.0f, 50.0f);
	glm::vec2 buttonSize_Big(200.0f, 200.0f);
	float buttonPosX = m_windowSize.x*0.006f;

	// Defines HUD buttons
	// Model selection button
	m_buttons.m_pModel = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(buttonPosX + buttonSize_Big.x*0.5f, m_windowSize.y*0.606f + buttonSize_Big.y*0.5f),
			"assets/UI/Editor/Model.png",
			"assets/UI/Editor/ModelHover.png",
			glm::vec3(buttonSize_Big, 1.0f),
			pOverlay
		)
	);

	// Defines Collectable toggle button
	m_buttons.m_pCollectable = std::shared_ptr<ToggleButton>
	(
		new ToggleButton
		(
			std::shared_ptr<Button>
			(
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.466f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/CollectableTrue.png",
					"assets/UI/Editor/CollectableTrueHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			),
			std::shared_ptr<Button>
			(
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.466f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/CollectableFalse.png",
					"assets/UI/Editor/CollectableFalseHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			)
		)
	);

	// Defines AI toggle button
	m_buttons.m_pAI = std::shared_ptr<ToggleButton>
	(
		new ToggleButton
		(
			std::shared_ptr<Button>
			(
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.406f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/AITrue.png",
					"assets/UI/Editor/AITrueHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			),
			std::shared_ptr<Button>
			(	
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.406f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/AIFalse.png",
					"assets/UI/Editor/AIFalseHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			)
		)
	);

	// Defines TranslateMode toggle button
	m_buttons.m_pTranslateMode = std::shared_ptr<ToggleButton>
	(
		new ToggleButton
		(
			std::shared_ptr<Button>
			(
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.266f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/TranslateTrue.png",
					"assets/UI/Editor/TranslateTrueHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			),
			std::shared_ptr<Button>
			(
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.266f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/TranslateFalse.png",
					"assets/UI/Editor/TranslateFalseHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			)
		)
	);

	// Defines RotateMode toggle button
	m_buttons.m_pRotateMode = std::shared_ptr<ToggleButton>
	(
		new ToggleButton
		(
			std::shared_ptr<Button>
			(
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.206f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/RotateTrue.png",
					"assets/UI/Editor/RotateTrueHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			),
			std::shared_ptr<Button>
			(	
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.206f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/RotateFalse.png",
					"assets/UI/Editor/RotateFalseHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			)
		)
	);

	// Defines ScaleMode toggle button
	m_buttons.m_pScaleMode = std::shared_ptr<ToggleButton>
	(
		new ToggleButton
		(
			std::shared_ptr<Button>
			(
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.146f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/ScaleTrue.png",
					"assets/UI/Editor/ScaleTrueHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			),
			std::shared_ptr<Button>
			(
				new Button
				(
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.146f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/ScaleFalse.png",
					"assets/UI/Editor/ScaleFalseHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			)
		)
	);
	
	// Defines Save button
	m_buttons.m_pSave = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.006f + buttonSize_Small.y*0.5f), 
			"assets/UI/Editor/Save.png", 
			"assets/UI/Editor/SaveHover.png", 
			glm::vec3(buttonSize_Small, 1.0f), 
			pOverlay
		)
	);

	// Toggles TranslateMode button true to match selection.mode declaration
	m_buttons.m_pTranslateMode->setToggled(true);

	// Updates camera vision
	m_camera.updateView();

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

		if (kiKey != GLFW_KEY_BACKSPACE)
		{
			// Adds input to textbox
			m_textBox->addLetter(kiKey);
		}
		else
		{
			// Removes last letter on textbox
			m_textBox->removeLetter();
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
			// If Model button is clicked
			if (m_buttons.m_pModel->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				// Enter Model selection menu
			}

			// If Collectable button is clicked
			if (m_buttons.m_pCollectable->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				// Toggle Collectable
				m_selection.m_bCollectable = !m_selection.m_bCollectable;

				// Toggles button
				m_buttons.m_pCollectable->toggle();
			}
			// If AI button is clicked
			if (m_buttons.m_pAI->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				// Toggle AI
				m_selection.m_bAI = !m_selection.m_bAI;

				// Toggles button
				m_buttons.m_pAI->toggle();
			}

			// If TranslateMode button is clicked
			if (m_buttons.m_pTranslateMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				// Mode switched to Translate
				m_transformMode = TRANSLATE;

				// Toggles buttons appropriately
				m_buttons.m_pTranslateMode->setToggled(true);
				m_buttons.m_pRotateMode->setToggled(false);
				m_buttons.m_pScaleMode->setToggled(false);
			}
			// If RotateMode button is clicked
			if (m_buttons.m_pRotateMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				// Mode switched to Rotate
				m_transformMode = ROTATE;

				// Toggles buttons appropriately
				m_buttons.m_pTranslateMode->setToggled(false);
				m_buttons.m_pRotateMode->setToggled(true);
				m_buttons.m_pScaleMode->setToggled(false);
			}
			// If ScaleMode button is clicked
			if (m_buttons.m_pScaleMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				// Mode switched to Scale
				m_transformMode = SCALE;

				// Toggles buttons appropriately
				m_buttons.m_pTranslateMode->setToggled(false);
				m_buttons.m_pRotateMode->setToggled(false);
				m_buttons.m_pScaleMode->setToggled(true);
			}

			// If Save button is clicked
			if (m_buttons.m_pSave->mouseOver(m_mousePos, (float)m_windowSize.y))
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

	// Sets Model position to selection value
	m_selection.m_pModel->setPosition(m_selection.m_position);
	// Sets Model rotation to selection value
	m_selection.m_pModel->setRotation(m_selection.m_rotation);
	// Sets Model scale to selection value
	m_selection.m_pModel->setScale(m_selection.m_scale);
	// Sets Model material to selection value
	m_selection.m_pModel->setMaterial(m_selection.m_uiMaterial);

	// Sets last cursor position
	m_lastMousePos = m_mousePos;

	// Checks whether buttons are hovered
	m_buttons.m_pModel->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_buttons.m_pCollectable->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_buttons.m_pAI->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_buttons.m_pTranslateMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_buttons.m_pRotateMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_buttons.m_pScaleMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_buttons.m_pSave->mouseOver(m_mousePos, (float)m_windowSize.y);
}

// Void: Renders the Editor to display
void Editor::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Activates World shader
	m_spotlightShader.use();

	// Sets shader MVP
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
	//m_pSelectedModel->buffer();

	// Passes Model transformation data to shader
//	m_spotlightShader.setUniform("M", m_pSelectedModel->m_M);

	// Renders Model
	m_selection.m_pModel->render(&m_phongShader, glm::mat4(1.0f));

	// Updates Skybox position
	m_pSkybox->setPosition(m_camera.getPosition());
	// Renders Skybox
	m_pSkybox->render(&m_phongShader, glm::mat4(1.0f));

	// Render all Models in the Scene
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// Buffers the Model
		//pModel->buffer();
		
		// Passes Model transformation data to shader
		m_spotlightShader.setUniform("M", pModel->getM());
	
		// Renders Model
		pModel->render(&m_spotlightShader, pModel->getM());
	}

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
		pTexLoc->SetText(pModel->getTextureLocation().c_str());
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
		tinyxml2::XMLElement* pAI = document.NewElement("AI");
		// Sets <AI> value to selection AI setting
		pAI->SetText(m_selection.m_bAI);
		// Inserts element into <Object>
		pObject->InsertEndChild(pAI);
	}

	// Saves the document
	document.SaveFile(m_sFilepath.c_str());

	std::cerr << "[EDITOR] File saved." << std::endl;
}