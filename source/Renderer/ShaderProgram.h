#pragma once
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
private:
	friend class ResourceManager;

	bool compiled = false;
	GLuint id = 0;

	bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);

	ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	~ShaderProgram();

public:
	ShaderProgram() = delete;
	ShaderProgram(ShaderProgram&) = delete;
	ShaderProgram operator = (const ShaderProgram&) = delete;

	bool isCompiled();

	void use() const;
	void unUse() const;

	void setInt(const std::string& name, const GLint value) const;
	void setFloat(const std::string& name, const GLfloat value) const;
	void setDouble(const std::string& name, const GLfloat value) const;
	void setMatrix(const std::string& name, const glm::mat4& matrix) const;
};
