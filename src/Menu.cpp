#include "Menu.h"

Menu::Menu(GLFWwindow *pWindow, sf::Vector2i windowSizes)
{
	// Sets members with input
	m_pWindow = pWindow;
	m_windowSize = windowSizes;
}

void Menu::initScene(Freetype * pOverlay)
{
	// Get the Heads up display for the scene
	m_pHUD = pOverlay;

	// Sets cursor style
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	linkShaders();

	// Stops rendered models from being transparent
	gl::Enable(gl::DEPTH_TEST);

	m_buttons.m_pPlay = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2((float)m_windowSize.x*0.5f, (float)m_windowSize.y*0.65f),
			"assets/UI/Play.png",
			"assets/UI/PlayHover.png",
			glm::vec3(147.f, 46.f, 1.f),
			pOverlay
		)
	);
	m_buttons.m_pEditor = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2((float)m_windowSize.x*0.5f, (float)m_windowSize.y*0.55f),
			"assets/UI/WorldEditor.png", 
			"assets/UI/WorldEditorHover.png", 
			glm::vec3(148.f, 46.f, 1.f), 
			pOverlay
		)
	);
	m_buttons.m_pOptions = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2((float)m_windowSize.x*0.5f, (float)m_windowSize.y*0.45f),
			"assets/UI/Options.png", 
			"assets/UI/OptionsHover.png",
			glm::vec3(126.f, 46.f, 1.f),
			pOverlay
		)
	);
	m_buttons.m_pExit = std::shared_ptr<Button>
	(
		new Button
		(
			glm::vec2((float)m_windowSize.x*0.5f, (float)m_windowSize.y*0.35f),
			"assets/UI/Exit.png", 
			"assets/UI/ExitHover.png",
			glm::vec3(83, 46.f, 1.f),
			pOverlay
		)
	);
	
	// Loads main menu music
	if (!m_menuTheme.loadFromFile("assets/sounds/MainMenu.wav")) { /* Nothing if load failed */ }

	// Sets BG index to image plane size
	m_uiBGIndex = (unsigned int)m_pHUD->m_imagePlane.size();
	// Loads menu background into HUD
	m_pHUD->addImage("assets/UI/BG.png", glm::vec3((float)m_windowSize.x*0.5f, (float)m_windowSize.y*0.5f, 1.0f), 0, glm::vec3((float)m_windowSize.x, (float)m_windowSize.y, 1.f), true);
}

// Void: Called on key input event
void Menu::input_key(const int kiKey, const int kiAction)
{
	// If action is a key press
	if (kiAction == GLFW_PRESS)
	{
		// If Esc key pressed
		if (kiKey == GLFW_KEY_ESCAPE) closeProgram();
	}
}

// Void: Called on mouseButton input event
void Menu::input_button(const int kiButton, const int kiAction)
{
	// If action is a button press
	if (kiAction == GLFW_PRESS)
	{
		// If left mouse button is clicked
		if (kiButton == GLFW_MOUSE_BUTTON_LEFT)
		{
			// If menu play button is clicked
			if (m_buttons.m_pPlay->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				m_intention = TO_GAME;
			}

			// If menu editor button is clicked
			if (m_buttons.m_pEditor->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				m_intention = TO_EDITOR;
			}

			// If menu options button is clicked
			if (m_buttons.m_pOptions->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				// TEMPORARY NOTHING
			}

			// If menu exit button is clicked
			if (m_buttons.m_pExit->mouseOver(m_mousePos, (float)m_windowSize.y))
			{
				closeProgram();
			}
		}
	}
}

void Menu::update(const float kfTimeElapsed)
{
	// If music is not playing
	if (m_music.getStatus() != sf::Sound::Playing)
	{
		// Play music
		m_music.setBuffer(m_menuTheme);
		m_music.setVolume(40.0f);
		m_music.play();
	}

	// Checks whether HUD buttons are hovered
	m_buttons.m_pPlay->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_buttons.m_pEditor->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_buttons.m_pOptions->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_buttons.m_pExit->mouseOver(m_mousePos, (float)m_windowSize.y);
}

void Menu::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Draws HUD buttons
	m_buttons.m_pPlay->render(&m_imageType, m_windowSize);
	m_buttons.m_pEditor->render(&m_imageType, m_windowSize);
	m_buttons.m_pOptions->render(&m_imageType, m_windowSize);
	m_buttons.m_pExit->render(&m_imageType, m_windowSize);
	
	//Draws Background		
	m_imageType.setUniform("M", m_pHUD->m_imagePlane.at(m_uiBGIndex).getM());
	m_imageType.setUniform("P", glm::ortho(0.0f, (float)m_windowSize.x, 0.f, (float)m_windowSize.y));
	m_pHUD->renderImage(&m_imageType, m_uiBGIndex);
}
