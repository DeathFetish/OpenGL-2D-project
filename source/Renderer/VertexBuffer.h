#pragma once
#include <glad/glad.h>

class VertexBuffer
{
private:
	GLuint id;

public:
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator = (const VertexBuffer&) = delete;
	VertexBuffer() : id(0) {}
	~VertexBuffer() { glDeleteBuffers(1, &id); }

	void init(const void* data, const unsigned int size)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void update(const void* data, const unsigned int size) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

	void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};