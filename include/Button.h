
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
	Button(); 

	// Overloaded constructer which contructs a button from a X position, Y position, Texture ID and a TextureLoader
	Button(float fX, float fY, string ButtonLoc, string ButtonHoverLoc, vec3 scale, Freetype *Overlay);

	// Checks if the button is being hovered over
	bool mouseOver(const sf::Vector2f kMousePos);

	// Draws the button
	void render();
};