#pragma once

#include "PreHeader.h"

#include "GLSLProgram.h"
#include "Freetype.h"

class TextBox
{
private:

	glm::vec2 m_position;
	std::string m_sText = "";

public:

	TextBox() {}
	TextBox(const glm::vec2 kPosition);

	void addLetter(const int kiLetter) { m_sText += kiLetter; }
	void removeLetter() { if (!m_sText.empty()) { m_sText.pop_back(); } }

	void setStr(const std::string ksString) { m_sText = ksString; }
	std::string getStr() { return m_sText; }

	void render(GLSLProgram* pShader, Freetype* pHUD, const glm::vec2 kWindowSize);
};