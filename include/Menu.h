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

	Freetype* UI;

	Button *m_PlayButton;
	Button *m_EditorButton;
	Button *m_OptionsButton;
	Button *m_ExitButton;

	GLuint m_programHandle; // Program context
	glm::mat4 m_M; // Allows manipulation of each models position, rotation and scale

	glm::mat4 m_V, m_P;

	enum State { None = 0, Play = 1, Create = 2, Options = 3, ExitMenu = 4 }; // Enum used for knowing which button is clicked
	State WhichState; // Instance of State enum

	bool m_bClicked;

	sf::SoundBuffer MenuTheme;

public:
	Menu(GLFWwindow *pWindow, sf::Vector2i windowSize);
	void initScene(Freetype* Overlay);

	void input_key(const int kiKey, const int kiAction) {}
	void input_char(const unsigned int kuiCodepoint) {}
	void input_button(const int kiButton, const int kiAction) {}
	void input_scroll(const double kdDelta) {}

	void update(const float kfTimeElapsed);
	void render();
	int returnMenuChoice();
	void Click();
	void ResetClick();
};
