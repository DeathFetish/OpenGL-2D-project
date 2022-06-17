#pragma once
#include <glad/glad.h>

class IndexBuffer
{
private:
	GLuint id;
	unsigned int count;
public:
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator = (const IndexBuffer&) = delete;
	IndexBuffer() : id(0) {}
	~IndexBuffer() { glDeleteBuffers(1, &id); }

	void init(const void* data, const unsigned int count)
	{
		this->count = count;
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
	}

	void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

	void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	unsigned int getCount() const { return count; }
};
