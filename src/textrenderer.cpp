#include "textrenderer.h"

TextRenderer::TextRenderer(const Shader &as, const GLuint &avao, const GLuint &avbo) : s(as), vao(avao), vbo(avbo)
{
	Initialise();
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state
	s.use();
	glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBOs for each character
		GLfloat vertices[6][4] = {
			{xpos, ypos + h, 0.0, 0.0},
			{xpos, ypos, 0.0, 1.0},
			{xpos + w, ypos, 1.0, 1.0},

			{xpos, ypos + h, 0.0, 0.0},
			{xpos + w, ypos, 1.0, 1.0},
			{xpos + w, ypos + h, 1.0, 0.0}};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBOs memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::Initialise()
{
	std::cout << "INITIALIZED::TEXTRENDERER" << std::endl;
	// Text
	// ----
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}
