#pragma once
#include <iostream>
#include <string>
#include <map>
#include <glad/glad.h>
#include <glm/vec2.hpp>

class Texture
{
public:
	friend class ResourceManager;

	struct SubTexture
	{
		glm::vec2 leftDownPoint;
		glm::vec2 rightTopPoint;
		glm::uvec2 size;

		SubTexture(const glm::vec2& leftDownPoint, const glm::vec2& rightTopPoint, const glm::uvec2& size)
			: leftDownPoint(leftDownPoint), rightTopPoint(rightTopPoint), size(size) {}
	};

private:
	GLuint id;
	GLint mode;
	
	glm::uvec2 size;
	std::map<std::string, SubTexture> subTextures;

	Texture(const GLuint width,
		const GLuint height,
		const unsigned char* pixels,
		const unsigned int channels,
		const GLenum filter,
		const GLenum wrapMode);

	~Texture();

	Texture() = delete;
	Texture(const Texture&) = delete;
	Texture& operator = (const Texture&) = delete;

public:

	void addSubTexture(const std::string& name,
					   const glm::vec2& leftDownPoint,
					   const glm::vec2& rightTopPoint,
					   const glm::vec2& size);

	void bind() const;
	unsigned int width() const;
	unsigned int height() const;
	const SubTexture& getSubTexture(const std::string& name) const;
};