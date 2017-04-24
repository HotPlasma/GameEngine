#pragma once

#include <iostream>
#include <map>
#include <string>

#include "../libraries/glm/glm/glm.hpp"
#include "../libraries/glm/glm/gtc/matrix_transform.hpp"
#include "../libraries/glm/glm/gtc/type_ptr.hpp"

#include "Bitmap.h"
#include "Texture.h"
#include <ModelReader.h>
#include <Model.h>

#include <glslprogram.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
	GLuint uiTextureID;   ///< ID handle of the glyph texture
	glm::ivec2 Size;    ///< Size of glyph
	glm::ivec2 Bearing;  ///< Offset from baseline to left/top of glyph
	GLuint uiAdvance;    ///< Horizontal offset to advance to next glyph
};

class Freetype 
{
public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor which creates a freetype render
	////////////////////////////////////////////////////////////
	Freetype();

	FT_Library m_Ft; ///< Freetype Library

	FT_Face m_Face; ///< Font

	bool m_bVisable; ///< Toggles visablity of freetype asset

	GLuint m_VAO; ///< VAO for drawing font
	GLuint m_VBO; ///< VBO for drawing font

	std::map<GLchar, Character> m_cCharacters; ///< Map of characters of font

	ModelReader* m_pModelReader;

	vector<Model> m_ImagePlane;

	////////////////////////////////////////////////////////////
	/// \brief Loads the characters from a given font into m_cCharacters
	////////////////////////////////////////////////////////////
	void loadCharacters(); 

	////////////////////////////////////////////////////////////
	/// \brief Sets up buffers for drawing
	////////////////////////////////////////////////////////////
	void setupBuffers();

	////////////////////////////////////////////////////////////
	/// \brief Draws text to to the screen
	///
	/// \param ProgramHandle The freetype shaders program handle
	/// \param text The actual text to be printed to the screen
	/// \param x X coordinate of text
	/// \param y Y coordinate of text
	/// \param y Y coordinate of text
	/// \param scale Scale of text
	/// \param color Color of text
	///
	////////////////////////////////////////////////////////////
	void RenderText(GLuint ProgramHandle , std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void LoadHUDImage(std::string imageLocation, glm::vec3 position, GLfloat rotation, glm::vec3 scale, bool visablity);
	void RenderImage(int index);
};

////////////////////////////////////////////////////////////
/// \class Freetype
///
/// FreeType is used for creating a heads up display by changing
/// a font into images to put onto a transparent plane.
/// 
/// loadCharacters() and setupBuffers() should be run first
/// followed by RenderText which should be given the shaders program
/// handle, the text you wish to be drawn, an x and y position,
/// a scale for the text and a colour for the font.
////////////////////////////////////////////////////////////