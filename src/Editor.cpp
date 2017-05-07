#include "Editor.h"

#define POSITION_SPEED 12.5f
#define ROTATION_SPEED 90.0f
#define SCALE_SPEED 2.5f

#define CAMERA_ROTATION 0.0015f
#define CAMERA_SPEED 0.01f
#define CAMERA_ZOOM 3.5f

#define FLASH_SPEED 0.5f

// Constructor
Editor::Editor(GLFWwindow *pWindow, const sf::Vector2i kWindowSize)
{
	// Sets members with input
	m_pWindow = pWindow;
	m_windowSize = kWindowSize;

	// Updates Camera aspect ratio
	m_camera.setAspectRatio((float)kWindowSize.x / kWindowSize.y);

	// Sets Camera Near/Far culling
	m_camera.setNearCull(1.0f);
	m_camera.setFarCull(500.0f);

	// Sets Camera initital position 
	m_camera.setPosition(glm::vec3(0.0f, 15.0f, 40.0f));

	// Creates a default selected Model
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

	// Creates a spawn indicator
	m_pSpawn = std::shared_ptr<Model>(new Model());
	m_pSpawn->setName("Spawn");
	m_pSpawn->setFileLocation("assets/models/pole.obj");
	m_pSpawn->setTextureLocation("assets/textures/pole.bmp");
	m_pSpawn->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	m_pSpawn->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	m_pSpawn->setScale(glm::vec3(3.0f, 3.0f, 3.0f));
	pModel->setMaterial(1);

	// Loads Model so it's ready for drawing
	m_pSpawn->loadModel();
	// Initialises Model
	m_pSpawn->initModel();

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
	glm::vec2 buttonSize_Tiny(100.0f, 50.0f);
	glm::vec2 buttonSize_Small(200.0f, 50.0f);
	glm::vec2 buttonSize_Big(200.0f, 200.0f);
	float buttonPosX = m_windowSize.x*0.006f;

	// Defines HUD buttons
	// Model selection button
	m_buttons.m_pModel = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(buttonPosX + buttonSize_Big.x*0.5f, m_windowSize.y*0.666f + buttonSize_Big.y*0.5f),
			"assets/UI/Editor/Model.png",
			"assets/UI/Editor/ModelHover.png",
			glm::vec3(buttonSize_Big, 1.0f),
			pOverlay
		)
	);

	// Undo button
	m_buttons.m_pUndo = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(buttonPosX + buttonSize_Tiny.x*0.5f, m_windowSize.y*0.606f + buttonSize_Tiny.y*0.5f),
			"assets/UI/Editor/Undo.png",
			"assets/UI/Editor/UndoHover.png",
			glm::vec3(buttonSize_Small.x*0.5f, buttonSize_Small.y, 1.0f),
			pOverlay
		)
	);
	// Redo button
	m_buttons.m_pRedo = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(buttonPosX + buttonSize_Tiny.x + buttonSize_Tiny.x*0.5f, m_windowSize.y*0.606f + buttonSize_Tiny.y*0.5f),
			"assets/UI/Editor/Redo.png",
			"assets/UI/Editor/RedoHover.png",
			glm::vec3(buttonSize_Small.x*0.5f, buttonSize_Small.y, 1.0f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.486f + buttonSize_Small.y*0.5f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.486f + buttonSize_Small.y*0.5f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.426f + buttonSize_Small.y*0.5f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.426f + buttonSize_Small.y*0.5f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.306f + buttonSize_Small.y*0.5f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.306f + buttonSize_Small.y*0.5f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.246f + buttonSize_Small.y*0.5f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.246f + buttonSize_Small.y*0.5f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.186f + buttonSize_Small.y*0.5f),
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
					glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.186f + buttonSize_Small.y*0.5f),
					"assets/UI/Editor/ScaleFalse.png",
					"assets/UI/Editor/ScaleFalseHover.png",
					glm::vec3(buttonSize_Small, 1.0f),
					pOverlay
				)
			)
		)
	);
	
	// Defines Load button
	m_buttons.m_pLoad = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.066f + buttonSize_Small.y*0.5f),
			"assets/UI/Editor/LoadScene.png",
			"assets/UI/Editor/LoadSceneHover.png",
			glm::vec3(buttonSize_Small, 1.0f),
			pOverlay
		)
	);

	// Defines Save button
	m_buttons.m_pSave = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(buttonPosX + buttonSize_Small.x*0.5f, m_windowSize.y*0.006f + buttonSize_Small.y*0.5f), 
			"assets/UI/Editor/SaveScene.png", 
			"assets/UI/Editor/SaveSceneHover.png", 
			glm::vec3(buttonSize_Small, 1.0f), 
			pOverlay
		)
	);

	// Model Selection Menu Setup
	m_select.m_pNameField = std::shared_ptr<TextBox>(new TextBox(glm::vec2(m_windowSize.x*0.3f, m_windowSize.y*0.65f)));
	m_select.m_pObjField = std::shared_ptr<TextBox>(new TextBox(glm::vec2(m_windowSize.x*0.3f, m_windowSize.y*0.55f)));
	m_select.m_pTexField = std::shared_ptr<TextBox>(new TextBox(glm::vec2(m_windowSize.x*0.3f, m_windowSize.y*0.45f)));
	m_select.m_pActiveField = m_select.m_pNameField;

	m_select.m_uiBGIndex = (unsigned int)m_pHUD->m_imagePlane.size();
	m_pHUD->addImage("assets/UI/Editor/ModelMenuBG.png", vec3(m_windowSize.x*0.5f, m_windowSize.y*0.5f, 1.0f), 0, glm::vec3(m_windowSize.x*0.5, m_windowSize.y*0.5, 1.0f), false);
	
	m_select.m_pLoad = std::shared_ptr<Button>
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
	m_select.m_pCancel = std::shared_ptr<Button>
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

	// Editor Save Menu Setup
	m_save.m_pFileField = std::shared_ptr<TextBox>(new TextBox(glm::vec2(m_windowSize.x*0.3f, m_windowSize.y*0.55f)));

	m_save.m_uiBGIndex = (unsigned int)m_pHUD->m_imagePlane.size();
	m_pHUD->addImage("assets/UI/Editor/SaveMenuBG.png", vec3(m_windowSize.x*0.5f, m_windowSize.y*0.5f, 1.0f), 0, glm::vec3(m_windowSize.x*0.5, m_windowSize.y*0.5, 1.0f), false);

	m_save.m_pEnter = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(m_windowSize.x*0.6f, m_windowSize.y*0.3f + buttonSize_Small.y*0.5f),
			"assets/UI/Editor/SaveXML.png",
			"assets/UI/Editor/SaveXMLHover.png",
			glm::vec3(buttonSize_Small, 1.0f),
			pOverlay
		)
	);
	m_save.m_pCancel = std::shared_ptr<Button>
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

	// Editor Load Menu Setup
	m_load.m_pFileField = std::shared_ptr<TextBox>(new TextBox(glm::vec2(m_windowSize.x*0.3f, m_windowSize.y*0.55f)));

	m_load.m_uiBGIndex = (unsigned int)m_pHUD->m_imagePlane.size();
	m_pHUD->addImage("assets/UI/Editor/LoadMenuBG.png", vec3(m_windowSize.x*0.5f, m_windowSize.y*0.5f, 1.0f), 0, glm::vec3(m_windowSize.x*0.5, m_windowSize.y*0.5, 1.0f), false);

	m_load.m_pEnter = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2(m_windowSize.x*0.6f, m_windowSize.y*0.3f + buttonSize_Small.y*0.5f),
			"assets/UI/Editor/LoadXML.png",
			"assets/UI/Editor/LoadXMLHover.png",
			glm::vec3(buttonSize_Small, 1.0f),
			pOverlay
		)
	);
	m_load.m_pCancel = std::shared_ptr<Button>
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
		if (kiKey == GLFW_KEY_ESCAPE)
		{
			// If in Editor state
			if (m_state == EDITOR) m_intention = TO_MENU; // Switch to Menu Scene

			// If in Model selection menu
			else if (m_state == MENU_SELECT)
			{
				// Closes menu
				m_state = EDITOR;
				m_select.reset();
			}
			// If in save menu
			else if (m_state == MENU_SAVE)
			{
				// Closes menu
				m_state = EDITOR;
				m_save.reset();
			}
			// If in load menu
			else if (m_state == MENU_LOAD)
			{
				// Closes menu
				m_state = EDITOR;
				m_load.reset();
			}
		}

		if (kiKey == GLFW_KEY_ENTER)
		{
			// If in Editor state
			if (m_state == EDITOR)
			{
				// Model is added to a vector of placed Models
				m_pModels.push_back(std::shared_ptr<Model>(new Model(*m_selection.m_pModel.get())));
				// Resets redo list
				m_selection.m_pRedoList.clear();
			}

			// If in Model selection menu
			else if (m_state == MENU_SELECT)
			{
				button_select();
			}
			// If in save menu
			else if (m_state == MENU_SAVE)
			{
				button_save();
			}
			// If in load menu
			else if (m_state == MENU_LOAD)
			{
				button_load();
			}
		}

		// If in Editor state
		if (m_state == EDITOR)
		{
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

		// If in selection menu state
		else if (m_state == MENU_SELECT)
		{
			if (kiKey == GLFW_KEY_BACKSPACE) // Backspace
			{
				// Removes last letter in active textbox
				m_select.m_pActiveField->removeLetter();
			}

			// TEMPORARY Field Selection
			if (kiKey == GLFW_KEY_TAB)
			{
				// If name field active - Wwitch to obj field
				if (m_select.m_pActiveField == m_select.m_pNameField) m_select.m_pActiveField = m_select.m_pObjField;

				// If obj field active - Wwitch to tex field
				else if (m_select.m_pActiveField == m_select.m_pObjField) m_select.m_pActiveField = m_select.m_pTexField;

				// If tex field active - Wwitch to name field
				else if (m_select.m_pActiveField == m_select.m_pTexField) m_select.m_pActiveField = m_select.m_pNameField;
			}
		}

		// If in save menu state
		else if (m_state == MENU_SAVE)
		{
			if (kiKey == GLFW_KEY_BACKSPACE) // Backspace
			{
				// Removes last letter in file textbox
				m_save.m_pFileField->removeLetter();
			}
		}
		// If in load menu state
		else if (m_state == MENU_LOAD)
		{
			if (kiKey == GLFW_KEY_BACKSPACE) // Backspace
			{
				// Removes last letter in file textbox
				m_load.m_pFileField->removeLetter();
			}
		}
	}
}

