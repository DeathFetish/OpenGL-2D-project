#pragma once
#include <glad/glad.h>
#include <vector>

using namespace std;

namespace RenderEngine
{
	struct VertexBufferLayoutElement
	{
		GLint count;
		GLenum type;
		GLboolean normalized;
		unsigned int size;

		VertexBufferLayoutElement(const GLint count, const GLenum type, GLboolean normalized, unsigned int size)
			: count(count), type(type), normalized(normalized), size(size) {}
	};

	// Шаблон буфера, который показывает какие данные в нём хранятся
	class VertexBufferLayout
	{
	private:
		vector<VertexBufferLayoutElement> layoutElements;
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

		const vector<VertexBufferLayoutElement>& getLayoutElements() const { return layoutElements; }
	};
}