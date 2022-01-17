#pragma once
#include <glad/glad.h>

namespace Renderer
{
	class IndexBuffer
	{
	private:
		GLuint id;

	public:
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator = (const IndexBuffer&) = delete;
		IndexBuffer() : id(0) {}
		~IndexBuffer() { glDeleteBuffers(1, &id); }

		void init(const void* data, const unsigned int size)
		{
			glGenBuffers(1, &id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

		void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
	};
}