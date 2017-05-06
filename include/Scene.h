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
			m_freeType.compileShader("Shaders/freetype.vert");
			m_freeType.compileShader("Shaders/freetype.frag");
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
			m_imageType.compileShader("Shaders/image.vert");
			m_imageType.compileShader("Shaders/image.frag");
			m_imageType.link();
			m_imageType.validate();
		}
		catch (GLSLProgramException & e)
		{
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void setLightParams(GLSLProgram *pShader)
	{
		pShader->setUniform("Material.Ka", glm::vec3(0.2f, 0.2f, 0.2f));
		pShader->setUniform("Material.Kd", glm::vec3(0.5f, 0.5f, 0.5f));
		pShader->setUniform("Material.Ks", glm::vec3(1.0f, 1.0f, 1.0f));
		pShader->setUniform("Material.Shininess", 20.0f);

		pShader->setUniform("Light.Intensity", glm::vec3(0.1f, 0.1f, 0.1f));
		pShader->setUniform("Light.Position", m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);

		pShader->setUniform("Spotlight.Direction", m_camera.getPosition() + m_camera.getDirection());
		pShader->setUniform("Spotlight.CutOff", glm::cos(glm::radians(15.0f)));
		pShader->setUniform("Spotlight.OuterCutOff", glm::cos(glm::radians(22.5f)));

		//pShader->setUniform("light.direction", m_camera.getPosition().x + m_camera.getDirection().x, m_camera.getPosition().y + m_camera.getDirection().y, m_camera.getPosition().z + m_camera.getDirection().z);
		//pShader->setUniform("light.cutOff", glm::cos(glm::radians(25.5f)));
		//pShader->setUniform("light.outerCutOff", glm::cos(glm::radians(35.5f)));
		//
		//pShader->setUniform("light.ambient", 0.3f, 0.3f, 0.3f);
		//pShader->setUniform("light.diffuse", 0.5f, 0.5f, 0.5f);
		//pShader->setUniform("light.specular", 0.8f, 0.8f, 0.8f);
		//pShader->setUniform("light.constant", 1.0f);
		//pShader->setUniform("light.linear", 0.09f);
		//pShader->setUniform("light.quadratic", 0.032f);

		//if (m_sceneReader.m_modelList.at(i).getMaterial() == 1) //Wooden material
		//{
		//	m_spotlightShader.setUniform("Id", 0.5f, 0.5f, 0.5f);
		//	m_spotlightShader.setUniform("Is", 0.4f, 0.4f, 0.4f);
		//	m_spotlightShader.setUniform("Rd", 0.6f, 0.6f, 0.6f);
		//	m_spotlightShader.setUniform("Rs", 0.3f, 0.3f, 0.3f);
		//}
		//else if (m_sceneReader.m_modelList.at(i).getMaterial() == 2) //Metal material
		//{
		//	m_spotlightShader.setUniform("Id", 0.7f, 0.7f, 0.7f);
		//	m_spotlightShader.setUniform("Is", 0.5f, 0.5f, 0.5f);
		//	m_spotlightShader.setUniform("Rd", 0.8f, 0.8f, 0.8f);
		//	m_spotlightShader.setUniform("Rs", 0.8f, 0.8f, 0.8f);
		//}
		//else if (m_sceneReader.m_modelList.at(i).getMaterial() == 3) //Deer material
		//{
		//	m_spotlightShader.setUniform("Id", 0.5f, 0.5f, 0.5f);
		//	m_spotlightShader.setUniform("Is", 0.3f, 0.3f, 0.3f);
		//	m_spotlightShader.setUniform("Rd", 0.7f, 0.7f, 0.7f);
		//	m_spotlightShader.setUniform("Rs", 0.5f, 0.5f, 0.5f);
		//}
		//else if (m_sceneReader.m_modelList.at(i).getMaterial() == 4) //Skybox material
		//{
		//	m_spotlightShader.setUniform("Id", 1.0f, 1.0f, 1.0f);
		//	m_spotlightShader.setUniform("Is", 0.0f, 0.0f, 0.0f);
		//	m_spotlightShader.setUniform("Rd", 0.0f, 0.0, 0.0f);
		//	m_spotlightShader.setUniform("Rs", 0.0f, 0.0f, 0.0f);
		//}
	}
};

#endif // SCENE_H
