#include "..\include\Menu.h"

void Menu::linkShaders()
{
	try 
	{
		// Shader which allows first person camera and textured objects
		m_FreeType.compileShader("Shaders/freetype.vert");
		m_FreeType.compileShader("Shaders/freetype.frag");
		m_FreeType.link();
		m_FreeType.validate();

		// Shader which allows first person camera and textured objects
		m_ImageType.compileShader("Shaders/image.vert");
		m_ImageType.compileShader("Shaders/image.frag");
		m_ImageType.link();
		m_ImageType.validate();
	}
	catch (GLSLProgramException & e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

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

	m_PlayButton = new Button(m_windowSize.x / 2, 700, "assets/UI/Play.png", "assets/UI/PlayHover.png", glm::vec3(147.f, 46.f, 1.f), Overlay);

	m_EditorButton = new Button(m_windowSize.x / 2, 600, "assets/UI/WorldEditor.png", "assets/UI/WorldEditorHover.png", glm::vec3(148.f, 46.f, 1.f), Overlay);

	m_OptionsButton = new Button(m_windowSize.x / 2, 500, "assets/UI/Options.png", "assets/UI/OptionsHover.png", glm::vec3(126.f, 46.f, 1.f), Overlay);

	m_ExitButton = new Button(m_windowSize.x / 2, 400, "assets/UI/Exit.png", "assets/UI/ExitHover.png", glm::vec3(83, 46.f, 1.f), Overlay);
	
	if (!MenuTheme.loadFromFile("assets/sounds/MainMenu.wav"));
	{

	}

	//UI->LoadHUDImage("assets/UI/Play.png", glm::vec3(m_windowSize.x / 2, m_windowSize.y / 2, 1), 0, glm::vec3(147, 46, 1.f));
	UI->LoadHUDImage("assets/UI/BG.png", glm::vec3(m_windowSize.x / 2, m_windowSize.y / 2, 1), 0, glm::vec3(1920,1080,1.f), true);
}

void Menu::setMousePos(sf::Vector2f mousepos)
{
	m_mousePos = mousepos;
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
	m_P = glm::perspective(90.f, (float)m_windowSize.x / m_windowSize.y, 1.f, 5000.f);
	m_PlayButton->mouseOver(m_mousePos, m_windowSize.y);
	m_EditorButton->mouseOver(m_mousePos, m_windowSize.y);
	m_OptionsButton->mouseOver(m_mousePos, m_windowSize.y);
	m_ExitButton->mouseOver(m_mousePos, m_windowSize.y);
}

void Menu::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Draws HUD buttons
	m_PlayButton->render(&m_ImageType, m_windowSize);
	m_EditorButton->render(&m_ImageType, m_windowSize);
	m_OptionsButton->render(&m_ImageType, m_windowSize);
	m_ExitButton->render(&m_ImageType, m_windowSize);
	
	//Draws Background		
	m_ImageType.setUniform("M", UI->m_ImagePlane.at(8).m_M);
	m_ImageType.setUniform("P", glm::ortho(0.0f, (float)m_windowSize.x, 0.f, (float)m_windowSize.y));
	UI->RenderImage(8);
}

int Menu::returnMenuChoice()
{
	if (m_bClicked) // If clicked
	{
		if (m_PlayButton->mouseOver(m_mousePos, m_windowSize.y)) // New World button clicked
		{
			WhichState = Play;
		}
		else if (m_EditorButton->mouseOver(m_mousePos, m_windowSize.y)) // Load world button clicked
		{
			WhichState = Create;
		}
		else if (m_OptionsButton->mouseOver(m_mousePos, m_windowSize.y)) // Exit button clicked
		{
			WhichState = Options;
		}
		else if (m_ExitButton->mouseOver(m_mousePos, m_windowSize.y)) // Exit button clicked
		{
			WhichState = ExitMenu;
		}
		m_bClicked = false; // "Unclick" button
		Music.stop();
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

