#include "Button.h"

Button::Button(const glm::vec2 kPosition, const string ksLoc, const string ksHoverLoc, const vec3 kScale, Freetype *pOverlay)
{
	m_pHUD = pOverlay;

	m_uiTextureIndex = (unsigned int)m_pHUD->m_imagePlane.size();
	m_pHUD->LoadHUDImage(ksLoc, vec3(kPosition, 1), 0, kScale, false);

	m_uiHoverTextureIndex = (unsigned int)m_pHUD->m_imagePlane.size();
	m_pHUD->LoadHUDImage(ksHoverLoc, vec3(kPosition, 1), 0, kScale, false);

	m_position = kPosition;
	m_scale = vec2(kScale);
}

bool Button::mouseOver(sf::Vector2f mousePos, const float kfWindowYSize)
{
	mousePos.y = kfWindowYSize - mousePos.y; // Origins mouse at top-left instead of bottom left.
	
	// Mouse is over over Button
	if (mousePos.x > (m_position.x - (m_scale.x / 2)) && mousePos.x < (m_position.x - (m_scale.x / 2)) + m_scale.x  &&
		mousePos.y > (m_position.y - (m_scale.y / 2)) && mousePos.y < (m_position.y - (m_scale.y / 2)) + m_scale.y)
	{
		// Sets standard texture as invisible and hovertex as visible
		m_pHUD->m_imagePlane.at(m_uiTextureIndex).setVisible(false);
		m_pHUD->m_imagePlane.at(m_uiHoverTextureIndex).setVisible(true);

		return true;
	}
	// Mouse is not over Button
	else
	{
		// Sets standard hovertex as invisible and texture as visible
		m_pHUD->m_imagePlane.at(m_uiTextureIndex).setVisible(true);
		m_pHUD->m_imagePlane.at(m_uiHoverTextureIndex).setVisible(false);

		return false;
	}

	return false;
}

void Button::render(GLSLProgram* pShader, const sf::Vector2i kWindowSize)
{
	pShader->use();

	// If default texture visible
	if (m_pHUD->m_imagePlane.at(m_uiTextureIndex).isVisible())
	{
		pShader->setUniform("M", m_pHUD->m_imagePlane.at(m_uiTextureIndex).getM());
		pShader->setUniform("P", glm::ortho(0.0f, (float)kWindowSize.x, 0.f, (float)kWindowSize.y));
		m_pHUD->RenderImage(pShader, m_uiTextureIndex);
	}

	// If hover texture visible
	if (m_pHUD->m_imagePlane.at(m_uiHoverTextureIndex).isVisible())
	{
		pShader->setUniform("M", m_pHUD->m_imagePlane.at(m_uiHoverTextureIndex).getM());
		pShader->setUniform("P", glm::ortho(0.0f, (float)kWindowSize.x, 0.f, (float)kWindowSize.y));
		m_pHUD->RenderImage(pShader, m_uiHoverTextureIndex);
	}
}