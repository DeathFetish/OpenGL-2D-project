#pragma once
#include <glad/glad.h>
#include <vector>

struct VertexBufferLayoutElement
{
	GLint count;
	GLenum type;
	GLboolean normalized;
	unsigned int size;

	VertexBufferLayoutElement(const GLint count, const GLenum type, GLboolean normalized, unsigned int size)
		: count(count), type(type), normalized(normalized), size(size) {}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferLayoutElement> layoutElements;
	unsigned int stride = 0;

public:
	VertexBufferLayout() {}

	void addElementLayoutFloat(const int count, const bool normalized)
	{
		layoutElements.push_back(VertexBufferLayoutElement(count, GL_FLOAT, normalized, count * sizeof(GLfloat)));
		stride += layoutElements.back().size;
	}

	void reserveElements(const size_t count)
	{
		layoutElements.reserve(count);
	}

	unsigned int getStride() const { return stride; }

	const std::vector<VertexBufferLayoutElement>& getLayoutElements() const { return layoutElements; }
};