#pragma once

#include "PreHeader.h"

#include "Scene.h"
#include "ModelReader.h"
#include "glslprogram.h"
#include "Freetype.h"
#include "Button.h"

class Menu : public Scene
{
private:
	GLSLProgram m_FreeType;
	GLSLProgram m_ImageType;

	Freetype* UI;

	Button *m_PlayButton;
	Button *m_EditorButton;
	Button *m_ExitButton;

	GLuint m_programHandle; // Program context
	glm::mat4 m_M; // Allows manipulation of each models position, rotation and scale

	void linkShaders(); // Connects shaders

	glm::mat4 m_V, m_P;

	enum State { None = 0, Play = 1, Create = 2, ExitMenu = 3 }; // Enum used for knowing which button is clicked
	State WhichState; // Instance of State enum

	bool m_bClicked;

public:
	Menu(GLFWwindow *pWindow, sf::Vector2i windowSize);
	void initScene(Freetype* Overlay);
	void setMousePos(sf::Vector2f mousepos);
	void update(float t);
	void render();
	int returnMenuChoice();
	void Click();
	void ResetClick();
};
