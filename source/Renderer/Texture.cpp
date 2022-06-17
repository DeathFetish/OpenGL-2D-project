#include "Texture.h"

Texture::Texture(const GLuint width,
				 const GLuint height,
				 const unsigned char* pixels,
				 const unsigned int channels,
				 const GLenum filter,
				 const GLenum wrapMode) : size(glm::uvec2(width, height))
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

Texture::~Texture() { glDeleteTextures(1, &id); }

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, id); }

unsigned int Texture::width() const { return size.x; }

unsigned int Texture::height() const { return size.y; }

void Texture::addSubTexture(const std::string& name,
							const glm::vec2& leftDownPoint,
							const glm::vec2& rightTopPoint,
							const glm::vec2& size)
{
	subTextures.emplace(name, SubTexture(leftDownPoint, rightTopPoint, size));
}

const Texture::SubTexture& Texture::getSubTexture(const std::string& name) const
{
	auto it = subTextures.find(name);
	if (it == subTextures.end())
	{
		std::cout << "Error! SubTexture not found. SubTexture name: " << name << "." << std::endl;
		exit(-1);
	}
	return it->second;
}
