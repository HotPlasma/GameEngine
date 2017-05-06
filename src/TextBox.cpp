#include "TextBox.h"

TextBox::TextBox(const glm::vec2 kPosition)
{
	// Sets members with input
	m_position = kPosition;
}

void TextBox::render(GLSLProgram* pShader, Freetype* pHUD, const glm::vec2 kWindowSize)
{
	// Activates FreeType shader
	pShader->use();
	// Configures projection
	pShader->setUniform("projection", glm::ortho(0.0f, kWindowSize.x, 0.f, kWindowSize.y));
	// Renders placeholder text to HUD
	pHUD->renderText(pShader->getHandle(), m_sText, m_position.x, m_position.y, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}