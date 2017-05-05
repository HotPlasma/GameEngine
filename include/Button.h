#pragma once

#include <iostream>
#include <PreHeader.h>
#include <Freetype.h>

class Button
{
private:

	// Freetype HUD object
	Freetype *m_pHUD;

	// Texture index in the freetype image plane
	unsigned int m_uiTextureIndex;
	// HoverTexture index in the freetype image plane
	unsigned int m_uiHoverTextureIndex;

	// Button position
	glm::vec2 m_position;
	// Button scale
	glm::vec2 m_scale;

public:

	// Default constructor
	Button() {};

	// Overloaded constructer which contructs a button from a X position, Y position, Texture ID and a TextureLoader
	Button(const glm::vec2 kPosition, const string ksLoc, const string ksHoverLoc, const vec3 kScale, Freetype *pOverlay);

	// Sets Texture index
	void setTextureIndex(const unsigned int kuiIndex) { m_uiTextureIndex = kuiIndex; }
	// Gets Texture index
	unsigned int getTextureIndex() { return m_uiTextureIndex; }
	// Sets HoverTexture index
	void setHoverTextureIndex(const unsigned int kuiIndex) { m_uiHoverTextureIndex = kuiIndex; }
	// Gets HoverTexture index
	unsigned int getHoverTextureIndex() { return m_uiHoverTextureIndex; }

	// Checks if the button is being hovered over
	bool mouseOver(sf::Vector2f mousePos, const float kfWindowYSize);

	// Draws the button
	void render(GLSLProgram* pShader, const sf::Vector2i kWindowSize);
};

class ToggleButton
{
private:

	// Whether the button is toggled on or off
	bool m_bToggle = false; // Default false

	// The button while toggled True
	std::shared_ptr<Button> m_pTrueButton;
	// The button while toggled False
	std::shared_ptr<Button> m_pFalseButton;

public:

	// Default constructor
	ToggleButton() {}

	// Constructor with two buttons for True and False variants
	ToggleButton(const bool kbToggle, std::shared_ptr<Button> pTrueButton, std::shared_ptr<Button> pFalseButton)
	{
		// Assigns parameters to members
		m_bToggle = kbToggle;
		m_pTrueButton = pTrueButton;
		m_pFalseButton = pFalseButton;
	}

	// Toggles the toggle member
	void toggle() { m_bToggle = !m_bToggle; }

	// Returns the True/False button based on current toggle
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

	// Returns toggle member
	bool isToggled() { return m_bToggle; }
	// Sets the toggle member with new value
	void setToggled(const bool kbToggle) { m_bToggle = kbToggle; }
};