#include "Freetype.h"

Freetype::Freetype()
{
	// FreeType
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&m_ftLib))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	if (FT_New_Face(m_ftLib, "assets/fonts/DEARBORN.ttf", 0, &m_face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(m_face, 0, 25);
}

void Freetype::loadCharacters()
{
	// Disable byte-alignment restriction
	gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		gl::GenTextures(1, &texture);
		gl::BindTexture(gl::TEXTURE_2D, texture);
		gl::TexImage2D(
			gl::TEXTURE_2D,
			0,
			gl::RED,
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows,
			0,
			gl::RED,
			gl::UNSIGNED_BYTE,
			m_face->glyph->bitmap.buffer
		);
		// Set texture options
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
			glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
			(GLuint)m_face->glyph->advance.x
		};
		m_cCharacters.insert(std::pair<GLchar, Character>(c, character));
	}
	gl::BindTexture(gl::TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_ftLib);
}

void Freetype::setupBuffers()
{
	gl::GenBuffers(1, &m_vbo);
	// Configure VAO/VBO for texture quads

	gl::BindBuffer(gl::ARRAY_BUFFER, m_vbo);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, gl::DYNAMIC_DRAW);

	gl::GenVertexArrays(1, &m_vao);
	gl::BindVertexArray(m_vao);

	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 4, gl::FLOAT, 0, 4 * sizeof(GLfloat), 0);

	gl::BindBuffer(gl::ARRAY_BUFFER, 0);
	gl::BindVertexArray(0);
}

void Freetype::addImage(std::string imageLocation, glm::vec3 position, GLfloat rotation, glm::vec3 scale, bool visablity)
{
	m_imagePlane.push_back(Model("assets/Models/HUDPlane.obj", imageLocation, position, glm::vec3(0, 0, rotation), scale, 0, false));

	m_imagePlane.at(m_imagePlane.size() - 1).setVisible(visablity);

	m_imagePlane.at(m_imagePlane.size() - 1).loadModel();

	m_imagePlane.at(m_imagePlane.size() - 1).initModel();
}

void Freetype::renderText(GLuint ProgramHandle, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	//shader.use();
	gl::Uniform3f(gl::GetUniformLocation(ProgramHandle, "textColor"), color.x, color.y, color.z);
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindVertexArray(m_vao);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_cCharacters[*c];

		GLfloat xpos = x + ch.m_bearing.x * scale;
		GLfloat ypos = y - (ch.m_size.y - ch.m_bearing.y) * scale;

		GLfloat w = ch.m_size.x * scale;
		GLfloat h = ch.m_size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		gl::BindTexture(gl::TEXTURE_2D, ch.m_textureID);
		// Update content of VBO memory
		gl::BindBuffer(gl::ARRAY_BUFFER, m_vbo);
		gl::BufferSubData(gl::ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		gl::BindBuffer(gl::ARRAY_BUFFER, 0);
		// Render quad
		gl::DrawArrays(gl::TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.m_advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	gl::BindVertexArray(0);
	gl::BindTexture(gl::TEXTURE_2D, 0);
}

void Freetype::renderImage(GLSLProgram* pShader, int index)
{
	m_imagePlane.at(index).render(pShader, glm::mat4(1.0f));
}