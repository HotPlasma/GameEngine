#include "..\include\Button.h"

Button::Button()
{

}

Button::Button(float fX, float fY, int iTextureID, string ButtonLoc, string ButtonHoverLoc, vec3 scale, Freetype *Overlay)
{
	SetProperties(fX, fY, iTextureID, ButtonLoc , ButtonHoverLoc, scale, Overlay); // Create button
}

void Button::SetProperties(float fX, float fY, int iTextureID, string ButtonLoc, string ButtonHoverLoc, vec3 scale, Freetype *Overlay)
{
	m_ButtonSprite = Overlay;
	m_ButtonSprite->LoadHUDImage(ButtonLoc, vec3(fX, fY, 1), 0, scale, false);
	m_ButtonSprite->LoadHUDImage(ButtonHoverLoc, vec3(fX, fY, 1), 0, scale, false);
	ButtonPos = vec2(fX, fY);
	m_Scale = vec2(scale);
	m_bActive = true;
}

void Button::ChangeTexture(int iNewTextureID)
{
	
}

void Button::CheckHover(sf::Vector2f MousePos, int index)
{
	
	MousePos.y = 1080 - MousePos.y; // Origins mouse at top-left instead of bottom left.

	// Mouse is hovering over button
	if (MousePos.x > (ButtonPos.x - (m_Scale.x /2)) && MousePos.x < (ButtonPos.x - (m_Scale.x / 2)) +  m_Scale.x  &&
		MousePos.y > (ButtonPos.y - (m_Scale.y / 2)) && MousePos.y < (ButtonPos.y - (m_Scale.y / 2)) + m_Scale.y)
	/*if (MousePos.x > 100)*/
	{
		//ChangeTexture(m_iCurrentTexID + 1); // Change texture to highlighted texture
		m_bActive = true;
		m_ButtonSprite->m_ImagePlane.at(index + 1).setVisable(true);
		m_ButtonSprite->m_ImagePlane.at(index).setVisable(false);
	}
	// Mouse is not pointing at button
	else
	{
		/*ChangeTexture(m_iCurrentTexID);*/
		m_bActive = false;
		m_ButtonSprite->m_ImagePlane.at(index).setVisable(true);
		m_ButtonSprite->m_ImagePlane.at(index + 1).setVisable(false);
	}
}

void Button::draw()
{
	//if (m_bActive)
	//{
	//	m_ButtonSprite->RenderImage(0);
	//}
	//else
	/*{
		m_ButtonSprite->RenderImage(1);
	}*/
}

bool Button::isActive()
{
	return m_bActive;
}

Button::~Button()
{
}
