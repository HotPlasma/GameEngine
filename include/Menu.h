#pragma once
#ifndef MENU_H
#define MENU_H

#include "PreHeader.h"

#include "Utilities.h"
#include "Scene.h"
#include "World.h"
#include "Editor.h"
#include "ModelReader.h"
#include "Freetype.h"
#include "Button.h"

struct MenuHUD
{
	std::shared_ptr<Button> m_pPlay;
	std::shared_ptr<Button> m_pEditor;
	std::shared_ptr<Button> m_pExit;
};

class Menu : public Scene
{
private:

	MenuHUD m_buttons;

	unsigned int m_uiBGIndex; //!< Background imageplane index
	sf::SoundBuffer m_menuTheme;

public:

	Menu(GLFWwindow *pWindow, sf::Vector2i windowSize);

	void initScene(Freetype* Overlay);

	void input_key(const int kiKey, const int kiAction);
	void input_char(const unsigned int kuiUnicode) {}
	void input_button(const int kiButton, const int kiAction);
	void input_scroll(const double kdDelta) {}

	void update(const float kfTimeElapsed);
	void render();
};

#endif