#include "..\include\Button.h"

Button::Button()
{

}

Button::Button(const float kfX, const float kfY, const string ksLoc, const string ksHoverLoc, const vec3 kScale, Freetype *pOverlay)
{
	m_pHUD = pOverlay;

	m_pHUD->LoadHUDImage(ksLoc, vec3(kfX, kfY, 1), 0, kScale, false);
	m_uiTextureIndex = m_pHUD->m_ImagePlane.size();

	m_pHUD->LoadHUDImage(ksLoc, vec3(kfX, kfY, 1), 0, kScale, false);
	m_uiHoverTextureIndex = m_pHUD->m_ImagePlane.size();

	m_position = vec2(kfX, kfY);
	m_scale = vec2(kScale);
}

bool Button::mouseOver(sf::Vector2f mousePos)
{
	mousePos.y = 1080 - mousePos.y; // Origins mouse at top-left instead of bottom left.

	// Mouse is over over Button
	if (mousePos.x > (m_position.x - (m_scale.x /2)) && mousePos.x < (m_position.x - (m_scale.x / 2)) + m_scale.x  &&
		mousePos.y > (m_position.y - (m_scale.y / 2)) && mousePos.y < (m_position.y - (m_scale.y / 2)) + m_scale.y)
	{
		m_pHUD->m_ImagePlane.at(m_uiHoverTextureIndex).setVisable(true);
		m_pHUD->m_ImagePlane.at(m_uiTextureIndex).setVisable(false);

		return true;
	}
	// Mouse is not over Button
	else
	{
		m_pHUD->m_ImagePlane.at(m_uiTextureIndex).setVisable(true);
		m_pHUD->m_ImagePlane.at(m_uiHoverTextureIndex).setVisable(false);

		return false;
	}

	return false;
}

void Button::render()
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
