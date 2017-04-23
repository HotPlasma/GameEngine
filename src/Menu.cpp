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

Menu::Menu(sf::Vector2i windowSizes)
{
	m_windowSize = windowSizes;
}

void Menu::initScene(Freetype * Overlay)
{
	UI = Overlay; // Get the Heads up display for the scene

	linkShaders();
	// Stops rendered models from being transparent
	gl::Enable(gl::DEPTH_TEST);

	m_PlayButton = new Button(m_windowSize.x / 2, 700, 0, "assets/UI/Play.png", "assets/UI/PlayHover.png", glm::vec3(147.f, 46.f, 1.f), Overlay);

	m_EditorButton = new Button(m_windowSize.x / 2, 600, 0, "assets/UI/WorldEditor.png", "assets/UI/WorldEditorHover.png", glm::vec3(148.f, 46.f, 1.f), Overlay);

	m_ExitButton = new Button(m_windowSize.x / 2, 400, 0, "assets/UI/Exit.png", "assets/UI/ExitHover.png", glm::vec3(83, 46.f, 1.f), Overlay);
	

	//UI->LoadHUDImage("assets/UI/Play.png", glm::vec3(m_windowSize.x / 2, m_windowSize.y / 2, 1), 0, glm::vec3(147, 46, 1.f));
	UI->LoadHUDImage("assets/UI/BG.png", glm::vec3(m_windowSize.x / 2, m_windowSize.y / 2, 1), 0, glm::vec3(1920,1080,1.f), true);
}

void Menu::setMousePos(GLFWwindow * GWindow, sf::Vector2i mousepos)
{
	m_pWindow = GWindow;
	m_mousePos = mousepos;
}

void Menu::update(float t)
{
	m_V = mat4(1.0f);
	m_P = glm::perspective(90.f, (float)m_windowSize.x / m_windowSize.y, 1.f, 5000.f);
	m_PlayButton->CheckHover(m_mousePos, 0);
	m_EditorButton->CheckHover(m_mousePos, 2);
	m_ExitButton->CheckHover(m_mousePos, 4);
}

void Menu::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	

	m_ImageType.use();
	for (int i = 0; i < UI->m_ImagePlane.size(); i++)
	{
		m_ImageType.setUniform("M", UI->m_ImagePlane.at(i).m_M);
		m_ImageType.setUniform("P", glm::ortho(0.0f, 1920.0f, 0.f, 1080.f));
		if (UI->m_ImagePlane.at(i).getVisable() == true)
		{
			UI->RenderImage(i);
		}
	}
	
	//m_PlayButton->draw();

	m_FreeType.use();
	m_FreeType.setUniform("projection", glm::ortho(0.0f, 1920.0f, 0.f, 1080.f));
	UI->RenderText(m_FreeType.getHandle(), "Game Engine", m_windowSize.x / 2, 900, 1.0f, glm::vec3(1.f, 1.f, 1.f));

}

int Menu::returnMenuChoice()
{
	if (m_bClicked) // If clicked
	{
		if (m_PlayButton->isActive()) // New World button clicked
		{
			WhichState = Play;
		}
		else if (m_EditorButton->isActive()) // Load world button clicked
		{
			WhichState = Create;
		}
		else if (m_ExitButton->isActive()) // Exit button clicked
		{
			WhichState = ExitMenu;
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

