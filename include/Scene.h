#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "PreHeader.h"

#include "Camera.h"
#include "Freetype.h"

enum Intent { NONE, TO_MENU, TO_GAME, TO_EDITOR };

// Abstract class in order to set up world
class Scene
{
public:

	// Default constructor
	Scene() {}
	
	sf::Vector2i m_windowSize; // Dimensions of window

	Intent m_intention = NONE;

	sf::Sound m_music;

	// Load in all texture and initilise shaders
	virtual void initScene(Freetype* pOverlay) = 0;

	// Called on key input event
	virtual void input_key(const int kiKey, const int kiAction) = 0;
	// Called on char input event
	virtual void input_char(const unsigned int kuiUnicode) = 0;
	// Called on mouseButton input event
	virtual void input_button(const int kiButton, const int kiAction) = 0;
	// Called on mouseScroll input event
	virtual void input_scroll(const double kdDelta) = 0;

	// Run every frame
	virtual void update(const float kfTimeElapsed) = 0;

	// Draw Scene
	virtual void render() = 0;

	// Allow screen to be resized without causing rendering issues
	void resize(int iWidth, int iHeight)
	{
		m_windowSize.x = iWidth;
		m_windowSize.y = iHeight;

		gl::Viewport(0, 0, m_windowSize.x, m_windowSize.y);
		m_camera.setAspectRatio((float)m_windowSize.x / m_windowSize.y);
	}

	// Used to update cursor position
	void setMousePos(const sf::Vector2f kMousePos) { m_mousePos = kMousePos; }

	sf::Vector2i getWindowSize() { return m_windowSize; }
    
protected:

	GLSLProgram m_textureShader;
	GLSLProgram m_phongShader;
	GLSLProgram m_spotlightShader;
	GLSLProgram m_freeType;
	GLSLProgram m_imageType;

	Camera m_camera; // Camera which user can control

	Freetype* m_pHUD;

	GLFWwindow *m_pWindow; // The window
	sf::Vector2f m_mousePos; // Holds mouse cursor position

	// Links vert and frag shaders into a glslprogram
	void linkShaders()
	{
		try
		{
			// Shader which allows blank flat texture rendering
			m_textureShader.compileShader("Shaders/texture.vs");
			m_textureShader.compileShader("Shaders/texture.fs");
			m_textureShader.link();
			m_textureShader.validate();
			m_textureShader.use();
		}
		catch (GLSLProgramException & e)
		{
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}

		try
		{
			// Shader which allows first person camera and textured object rendering
			m_phongShader.compileShader("Shaders/phong.vs");
			m_phongShader.compileShader("Shaders/phong.fs");
			m_phongShader.link();
			m_phongShader.validate();
			m_phongShader.use();
		}
		catch (GLSLProgramException & e)
		{
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}

		try
		{
			// Shader which allows for a spotlight
			m_spotlightShader.compileShader("Shaders/spotlight.vs");
			m_spotlightShader.compileShader("Shaders/spotlight.fs");
			m_spotlightShader.link();
			m_spotlightShader.validate();
			m_spotlightShader.use();
		}
		catch (GLSLProgramException & e)
		{
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}

		try
		{
			// Shader which allows heads up display rendering
			m_freeType.compileShader("Shaders/freetype.vs");
			m_freeType.compileShader("Shaders/freetype.fs");
			m_freeType.link();
			m_freeType.validate();
			m_freeType.use();
		}
		catch (GLSLProgramException & e) {
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}

		try
		{
			// Shader which allows for image rendering
			m_imageType.compileShader("Shaders/image.vs");
			m_imageType.compileShader("Shaders/image.fs");
			m_imageType.link();
			m_imageType.validate();
		}
		catch (GLSLProgramException & e)
		{
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}
};

#endif // SCENE_H
