#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
	// program ID
	unsigned int Program;
	// reads and builds the shader
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
	// use/activate the shader
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif
