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

	// Sets cursor style
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Enables OpenGL depth testing
	gl::Enable(gl::DEPTH_TEST);

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
			false,
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
			false,
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
			true,
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
			false,
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
			false,
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

	// Model Selection Menu Setup
	m_menu.m_pNameField = std::shared_ptr<TextBox>(new TextBox(glm::vec2(m_windowSize.x*0.3f, m_windowSize.y*0.65f)));
	m_menu.m_pObjField = std::shared_ptr<TextBox>(new TextBox(glm::vec2(m_windowSize.x*0.3f, m_windowSize.y*0.55f)));
	m_menu.m_pTexField = std::shared_ptr<TextBox>(new TextBox(glm::vec2(m_windowSize.x*0.3f, m_windowSize.y*0.45f)));
	m_menu.m_pActiveField = m_menu.m_pNameField;

	m_menu.m_uiBGIndex = (unsigned int)m_pHUD->m_imagePlane.size();
	m_pHUD->addImage("assets/UI/Editor/ModelMenuBG.png", vec3(m_windowSize.x*0.5f, m_windowSize.y*0.5f, 1.0f), 0, glm::vec3(m_windowSize.x*0.5, m_windowSize.y*0.5, 1.0f), false);
	
	m_menu.m_pLoad = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(m_windowSize.x*0.6f, m_windowSize.y*0.3f + buttonSize_Small.y*0.5f),
			"assets/UI/Editor/LoadModel.png",
			"assets/UI/Editor/LoadModelHover.png",
			glm::vec3(buttonSize_Small, 1.0f),
			pOverlay
		)
	);
	m_menu.m_pCancel = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(m_windowSize.x*0.4f, m_windowSize.y*0.3f + buttonSize_Small.y*0.5f),
			"assets/UI/Editor/Cancel.png",
			"assets/UI/Editor/CancelHover.png",
			glm::vec3(buttonSize_Small, 1.0f),
			pOverlay
		)
	);

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
		// If Esc key pressed
		if (kiKey == GLFW_KEY_ESCAPE) m_intention = TO_MENU; // Switch to Menu Scene

		if (m_bMenuOpen)
		{
			if (kiKey == GLFW_KEY_BACKSPACE) // Backspace
			{
				// Removes last letter in active textbox
				m_menu.m_pActiveField->removeLetter();
			}

			// TEMPORARY Field Selection
			if (kiKey == GLFW_KEY_1) m_menu.m_pActiveField = m_menu.m_pNameField;
			if (kiKey == GLFW_KEY_2) m_menu.m_pActiveField = m_menu.m_pObjField;
			if (kiKey == GLFW_KEY_3) m_menu.m_pActiveField = m_menu.m_pTexField;
		}
		else
		{
			// If Enter is pressed
			if (kiKey == GLFW_KEY_ENTER)
			{
				// Model is added to a vector of placed Models
				m_pModels.push_back(std::shared_ptr<Model>(new Model(*m_selection.m_pModel.get())));
			}

			// TEMPORARY UNDO -  If U key is pressed
			if (kiKey == GLFW_KEY_U)
			{
				// If vector is not empty pop the back Model off
				if (!m_pModels.empty()) m_pModels.pop_back();
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
}

// Void: Called on char input event
void Editor::input_char(const unsigned int kuiUnicode)
{
	// If Model selection menu is open
	if (m_bMenuOpen)
	{
		if 
		(
			(kuiUnicode >= 97 && kuiUnicode <= 122) || // a-z
			(kuiUnicode >= 65 && kuiUnicode <= 90) || // A-Z
			(kuiUnicode >= 48 && kuiUnicode <= 57) || // 0-9
			(kuiUnicode == 32) || // Spacebar
			(kuiUnicode == 33) || // !
			(kuiUnicode == 163) || // £
			(kuiUnicode == 35) || // #
			(kuiUnicode == 36) || // $
			(kuiUnicode == 37) || // %
			(kuiUnicode == 38) || // &
			(kuiUnicode == 39) || // '
			(kuiUnicode == 40) || // (
			(kuiUnicode == 41) || // )
			(kuiUnicode == 43) || // +
			(kuiUnicode == 44) || // ,
			(kuiUnicode == 45) || // -
			(kuiUnicode == 46) || // .
			(kuiUnicode == 59) || // ;
			(kuiUnicode == 61) || // =
			(kuiUnicode == 64) || // @
			(kuiUnicode == 91) || // [
			(kuiUnicode == 93) || // ]
			(kuiUnicode == 94) || // ^
			(kuiUnicode == 95) || // _
			(kuiUnicode == 96) || // `
			(kuiUnicode == 123) || // {
			(kuiUnicode == 125) || // }
			(kuiUnicode == 126) // ~
		)
		{
			// Adds input to active textbox
			m_menu.m_pActiveField->addLetter(kuiUnicode);
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
			// If Model selection menu is open
			if (m_bMenuOpen)
			{
				// If menu load button is clicked
				if (m_menu.m_pLoad->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// If obj file doesn't exist
					if (!fileExists("assets/models/" + m_menu.m_pObjField->getStr() + ".obj"))
					{
						std::cerr << "[EDITOR] ERROR - OBJ FILE DOESN'T EXIST: " << ("assets/models/" + m_menu.m_pObjField->getStr() + ".obj") << std::endl;
					}
					// Else - obj exists
					else
					{
						// If texture file doesn't exist
						if (!fileExists("assets/textures/" + m_menu.m_pTexField->getStr() + ".bmp"))
						{
							std::cerr << "[EDITOR] ERROR - BMP FILE DOESN'T EXIST: " << ("assets/textures/" + m_menu.m_pTexField->getStr() + ".bmp") << std::endl;
						}
						// Else - texture exists
						else
						{
							// Creates Model
							std::shared_ptr<Model> pModel = std::shared_ptr<Model>(new Model());
							pModel->setName(m_menu.m_pNameField->getStr());
							pModel->setFileLocation("assets/models/" + m_menu.m_pObjField->getStr() + ".obj");
							pModel->setTextureLocation("assets/textures/" + m_menu.m_pTexField->getStr() + ".bmp");
							pModel->setMaterial(1);

							// Loads Model so it's ready for drawing
							pModel->loadModel();
							// Initialises Model
							pModel->initModel();

							// Sets Model in hand to this new Model
							m_selection.m_pModel = pModel;

							// Closes menu
							m_bMenuOpen = false;
							m_menu.reset();
						}
					}
				}
				// If menu cancel button is clicked
				if (m_menu.m_pCancel->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// Closes menu
					m_bMenuOpen = false;
					m_menu.reset();
				}
			}
			else
			{
				// If Model button is clicked
				if (m_buttons.m_pModel->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// Enter Model selection menu
					m_bMenuOpen = true;
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
}

// Void: Called on mouseScroll input event
void Editor::input_scroll(const double kdDelta)
{
	// If Model selection menu is not open
	if (!m_bMenuOpen)
	{
		// Move in the Z axis 
		m_camera.move(glm::vec3(0.0f, 0.0f, -CAMERA_ZOOM*kdDelta));
	}
}

// Void: Updates the Editor with elapsed time
void Editor::update(const float kfTimeElapsed)
{
	/////////////////// USER DISPLAY PROCESSING ///////////////////
	// Calculates the mouse movement
	sf::Vector2f delta(m_mousePos - m_lastMousePos);

	// If Model selection menu is not open
	if (!m_bMenuOpen)
	{
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

	// If Model selection menu is open
	if (m_bMenuOpen)
	{
		// Checks whether buttons are hovered
		m_menu.m_pLoad->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_menu.m_pCancel->mouseOver(m_mousePos, (float)m_windowSize.y);
	}
	else
	{
		// Checks whether HUD buttons are hovered
		m_buttons.m_pModel->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pCollectable->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pAI->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pTranslateMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pRotateMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pScaleMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pSave->mouseOver(m_mousePos, (float)m_windowSize.y);
	}
}

// Void: Renders the Editor to display
void Editor::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Activates World shader
	m_phongShader.use();

	// Passes texture map to shader
	m_phongShader.setUniform("TextureMap", 0);
	// Sets shader view and projection
	m_phongShader.setUniform("V", m_camera.getView());
	m_phongShader.setUniform("P", m_camera.getProjection());
	// Configures lighting
	setLightParams(&m_phongShader);

	// Renders Model
	m_selection.m_pModel->render(&m_phongShader, glm::mat4(1.0f));

	// Render all Models in the Scene
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// Renders Model
		pModel->render(&m_phongShader, glm::mat4(1.0f));
	}

	// Activates texture shader
	m_textureShader.use();

	// Passes texture map to shader
	m_textureShader.setUniform("TextureMap", 0);
	// Sets shader view and projection
	m_textureShader.setUniform("V", m_camera.getView());
	m_textureShader.setUniform("P", m_camera.getProjection());

	// Updates Skybox position
	m_pSkybox->setPosition(m_camera.getPosition());
	// Renders Skybox
	m_pSkybox->render(&m_textureShader, glm::mat4(1.0f));

	// Draws HUD buttons
	m_buttons.m_pModel->render(&m_imageType, m_windowSize);
	m_buttons.m_pCollectable->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pAI->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pTranslateMode->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pRotateMode->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pScaleMode->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pSave->render(&m_imageType, m_windowSize);
	
	// If Model selection menu is open
	if (m_bMenuOpen)
	{
		// Draws menu buttons
		m_menu.m_pLoad->render(&m_imageType, m_windowSize);
		m_menu.m_pCancel->render(&m_imageType, m_windowSize);

		// Renders text boxes
		// Name field
		std::shared_ptr<TextBox> pNameField = std::shared_ptr<TextBox>(new TextBox(*m_menu.m_pNameField.get()));
		std::string sNameStr; sNameStr += "Model Name: "; sNameStr += m_menu.m_pNameField->getStr();
		pNameField->setStr(sNameStr);
		pNameField->render(&m_freeType, m_pHUD, glm::vec2(m_windowSize.x, m_windowSize.y));
		// Obj field
		std::shared_ptr<TextBox> pObjField = std::shared_ptr<TextBox>(new TextBox(*m_menu.m_pObjField.get()));
		std::string sObjStr; sObjStr += "Obj File: assets/models/"; sObjStr += m_menu.m_pObjField->getStr(); sObjStr += ".obj";
		pObjField->setStr(sObjStr);
		pObjField->render(&m_freeType, m_pHUD, glm::vec2(m_windowSize.x, m_windowSize.y));
		// Tex field
		std::shared_ptr<TextBox> pTexField = std::shared_ptr<TextBox>(new TextBox(*m_menu.m_pTexField.get()));
		std::string sTexStr; sTexStr += "Texture File: assets/textures/"; sTexStr += m_menu.m_pTexField->getStr(); sTexStr += ".bmp";
		pTexField->setStr(sTexStr);
		pTexField->render(&m_freeType, m_pHUD, glm::vec2(m_windowSize.x, m_windowSize.y));

		// Activates ImageType shader
		//m_imageType.use();
		//// Draws background
		//m_imageType.setUniform("M", m_pHUD->m_imagePlane.at(m_menu.m_uiBGIndex).getM());
		//m_imageType.setUniform("P", glm::ortho(0.0f, (float)m_windowSize.x, 0.f, (float)m_windowSize.y));
		//m_pHUD->renderImage(&m_imageType, m_menu.m_uiBGIndex);
	}
  
	// Activates FreeType shader
	m_freeType.use();
	// Configures projection
	m_freeType.setUniform("projection", glm::ortho(0.0f, float(m_windowSize.x), 0.0f, float(m_windowSize.y)));

	// Defines position string
	std::string sPos; sPos += "Model Position: x("; sPos += std::to_string(m_selection.m_position.x); sPos += ") y("; sPos += std::to_string(m_selection.m_position.y); sPos += ") z("; sPos += std::to_string(m_selection.m_position.z); sPos += ")";
	// Outputs position string to HUD
	m_pHUD->renderText(m_freeType.getHandle(), sPos, m_windowSize.x*0.006f, m_windowSize.y*0.964f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	// Defines rotation string
	std::string sRot; sRot += "Model Rotation: x("; sRot += std::to_string(m_selection.m_rotation.x); sRot += ") y("; sRot += std::to_string(m_selection.m_rotation.y); sRot += ") z("; sRot += std::to_string(m_selection.m_rotation.z); sRot += ")";
	// Outputs rotation string to HUD
	m_pHUD->renderText(m_freeType.getHandle(), sRot, m_windowSize.x*0.006f, m_windowSize.y*0.934f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	// Defines scale string
	std::string sScale; sScale += "Model Scale: x("; sScale += std::to_string(m_selection.m_scale.x); sScale += ") y("; sScale += std::to_string(m_selection.m_scale.y); sScale += ") z("; sScale += std::to_string(m_selection.m_scale.z); sScale += ")";
	// Outputs scale string to HUD
	m_pHUD->renderText(m_freeType.getHandle(), sScale, m_windowSize.x*0.006f, m_windowSize.y*0.904f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
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