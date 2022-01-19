#pragma once
#include <iostream>
#include <glad/glad.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"

#define renderer RenderEngine::Renderer::get()

namespace RenderEngine
{
	class Renderer final
	{
	private:
		Renderer() = default;
		~Renderer() = default;

	public:
		
		Renderer(const Renderer&) = delete;
		Renderer& operator = (const Renderer&) = delete;

		static Renderer& get()
		{
			static Renderer object;
			return object;
		}

		void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader)
		{
			shader.use();
			vertexArray.bind();
			indexBuffer.bind();

			glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer::setClearColor(const float r, const float g, const float b, const float a)
		{
			glClearColor(r, g, b, a);
		}

		void Renderer::setDepthTest(const bool enable)
		{
			if (enable)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
		}

		void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	};
}