#include <Freetype.h>

Freetype::Freetype()
{
	// FreeType
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&m_Ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	if (FT_New_Face(m_Ft, "assets/fonts/DEARBORN.ttf", 0, &m_Face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(m_Face, 0, 25);
}

void Freetype::loadCharacters()
{
	// Disable byte-alignment restriction
	gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
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
			m_Face->glyph->bitmap.width,
			m_Face->glyph->bitmap.rows,
			0,
			gl::RED,
			gl::UNSIGNED_BYTE,
			m_Face->glyph->bitmap.buffer
		);
		// Set texture options
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows),
			glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
			m_Face->glyph->advance.x
		};
		m_cCharacters.insert(std::pair<GLchar, Character>(c, character));
	}
	gl::BindTexture(gl::TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(m_Face);
	FT_Done_FreeType(m_Ft);
}

void Freetype::setupBuffers()
{
	gl::GenBuffers(1, &m_VBO);
	// Configure VAO/VBO for texture quads

	gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, gl::DYNAMIC_DRAW);

	gl::GenVertexArrays(1, &m_VAO);
	gl::BindVertexArray(m_VAO);

	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 4, gl::FLOAT, 0, 4 * sizeof(GLfloat), 0);

	gl::BindBuffer(gl::ARRAY_BUFFER, 0);
	gl::BindVertexArray(0);
}

void Freetype::RenderText(GLuint ProgramHandle, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	//shader.use();
	gl::Uniform3f(gl::GetUniformLocation(ProgramHandle, "textColor"), color.x, color.y, color.z);
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindVertexArray(m_VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_cCharacters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
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
		gl::BindTexture(gl::TEXTURE_2D, ch.uiTextureID);
		// Update content of VBO memory
		gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);
		gl::BufferSubData(gl::ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		gl::BindBuffer(gl::ARRAY_BUFFER, 0);
		// Render quad
		gl::DrawArrays(gl::TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.uiAdvance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	gl::BindVertexArray(0);
	gl::BindTexture(gl::TEXTURE_2D, 0);
}