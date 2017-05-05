#include "..\include\Menu.h"

Menu::Menu(GLFWwindow *pWindow, sf::Vector2i windowSizes)
{
	m_pWindow = pWindow;
	m_windowSize = windowSizes;
}

void Menu::initScene(Freetype * Overlay)
{
	UI = Overlay; // Get the Heads up display for the scene

	linkShaders();
	// Stops rendered models from being transparent
	gl::Enable(gl::DEPTH_TEST);

	m_PlayButton = new Button(glm::vec2((float)m_windowSize.x*0.5f, 700.0f), "assets/UI/Play.png", "assets/UI/PlayHover.png", glm::vec3(147.f, 46.f, 1.f), Overlay);

	m_EditorButton = new Button(glm::vec2((float)m_windowSize.x*0.5f, 600.0f), "assets/UI/WorldEditor.png", "assets/UI/WorldEditorHover.png", glm::vec3(148.f, 46.f, 1.f), Overlay);

	m_OptionsButton = new Button(glm::vec2((float)m_windowSize.x*0.5f, 500.0f), "assets/UI/Options.png", "assets/UI/OptionsHover.png", glm::vec3(126.f, 46.f, 1.f), Overlay);

	m_ExitButton = new Button(glm::vec2((float)m_windowSize.x*0.5f, 400.0f), "assets/UI/Exit.png", "assets/UI/ExitHover.png", glm::vec3(83, 46.f, 1.f), Overlay);
	
	if (!MenuTheme.loadFromFile("assets/sounds/MainMenu.wav"))
	{
		// Nothing
	}

	//UI->LoadHUDImage("assets/UI/Play.png", glm::vec3(m_windowSize.x / 2, m_windowSize.y / 2, 1), 0, glm::vec3(147, 46, 1.f));
	UI->LoadHUDImage("assets/UI/BG.png", glm::vec3((float)m_windowSize.x*0.5f, (float)m_windowSize.y*0.5f, 1.0f), 0, glm::vec3((float)m_windowSize.x, (float)m_windowSize.y, 1.f), true);
}

void Menu::update(float t)
{
	if (Music.getStatus() != sf::Sound::Playing)
	{
		Music.setBuffer(MenuTheme);
		Music.setVolume(40);
		Music.play();
	}

	m_V = mat4(1.0f);
	m_P = glm::perspective(90.f, (float)m_windowSize.x / (float)m_windowSize.y, 1.f, 5000.f);
	m_PlayButton->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_EditorButton->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_OptionsButton->mouseOver(m_mousePos, (float)m_windowSize.y);
	m_ExitButton->mouseOver(m_mousePos, (float)m_windowSize.y);
}

void Menu::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Draws HUD buttons
	m_PlayButton->render(&m_imageType, m_windowSize);
	m_EditorButton->render(&m_imageType, m_windowSize);
	m_OptionsButton->render(&m_imageType, m_windowSize);
	m_ExitButton->render(&m_imageType, m_windowSize);
	
	//Draws Background		
	m_imageType.setUniform("M", UI->m_ImagePlane.at(8).getM());
	m_imageType.setUniform("P", glm::ortho(0.0f, (float)m_windowSize.x, 0.f, (float)m_windowSize.y));
	UI->RenderImage(&m_imageType, 8);
}

int Menu::returnMenuChoice()
{
	if (m_bClicked) // If clicked
	{
		if (m_PlayButton->mouseOver(m_mousePos, (float)m_windowSize.y)) // New World button clicked
		{
			Music.stop();
			WhichState = Play;
			
		}
		else if (m_EditorButton->mouseOver(m_mousePos, (float)m_windowSize.y)) // Load world button clicked
		{
			WhichState = Create;
			Music.stop();
		}
		else if (m_OptionsButton->mouseOver(m_mousePos, (float)m_windowSize.y)) // Exit button clicked
		{
			WhichState = Options;
			Music.stop();
		}
		else if (m_ExitButton->mouseOver(m_mousePos, (float)m_windowSize.y)) // Exit button clicked
		{
			WhichState = ExitMenu;
			Music.stop();
		}
		m_bClicked = false; // "Unclick" button
		
		return WhichState; // Return which button was clicked
	}
	return WhichState = None; // If no button clicked return none
}

void Menu::Click()
{
	// To be run if mouse clicked
	m_bClicked = true;
}

void Menu::ResetClick()
{
	// Set click to false
	m_bClicked = false;
}