// Void: Called on char input event
void Editor::input_char(const unsigned int kuiUnicode)
{
	// If a menu is open
	if ((m_state == MENU_SELECT) || (m_state == MENU_SAVE) || (m_state == MENU_LOAD))
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
			if (m_state == MENU_SELECT)
			{
				// Adds input to active textbox
				m_select.m_pActiveField->addLetter(kuiUnicode);
			}
			else if (m_state == MENU_SAVE)
			{
				// Adds input to active textbox
				m_save.m_pFileField->addLetter(kuiUnicode);
			}
			else if (m_state == MENU_LOAD)
			{
				// Adds input to active textbox
				m_load.m_pFileField->addLetter(kuiUnicode);
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
			// If Editor in editing state
			if (m_state == EDITOR)
			{
				// If Model button is clicked
				if (m_buttons.m_pModel->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// Enter Model selection menu
					m_state = MENU_SELECT;
				}
				// If save button is clicked
				if (m_buttons.m_pSave->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// Enter Model selection menu
					m_state = MENU_SAVE;
				}
				// If load button is clicked
				if (m_buttons.m_pLoad->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// Enter Model selection menu
					m_state = MENU_LOAD;
				}

				// If undo button is clicked
				if (m_buttons.m_pUndo->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// If Model vector is not empty
					if (!m_pModels.empty())
					{
						// Stores Model in redo vector
						m_selection.m_pRedoList.push_back(m_pModels.back());
						// Removes Model from Model vector
						m_pModels.pop_back();
					}
				}
				// If redo button is clicked
				if (m_buttons.m_pRedo->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// If redo list is not empty
					if (!m_selection.m_pRedoList.empty())
					{
						// Stores redone Model in Model vector
						m_pModels.push_back(m_selection.m_pRedoList.back());
						// Removes Model from redo vector
						m_selection.m_pRedoList.pop_back();
					}
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
			}

			// If Model selection menu is open
			else if (m_state == MENU_SELECT)
			{
				// If menu load button is clicked
				if (m_select.m_pLoad->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					button_select();
				}
				// If menu cancel button is clicked
				if (m_select.m_pCancel->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// Closes menu
					m_state = EDITOR;
					m_select.reset();
				}
			}
			
			// If Model save menu is open
			else if (m_state == MENU_SAVE)
			{
				// If save button is clicked
				if (m_save.m_pEnter->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					button_save();
				}

				// If menu cancel button is clicked
				if (m_save.m_pCancel->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// Closes menu
					m_state = EDITOR;
					m_save.reset();
				}
			}
			
			// If Model load menu is open
			else if (m_state == MENU_LOAD)
			{
				// If load button is clicked
				if (m_load.m_pEnter->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					button_load();
				}

				// If menu cancel button is clicked
				if (m_load.m_pCancel->mouseOver(m_mousePos, (float)m_windowSize.y))
				{
					// Closes menu
					m_state = EDITOR;
					m_load.reset();
				}
			}
		}
	}
}

