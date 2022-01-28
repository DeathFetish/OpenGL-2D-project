#pragma once
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using std::string;

namespace RenderEngine
{
	class ShaderProgram
	{
	private:
		bool compiled = false;
		GLuint id = 0;

		bool createShader(const string& source, const GLenum shaderType, GLuint& shaderID)
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
				std::cout << "createShader compile error: " << info << std::endl;
				return false;
			}
			return true;
		}

	public:
		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram operator = (const ShaderProgram&) = delete;

		ShaderProgram(const string& vertexShader, const string& fragmentShader)
		{
			GLuint vertexShaderID;
			if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
			{
				std::cout << "createShader for vertex shader failed" << std::endl;
				return;
			}

			GLuint fragmentShaderID;
			if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
			{
				std::cout << "createShader for fragment shader failed" << std::endl;
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
				std::cout << "ShaderProgram link error: " << info << std::endl;
			}
			else
				compiled = true;
		}

		~ShaderProgram()
		{
			glDeleteProgram(id);
		}

		bool isCompiled() { return compiled; }

		void use() const { glUseProgram(id); }

		void unUse() const { glUseProgram(0); }

		void setInt(const char* name, const GLint value) const
		{
			glUniform1i(glGetUniformLocation(id, name), value);
		}

		void setMatrix(const char* name, const mat4& matrix) const
		{
			glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value_ptr(matrix));
		}
	};
}