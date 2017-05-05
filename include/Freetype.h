#pragma once
#ifndef FREETYPE_H
#define FREETYPE_H

#include "ModelReader.h"
#include "Model.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
	GLuint m_textureID;		//!< ID handle of the glyph texture
	glm::ivec2 m_size;		//!< Size of glyph
	glm::ivec2 m_bearing;	//!< Offset from baseline to left/top of glyph
	GLuint m_advance;		//!< Horizontal offset to advance to next glyph
};

class Freetype 
{
public:

	Freetype();

	FT_Library m_ftLib; //!< Freetype Library

	FT_Face m_face; //!< Font

	bool m_bVisible; //!< Toggles visablity of freetype asset

	GLuint m_vao; //!< VAO for drawing font
	GLuint m_vbo; //!< VBO for drawing font

	std::map<GLchar, Character> m_cCharacters; //!< Map of characters of font

	ModelReader* m_pModelReader;

	std::vector<Model> m_imagePlane;

	void loadCharacters();
	void setupBuffers();
	void RenderText(GLuint ProgramHandle , std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void LoadHUDImage(std::string imageLocation, glm::vec3 position, GLfloat rotation, glm::vec3 scale, bool visablity);
	void RenderImage(GLSLProgram* pShader, int index);
};

#endif