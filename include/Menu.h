#pragma once

#include "PreHeader.h"

#include "Scene.h"
#include "ModelReader.h"
#include "glslprogram.h"
#include "Freetype.h"

class Menu : public Scene
{
private:
	GLSLProgram m_FreeType;
	GLSLProgram m_ImageType;

	Freetype* UI;

	GLuint m_programHandle; // Program context
	glm::mat4 m_M; // Allows manipulation of each models position, rotation and scale

	void linkShaders(); // Connects shaders

	Texture *g_pTexture; // Holds texture
	GLFWwindow *m_pWindow; // The window
	sf::Vector2i m_mousePos; // Holds mouse cursor position
	glm::mat4 m_V, m_P;

public:
	Menu(sf::Vector2i windowSize);
	void initScene(Freetype* Overlay);
	void setMousePos(GLFWwindow *pWindow, sf::Vector2i mousepos);
	void update(float t);
	void render();
};
