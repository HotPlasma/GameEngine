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
	Button(const glm::vec2 kPosition, const string ksLoc, const string ksHoverLoc, const vec3 kScale, Freetype *pOverlay);

	// Sets Texture index
	void setTextureIndex(const unsigned int kuiIndex) { m_uiTextureIndex = kuiIndex; }
	unsigned int getTextureIndex() { return m_uiTextureIndex; }
	void setHoverTextureIndex(const unsigned int kuiIndex) { m_uiHoverTextureIndex = kuiIndex; }
	unsigned int getHoverTextureIndex() { return m_uiHoverTextureIndex; }

	// Checks if the button is being hovered over
	bool mouseOver(sf::Vector2f mousePos, const float kfWindowYSize);

	// Draws the button
	void render(GLSLProgram* pShader, const sf::Vector2i kWindowSize);
};

class ToggleButton
{
private:

	bool m_bToggle = false;

	std::shared_ptr<Button> m_pTrueButton;
	std::shared_ptr<Button> m_pFalseButton;

public:

	// Default constructor
	ToggleButton() {}

	ToggleButton(std::shared_ptr<Button> pTrueButton, std::shared_ptr<Button> pFalseButton)
	{
		m_pTrueButton = pTrueButton;
		m_pFalseButton = pFalseButton;

		m_bToggle = false;
	}

	void toggle() { m_bToggle = !m_bToggle; }

	std::shared_ptr<Button> getButton()
	{ 
		// If toggle true
		if (m_bToggle)
			// Sets active button to trueButton
			return m_pTrueButton;

		// Else toggle false
		else
			// Sets active button to falseButton
			return m_pFalseButton;
	}

	bool isToggled() { return m_bToggle; }
	void setToggled(const bool kbToggle) { m_bToggle = kbToggle; }
};