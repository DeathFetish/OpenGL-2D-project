#pragma once
#include <glad/glad.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace RenderEngine
{
	class VertexArray
	{
	private:
		GLuint id = 0;
		unsigned int elementsCount = 0;

	public:
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator = (const VertexArray&) = delete;
		VertexArray() { glGenVertexArrays(1, &id); }
		~VertexArray() { glDeleteVertexArrays(1, &id); }

		void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
		{
			bind();
			vertexBuffer.bind();
			const auto& layoutElements = layout.getLayoutElements();
			GLbyte* offset = nullptr;
			for (unsigned int i = 0; i < layoutElements.size(); ++i)
			{
				const auto& currentLayoutElement = layoutElements[i];
				GLuint currentAttribIndex = elementsCount + i;
				glEnableVertexAttribArray(currentAttribIndex);
				glVertexAttribPointer(currentAttribIndex,
					currentLayoutElement.count,
					currentLayoutElement.type,
					currentLayoutElement.normalized,
					layout.getStride(), offset);
				offset += currentLayoutElement.size;
			}
			elementsCount += static_cast<unsigned int>(layoutElements.size());
		}

		void bind() const { glBindVertexArray(id); }

		void unbind() const { glBindVertexArray(0); }
	};
}