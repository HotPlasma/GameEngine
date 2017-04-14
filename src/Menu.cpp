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

Menu::Menu(sf::Vector2i windowSize)
{
	m_windowSize = windowSize;
}

void Menu::initScene(Freetype * Overlay)
{
	UI = Overlay; // Get the Heads up display for the scene

	linkShaders();
	// Stops rendered models from being transparent
	gl::Enable(gl::DEPTH_TEST);

	UI->LoadHUDImage("assets/UI/BG.png", glm::vec3(m_windowSize.x / 2, m_windowSize.y / 2, 1), 0, glm::vec3(1920,1080,1.f));
	UI->LoadHUDImage("assets/UI/Play.png", glm::vec3(m_windowSize.x / 2, m_windowSize.y / 2, 1), 0, glm::vec3(147, 46, 1.f));
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
}

void Menu::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	/*m_FreeType.use();
	m_FreeType.setUniform("projection", glm::ortho(0.0f, 1920.0f, 0.f, 1080.f));
	UI->RenderText(m_FreeType.getHandle(), "Collectable Collected", 100.f, 100.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));*/

	m_ImageType.use();

	for (int i = 0; i < UI->m_ImagePlane.size(); i++)
	{
		m_ImageType.setUniform("M", UI->m_ImagePlane.at(i).m_M);
		m_ImageType.setUniform("P", glm::ortho(0.0f, 1920.0f, 0.f, 1080.f));
		UI->RenderImage(i);
	}
	
}
