
#pragma once

#include <iostream>
#include <PreHeader.h>
#include <Freetype.h>

class Button
{
private:
	////////////////////////////////////////////////////////////
	// Member data
	////////////////////////////////////////////////////////////
	Freetype *m_ButtonSprite;
	Freetype *m_ButtonHoverSprite;
	int m_iCurrentTexID; ///< int which represents the assigned texture for button
	bool m_bActive; ///< bool which becomes active if button is hovered over
	glm::vec2 ButtonPos;
	glm::vec2 m_Scale;

public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Allows creation of a button without variables.
	///
	////////////////////////////////////////////////////////////
	Button(); 

	////////////////////////////////////////////////////////////
	/// \brief Overloaded constructer which contructs a button from a X position, Y position, Texture ID and a TextureLoader
	///
	/// \param fX X Position of button
	/// \param fY Y Position of button
	/// \param iTextureID int Texture assigned to button
	/// \param TexLoader TextureLoader holding textures for button
	///
	/// \see SetProperties
	///
	////////////////////////////////////////////////////////////
	Button(float fX, float fY, int iTextureID, string ButtonLoc, string ButtonHoverLoc, vec3 scale, Freetype *Overlay); // Creates button with given parameters

	////////////////////////////////////////////////////////////
	/// \brief Creates a button using these given parameters
	///
	/// \param fX X Position of button
	/// \param fY Y Position of button
	/// \param iTextureID int Texture assigned to button
	/// \param TexLoader TextureLoader holding textures for button
	///
	/// \see Button
	///
	////////////////////////////////////////////////////////////
	void SetProperties(float fX, float fY, int iTextureID, string ButtonLoc, string ButtonHoverLoc, vec3 scale, Freetype *Overlay);

	////////////////////////////////////////////////////////////
	/// \brief Changes active texture on the button
	///
	/// \param iNewTextureID Texture index of buttons TextureLoader
	///
	/// \see m_TexLoader
	///
	////////////////////////////////////////////////////////////
	void ChangeTexture(int index);

	////////////////////////////////////////////////////////////
	/// \brief Checks if the button is being hovered over
	///
	/// \param MousePos Mouse cursor position
	///
	///
	////////////////////////////////////////////////////////////
	void CheckHover(sf::Vector2f MousePos, int index);

	////////////////////////////////////////////////////////////
	/// \brief Allows class to draw drawables to the screen
	///
	/// \param target Drawable to be drawn
	/// \param states States to be drawn
	///
	////////////////////////////////////////////////////////////
	void draw();

	////////////////////////////////////////////////////////////
	/// \brief Returns true if button is being hovered over
	///
	///
	////////////////////////////////////////////////////////////
	bool isActive();
	~Button(); 

};

////////////////////////////////////////////////////////////
/// \class Button
///	
/// Allows creation of clickable sprites which can perform actions.
/// These sprites are used within the menu system for this project.
/// 
/// Buttons check if they are being hovered and are able to change
/// their texture if they are. If a button is clicked while being hovered
/// it will return its m_bActive bool as true allowing for tasked to be
/// run and completed because of it.
/// 
/// To be instantiated a button needs a X position, a Y position, a int which
/// represents which texture it should currently have and a textureloader
/// 
////////////////////////////////////////////////////////////