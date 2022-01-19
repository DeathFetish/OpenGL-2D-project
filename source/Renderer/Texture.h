#pragma once
#include <iostream>
#include <string>
#include <map>
#include <glad/glad.h>

namespace RenderEngine
{
	class Texture
	{
	public:

		class SubTexture
		{
		public:
			vec2 leftDownPoint;
			vec2 rightTopPoint;

			SubTexture(vec2 leftDownPoint, vec2 rightTopPoint) : leftDownPoint(leftDownPoint), rightTopPoint(rightTopPoint) {}

			SubTexture() : leftDownPoint(vec2(0.f, 0.f)), rightTopPoint(vec2(1.f, 1.f)) {}
		};

	private:
		GLuint id;
		
		unsigned int textureWidth;
		unsigned int textureHeight;
		unsigned int subTextureHeight;
		unsigned int subtureHeight;

		GLint mode;
		map<string, SubTexture> subTextures;

	public:

		Texture() = delete;
		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;

		Texture(const GLuint width,
			const GLuint height,
			const unsigned char* pixels,
			const unsigned int channels,
			const GLenum filter,
			const GLenum wrapMode)
			: textureWidth(width), textureHeight(height)
		{
			switch (channels)
			{
			case 4: mode = GL_RGBA; break;
			case 3: mode = GL_RGB; break;
			default: mode = GL_RGBA; break;
			}

			glGenTextures(1, &id);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		~Texture() { glDeleteTextures(1, &id); }

		void bind() const { glBindTexture(GL_TEXTURE_2D, id); }

		unsigned int width() const { return textureWidth; }

		unsigned int height() const { return textureHeight; }

		void addSubTexture(const string& name, const vec2& leftDownPoint, const vec2& rightTopPoint)
		{
			subTextures.emplace(name, SubTexture(leftDownPoint, rightTopPoint));
		}

		const SubTexture& getSubTexture(const string& name) const
		{
			auto it = subTextures.find(name);
			const static SubTexture defaultSubTexture;
			if (it == subTextures.end())
			{
				return defaultSubTexture;
			}
			return it->second;
		}
	};
}