// Void: Called on mouseScroll input event
void Editor::input_scroll(const double kdDelta)
{
	// If in Editor state
	if (m_state == EDITOR)
	{
		// Move in the Z axis 
		m_camera.move(glm::vec3(0.0f, 0.0f, -CAMERA_ZOOM*kdDelta));
	}
}

// Void: Updates the Editor with elapsed time
void Editor::update(const float kfTimeElapsed)
{
	/////////////////// AUTOSAVING ///////////////////
	// Decrements autosave timeout by time
	m_fAutosaveTimer += kfTimeElapsed;
	if (m_fAutosaveTimer >= 180.0f)
	{
		// Resets autosave timer
		m_fAutosaveTimer = 0.0f;

		// Saves Scene to XML
		save("assets/scenes/autosave.xml");
	}
	
	/////////////////// USER DISPLAY PROCESSING ///////////////////
	// Calculates the mouse movement
	sf::Vector2f delta(m_mousePos - m_lastMousePos);

	/////////////////// SELECTION FLASHING ///////////////////
	m_selection.m_lightIntensity.update(FLASH_SPEED*kfTimeElapsed);
	m_collFlash.update(FLASH_SPEED*kfTimeElapsed);
	m_aiFlash.update(FLASH_SPEED*kfTimeElapsed);

	/////////////////// USER CONTROLS ///////////////////
	// If editing state
	if (m_state == EDITOR)
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
				m_selection.m_rotation += glm::vec3(0.0f, 0.0f, ROTATION_SPEED*kfTimeElapsed);
			// If D key is pressed
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				m_selection.m_rotation += glm::vec3(0.0f, 0.0f, -ROTATION_SPEED*kfTimeElapsed);
		}

		/////////////////// SCALE ///////////////////
		if (m_transformMode == SCALE)
		{
			/////////////////// X ///////////////////
			// If W key is pressed
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				m_selection.m_scale += glm::vec3(SCALE_SPEED*kfTimeElapsed, 0.0f, 0.0f);
			// If S key is pressed
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				m_selection.m_scale += glm::vec3(-SCALE_SPEED*kfTimeElapsed, 0.0f, 0.0f);

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
	// Sets Model collectable to selection value
	m_selection.m_pModel->setCollectable(m_selection.m_bCollectable);
	// Sets Model ai to selection value
	m_selection.m_pModel->setAI(m_selection.m_bAI);
	// Sets Model material to selection value
	m_selection.m_pModel->setMaterial(m_selection.m_uiMaterial);

	// Sets last cursor position
	m_lastMousePos = m_mousePos;

	/////////////////// HUD PROCESSING ///////////////////
	m_popUp.countdown(kfTimeElapsed);

	// If in Editor state
	if (m_state == EDITOR)
	{
		// Checks whether HUD buttons are hovered
		m_buttons.m_pModel->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pUndo->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pRedo->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pCollectable->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pAI->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pTranslateMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pRotateMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pScaleMode->getButton()->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pLoad->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_buttons.m_pSave->mouseOver(m_mousePos, (float)m_windowSize.y);
	}
	// If Model selection menu is open
	else if (m_state == MENU_SELECT)
	{
		// Checks whether buttons are hovered
		m_select.m_pLoad->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_select.m_pCancel->mouseOver(m_mousePos, (float)m_windowSize.y);
	}
	// If save menu is open
	else if (m_state == MENU_SAVE)
	{
		// Checks whether buttons are hovered
		m_save.m_pEnter->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_save.m_pCancel->mouseOver(m_mousePos, (float)m_windowSize.y);
	}
	// If load menu is open
	else if (m_state == MENU_LOAD)
	{
		// Checks whether buttons are hovered
		m_load.m_pEnter->mouseOver(m_mousePos, (float)m_windowSize.y);
		m_load.m_pCancel->mouseOver(m_mousePos, (float)m_windowSize.y);
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
	m_phongShader.setUniform("Material.Ka", glm::vec3(0.2f, 0.2f, 0.2f));
	m_phongShader.setUniform("Material.Kd", glm::vec3(0.5f, 0.5f, 0.5f));
	m_phongShader.setUniform("Material.Ks", glm::vec3(1.0f, 1.0f, 1.0f));
	m_phongShader.setUniform("Material.Shininess", 20.0f);
	m_phongShader.setUniform("Light.Position", m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);

	// Render all Models in the Scene
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// If collectable
		if (pModel->isCollectable())
		{
			// Flashes blue
			m_phongShader.setUniform("Light.Intensity", glm::vec3(0.6f, 0.6f, 0.6f + m_collFlash.value()));
		}
		// If AI
		else if (pModel->isAI())
		{
			// Flashes blue
			m_phongShader.setUniform("Light.Intensity", glm::vec3(0.6f, 0.6f + m_collFlash.value(), 0.6f));
		}
		// If neither Coll or AI
		else
		{
			m_phongShader.setUniform("Light.Intensity", glm::vec3(0.6f, 0.6f, 0.6f));
		}

		// Renders Model
		pModel->render(&m_phongShader, glm::mat4(1.0f));
	}

	// Applies flashing effect to selection
	m_phongShader.setUniform("Light.Intensity", glm::vec3(0.6f + m_selection.m_lightIntensity.value(), 0.6f + m_selection.m_lightIntensity.value(), 0.6f + m_selection.m_lightIntensity.value()));
	// Renders Model
	m_selection.m_pModel->render(&m_phongShader, glm::mat4(1.0f));

	// Activates texture shader
	m_textureShader.use();

	// Passes texture map to shader
	m_textureShader.setUniform("TextureMap", 0);
	// Sets shader view and projection
	m_textureShader.setUniform("V", m_camera.getView());
	m_textureShader.setUniform("P", m_camera.getProjection());
	m_textureShader.setUniform("Light.Intensity", glm::vec3(1.0f, 1.0f, 1.0f));

	// Updates Skybox position
	m_pSkybox->setPosition(m_camera.getPosition());
	// Renders Skybox
	m_pSkybox->render(&m_textureShader, glm::mat4(1.0f));

	// Renders Spawn indicator
	m_pSpawn->render(&m_textureShader, glm::mat4(1.0f));

	// Draws HUD buttons
	m_buttons.m_pModel->render(&m_imageType, m_windowSize);
	m_buttons.m_pUndo->render(&m_imageType, m_windowSize);
	m_buttons.m_pRedo->render(&m_imageType, m_windowSize);
	m_buttons.m_pCollectable->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pAI->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pTranslateMode->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pRotateMode->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pScaleMode->getButton()->render(&m_imageType, m_windowSize);
	m_buttons.m_pLoad->render(&m_imageType, m_windowSize);
	m_buttons.m_pSave->render(&m_imageType, m_windowSize);
	
	// If Model selection menu is open
	if (m_state == MENU_SELECT)
	{
		// Draws menu buttons
		m_select.m_pLoad->render(&m_imageType, m_windowSize);
		m_select.m_pCancel->render(&m_imageType, m_windowSize);

		// Defines colours for field text
		glm::vec3 activeColour = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 inactiveColour = glm::vec3(1.0f, 5.0f, 5.0f);

		// Sets all field colours to inactive colour
		glm::vec3 nameFieldColour = inactiveColour; glm::vec3 objFieldColour = inactiveColour; glm::vec3 texFieldColour = inactiveColour;

		// If field is active sets colour appropriately
		if (m_select.m_pActiveField == m_select.m_pNameField) { nameFieldColour = glm::vec3(1.0f, 0.0f, 0.0f); }
		if (m_select.m_pActiveField == m_select.m_pObjField) { objFieldColour = glm::vec3(1.0f, 0.0f, 0.0f); }
		if (m_select.m_pActiveField == m_select.m_pTexField) { texFieldColour = glm::vec3(1.0f, 0.0f, 0.0f); }

		// Renders text boxes
		// Name field
		std::shared_ptr<TextBox> pNameField = std::shared_ptr<TextBox>(new TextBox(*m_select.m_pNameField.get()));
		std::string sNameStr; sNameStr += "Model Name: "; sNameStr += m_select.m_pNameField->getStr();
		pNameField->setStr(sNameStr);
		pNameField->render(&m_freeType, m_pHUD, glm::vec2(m_windowSize.x, m_windowSize.y), nameFieldColour);
		// Obj field
		std::shared_ptr<TextBox> pObjField = std::shared_ptr<TextBox>(new TextBox(*m_select.m_pObjField.get()));
		std::string sObjStr; sObjStr += "Obj File: assets/models/"; sObjStr += m_select.m_pObjField->getStr(); sObjStr += ".obj";
		pObjField->setStr(sObjStr);
		pObjField->render(&m_freeType, m_pHUD, glm::vec2(m_windowSize.x, m_windowSize.y), objFieldColour);
		// Tex field
		std::shared_ptr<TextBox> pTexField = std::shared_ptr<TextBox>(new TextBox(*m_select.m_pTexField.get()));
		std::string sTexStr; sTexStr += "Texture File: assets/textures/"; sTexStr += m_select.m_pTexField->getStr(); sTexStr += ".bmp";
		pTexField->setStr(sTexStr);
		pTexField->render(&m_freeType, m_pHUD, glm::vec2(m_windowSize.x, m_windowSize.y), texFieldColour);

		// Activates ImageType shader
		//m_imageType.use();
		//// Draws background
		//m_imageType.setUniform("M", m_pHUD->m_imagePlane.at(m_select.m_uiBGIndex).getM());
		//m_imageType.setUniform("P", glm::ortho(0.0f, (float)m_windowSize.x, 0.f, (float)m_windowSize.y));
		//m_pHUD->renderImage(&m_imageType, m_select.m_uiBGIndex);
	}

	// If save menu is open
	else if (m_state == MENU_SAVE)
	{
		// Draws menu buttons
		m_save.m_pEnter->render(&m_imageType, m_windowSize);
		m_save.m_pCancel->render(&m_imageType, m_windowSize);

		// Renders text box
		// Obj field
		std::shared_ptr<TextBox> pFileField = std::shared_ptr<TextBox>(new TextBox(*m_save.m_pFileField.get()));
		std::string sFileStr; sFileStr += "XML File: assets/scenes/"; sFileStr += m_save.m_pFileField->getStr(); sFileStr += ".xml";
		pFileField->setStr(sFileStr);
		pFileField->render(&m_freeType, m_pHUD, glm::vec2(m_windowSize.x, m_windowSize.y), glm::vec3(1.0f, 0.0f, 0.0f));

		// Activates ImageType shader
		//m_imageType.use();
		//// Draws background
		//m_imageType.setUniform("M", m_pHUD->m_imagePlane.at(m_save.m_uiBGIndex).getM());
		//m_imageType.setUniform("P", glm::ortho(0.0f, (float)m_windowSize.x, 0.f, (float)m_windowSize.y));
		//m_pHUD->renderImage(&m_imageType, m_save.m_uiBGIndex);
	}

	// If save menu is open
	else if (m_state == MENU_LOAD)
	{
		// Draws menu buttons
		m_load.m_pEnter->render(&m_imageType, m_windowSize);
		m_load.m_pCancel->render(&m_imageType, m_windowSize);

		// Renders text box
		// Obj field
		std::shared_ptr<TextBox> pFileField = std::shared_ptr<TextBox>(new TextBox(*m_load.m_pFileField.get()));
		std::string sFileStr; sFileStr += "XML File: assets/scenes/"; sFileStr += m_load.m_pFileField->getStr(); sFileStr += ".xml";
		pFileField->setStr(sFileStr);
		pFileField->render(&m_freeType, m_pHUD, glm::vec2(m_windowSize.x, m_windowSize.y), glm::vec3(1.0f, 0.0f, 0.0f));

		// Activates ImageType shader
		//m_imageType.use();
		//// Draws background
		//m_imageType.setUniform("M", m_pHUD->m_imagePlane.at(m_load.m_uiBGIndex).getM());
		//m_imageType.setUniform("P", glm::ortho(0.0f, (float)m_windowSize.x, 0.f, (float)m_windowSize.y));
		//m_pHUD->renderImage(&m_imageType, m_load.m_uiBGIndex);
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

	// If there's a pop up
	if (m_popUp.getCountdown() > 0.0f)
	{
		// Outputs scale string to HUD
		m_pHUD->renderText(m_freeType.getHandle(), m_popUp.getText(), m_windowSize.x*0.15f, m_windowSize.y*0.775f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
}

// Void: Saves the Scene to XML file
void Editor::save(const std::string ksFilePath)
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
		std::string sTData = std::to_string(pModel->getPosition().x); sTData += " "; sTData += std::to_string(pModel->getPosition().y); sTData += " "; sTData += std::to_string(pModel->getPosition().z);
		// Sets <Translation> value to the selection position
		pTrans->SetText(sTData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pTrans);

		// Defines new element for <Rotation>
		tinyxml2::XMLElement* pRot = document.NewElement("Rotation");
		// Defines string for Rotation data
		std::string sRData = std::to_string(pModel->getRotation().x); sRData += " "; sRData += std::to_string(pModel->getRotation().y); sRData += " "; sRData += std::to_string(pModel->getRotation().z);
		// Sets <Rotation> value to selection rotation
		pRot->SetText(sRData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pRot);

		// Defines new element for <Scale>
		tinyxml2::XMLElement* pScale = document.NewElement("Scale");
		// Defines string for Scale data
		std::string sSData = std::to_string(pModel->getScale().x); sSData += " "; sSData += std::to_string(pModel->getScale().y); sSData += " "; sSData += std::to_string(pModel->getScale().z);
		// Sets <Scale> value to selection scale
		pScale->SetText(sSData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pScale);

		// Defines new element for <Material>
		tinyxml2::XMLElement* pMaterial = document.NewElement("Material");
		// Sets <Material> value to selection material
		pMaterial->SetText(pModel->getMaterial());
		// Inserts element into <Object>
		pObject->InsertEndChild(pMaterial);

		// Defines new element for <Collectable>
		tinyxml2::XMLElement* pCollectable = document.NewElement("Collectable");
		// Sets <Collectable> value to selection collectable setting
		pCollectable->SetText(pModel->isCollectable());
		// Inserts element into <Object>
		pObject->InsertEndChild(pCollectable);

		// Defines new element for <AI>
		tinyxml2::XMLElement* pAI = document.NewElement("AI");
		// Sets <AI> value to selection AI setting
		pAI->SetText(pModel->isAI());
		// Inserts element into <Object>
		pObject->InsertEndChild(pAI);
	}

	// Saves the document
	document.SaveFile(ksFilePath.c_str());

	// Resets autosave timer
	m_fAutosaveTimer = 0.0f;

	std::cerr << "[EDITOR] File saved." << std::endl;
}

// Void: Loads a Scene into the Editor
void Editor::load(const std::string ksFilePath)
{
	std::cerr << "[EDITOR] Loading from file..." << std::endl;

	// Clears current Scene
	while (!m_pModels.empty())
	{
		m_pModels.pop_back();
	}

	// Creates a SceneReader that loads the file into a vector
	SceneReader reader = SceneReader(ksFilePath);
	std::vector<Model> loadedModels = reader.m_modelList;

	while (!loadedModels.empty())
	{
		// Initialises Model
		loadedModels.back().initModel();

		// Pushes Model onto Editor vector
		m_pModels.push_back(std::make_shared<Model>(loadedModels.back()));

		// Removes Model from loaded vector
		loadedModels.pop_back();
	}

	// Resets autosave timer
	m_fAutosaveTimer = 0.0f;

	std::cerr << "[EDITOR] File loaded." << std::endl;
}

// Void: ModelSelection button action
void Editor::button_select()
{
	// If obj file doesn't exist
	if (!fileExists("assets/models/" + m_select.m_pObjField->getStr() + ".obj"))
	{
		std::cerr << "[EDITOR] ERROR - OBJ FILE DOESN'T EXIST: " << ("assets/models/" + m_select.m_pObjField->getStr() + ".obj") << std::endl;
		// Outputs popup on HUD
		m_popUp.newPopUp("ERROR - OBJ FILE DOESN'T EXIST: " + ("assets/models/" + m_select.m_pObjField->getStr() + ".obj"), 2.0f);
	}
	// Else - obj exists
	else
	{
		// If texture file doesn't exist
		if (!fileExists("assets/textures/" + m_select.m_pTexField->getStr() + ".bmp"))
		{
			std::cerr << "[EDITOR] ERROR - BMP FILE DOESN'T EXIST: " << ("assets/textures/" + m_select.m_pTexField->getStr() + ".bmp") << std::endl;
			// Outputs popup on HUD
			m_popUp.newPopUp("ERROR - BMP FILE DOESN'T EXIST: " + ("assets/textures/" + m_select.m_pTexField->getStr() + ".bmp"), 2.0f);
		}
		// Else - texture exists
		else
		{
			// Creates Model
			std::shared_ptr<Model> pModel = std::shared_ptr<Model>(new Model());
			pModel->setName(m_select.m_pNameField->getStr());
			pModel->setFileLocation("assets/models/" + m_select.m_pObjField->getStr() + ".obj");
			pModel->setTextureLocation("assets/textures/" + m_select.m_pTexField->getStr() + ".bmp");
			pModel->setMaterial(1);

			// Loads Model so it's ready for drawing
			pModel->loadModel();
			// Initialises Model
			pModel->initModel();

			// Sets Model in hand to this new Model
			m_selection.m_pModel = pModel;

			// Closes menu
			m_state = EDITOR;
			m_select.reset();
		}
	}
}

// Void: SaveScene button action
void Editor::button_save()
{
	// Outputs popup on HUD
	m_popUp.newPopUp("Saving file: " + ("assets/scenes/" + m_save.m_pFileField->getStr() + ".xml") + "...", 2.0f);

	// Saves to file
	save("assets/scenes/" + m_save.m_pFileField->getStr() + ".xml");

	// Outputs popup on HUD
	m_popUp.newPopUp("File saved: " + ("assets/scenes/" + m_save.m_pFileField->getStr() + ".xml"), 2.0f);

	// Closes menu
	m_state = EDITOR;
	m_save.reset();
}

// Void: LoadScene button action
void Editor::button_load()
{
	// If xml file doesn't exist
	if (!fileExists("assets/scenes/" + m_load.m_pFileField->getStr() + ".xml"))
	{
		std::cerr << "[EDITOR] ERROR - XML FILE DOESN'T EXIST: " << ("assets/scenes/" + m_load.m_pFileField->getStr() + ".xml") << std::endl;
		// Outputs popup on HUD
		m_popUp.newPopUp("ERROR - XML FILE DOESN'T EXIST: " + ("assets/scenes/" + m_load.m_pFileField->getStr() + ".xml"), 2.0f);
	}
	// Else - xml exists
	else
	{
		// Outputs popup on HUD
		m_popUp.newPopUp("Loading file: " + ("assets/scenes/" + m_load.m_pFileField->getStr() + ".xml") + "...", 2.0f);

		// Loads file
		load("assets/scenes/" + m_load.m_pFileField->getStr() + ".xml");

		// Outputs popup on HUD
		m_popUp.newPopUp("File loaded: " + ("assets/scenes/" + m_load.m_pFileField->getStr() + ".xml"), 2.0f);

		// Closes menu
		m_state = EDITOR;
		m_load.reset();
	}
}