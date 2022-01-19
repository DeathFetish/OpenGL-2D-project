#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace RenderEngine
{
	class Sprite
	{
	protected:
		const ShaderProgram* shader;
		const Texture* texture;
		vec2 position;
		vec2 size;
		float rotation;

		VertexArray vertexArray;
		VertexBuffer vertexCoordsBuffer;
		VertexBuffer textureCoordsBuffer;
		IndexBuffer indexBuffer;

	public:
		Sprite(const Sprite&) = delete;
		Sprite& operator = (const Sprite&) = delete;

		Sprite(const ShaderProgram* shader,
			const Texture* texture,
			const string& initialSubTextureName,
			const vec2& position,
			const vec2& size,
			const vec2& repeatsForWrapMode,
			const float rotation)
			: shader(shader), texture(texture), position(position), rotation(rotation),
			size(size.x* repeatsForWrapMode.x * 4, size.y* repeatsForWrapMode.y * 4)
		{
			//	1-----2
			//  |	  |
			//  |	  |
			//  0-----3
			const GLuint indices[] =
			{
				0, 1, 2,
				2, 3, 0
			};

			const GLfloat vertexCoords[] =
			{
				0.0, 0.0,
				0.0, 1.0,
				1.0, 1.0,
				1.0, 0.0
			};

			auto subTexture = texture->getSubTexture(initialSubTextureName);

			const GLfloat textureCoords[] =
			{
				subTexture.leftDownPoint.x * repeatsForWrapMode.x, subTexture.leftDownPoint.y * repeatsForWrapMode.y,
				subTexture.leftDownPoint.x * repeatsForWrapMode.x, subTexture.rightTopPoint.y * repeatsForWrapMode.y,
				subTexture.rightTopPoint.x * repeatsForWrapMode.x, subTexture.rightTopPoint.y * repeatsForWrapMode.y,
				subTexture.rightTopPoint.x * repeatsForWrapMode.x, subTexture.leftDownPoint.y * repeatsForWrapMode.y
			};

			vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
			VertexBufferLayout vertexCoordsLayout;
			vertexCoordsLayout.addElementLayoutFloat(2, false);
			vertexArray.addBuffer(vertexCoordsBuffer, vertexCoordsLayout);

			textureCoordsBuffer.init(textureCoords, sizeof(textureCoords));
			VertexBufferLayout textureCoordsLayout;
			textureCoordsLayout.addElementLayoutFloat(2, false);
			vertexArray.addBuffer(textureCoordsBuffer, textureCoordsLayout);

			indexBuffer.init(indices, 6);

			vertexArray.unbind();
			indexBuffer.unbind();
		}

		~Sprite() {}

		virtual void render() const
		{
			shader->use();

			mat4 model(1.f);

			model = translate(model, vec3(position, 0.f));
			model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.f));
			model = rotate(model, radians(rotation), vec3(0.f, 0.f, 1.f));
			model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
			model = scale(model, vec3(size, 1.f));

			vertexArray.bind();
			shader->setMatrix("model_mat", model);

			glActiveTexture(GL_TEXTURE0);
			texture->bind();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			vertexArray.unbind();
		}

		virtual void render(vec2 repeatsForWrapMode) const
		{
			shader->use();

			auto subTexture = texture->getSubTexture("");

			const GLfloat textureCoords[] =
			{
				subTexture.leftDownPoint.x * repeatsForWrapMode.x, subTexture.leftDownPoint.y * repeatsForWrapMode.y,
				subTexture.leftDownPoint.x * repeatsForWrapMode.x, subTexture.rightTopPoint.y * repeatsForWrapMode.y,
				subTexture.rightTopPoint.x * repeatsForWrapMode.x, subTexture.rightTopPoint.y * repeatsForWrapMode.y,
				subTexture.rightTopPoint.x * repeatsForWrapMode.x, subTexture.leftDownPoint.y * repeatsForWrapMode.y
			};

			textureCoordsBuffer.update(textureCoords, sizeof(textureCoords));

			mat4 model(1.f);

			model = translate(model, vec3(position, 0.f));
			model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.f));
			model = rotate(model, radians(rotation), vec3(0.f, 0.f, 1.f));
			model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
			model = scale(model, vec3(size, 1.f));

			vertexArray.bind();
			shader->setMatrix("model_mat", model);

			glActiveTexture(GL_TEXTURE0);
			texture->bind();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			vertexArray.unbind();
		}

		vec2 getSize() { return size; }

		void setSize(const vec2& size) { this->size = size; }

		void setPosition(const vec2& position) { this->position = position; }

		void setRotation(const float rotation) { this->rotation = rotation; }
	};
}