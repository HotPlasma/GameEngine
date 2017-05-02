#pragma once

#include <iostream>
#include <PreHeader.h>
#include <Freetype.h>

class Button
{
private:

	Freetype *m_pHUD;

	unsigned int m_uiTextureIndex;
	unsigned int m_uiHoverTextureIndex;

	glm::vec2 m_position;
	glm::vec2 m_scale;

public:

	// Default constructor
	Button() {};

	// Overloaded constructer which contructs a button from a X position, Y position, Texture ID and a TextureLoader
	Button(const float kfX, const float kfY, const string ksLoc, const string ksHoverLoc, const vec3 kScale, Freetype *pOverlay);

	// Checks if the button is being hovered over
	bool mouseOver(sf::Vector2f mousePos, const float kfWindowYSize);

	// Draws the button
	void render(GLSLProgram* pShader, const sf::Vector2i kWindowSize);
};