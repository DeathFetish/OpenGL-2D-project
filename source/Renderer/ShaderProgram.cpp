#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint vertexShaderID;

	if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
	{
		std::cout << "Error! createing vertex shader failed." << std::endl;
		return;
	}

	GLuint fragmentShaderID;
	if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
	{
		std::cout << "Error! createing fragment shader failed." << std::endl;
		glDeleteShader(vertexShaderID);
		return;
	}

	id = glCreateProgram();
	glAttachShader(id, vertexShaderID);
	glAttachShader(id, fragmentShaderID);
	glLinkProgram(id);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	GLint success;

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLchar info[1024];
		glGetShaderInfoLog(id, 1024, nullptr, info);
		std::cout << "Error! Shader is not linked: " << info << std::endl;
	}
	else
		compiled = true;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(id);
}

bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
{
	shaderID = glCreateShader(shaderType);
	const char* code = source.c_str();
	glShaderSource(shaderID, 1, &code, nullptr);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLchar info[1024];
		glGetShaderInfoLog(shaderID, 1024, nullptr, info);
		std::cout << "Error! Shader is not compiled: " << info << std::endl;
		return false;
	}
	return true;
}

void ShaderProgram::use() const { glUseProgram(id); }

void ShaderProgram::unUse() const { glUseProgram(0); }

bool ShaderProgram::isCompiled() { return compiled; }

void ShaderProgram::setInt(const std::string& name, const GLint value) const
{
	use();
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	unUse();
}

void ShaderProgram::setFloat(const std::string& name, const GLfloat value) const
{
	use();
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	unUse();
}

void ShaderProgram::setDouble(const std::string& name, const GLfloat value) const
{
	use();
	glUniform1d(glGetUniformLocation(id, name.c_str()), value);
	unUse();
}

void ShaderProgram::setMatrix(const std::string& name, const glm::mat4& matrix) const
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, value_ptr(matrix));
	unUse();
}
