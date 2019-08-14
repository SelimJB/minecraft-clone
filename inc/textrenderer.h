#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include "shader.h"
#include <map>

struct Character
{
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;	// Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	long int Advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer(const Shader &s, const GLuint &vao, const GLuint &vbo);
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
	Shader s;
	GLuint vao, vbo;
	std::map<GLchar, Character> Characters;
	void Initialise();
};

#endif
