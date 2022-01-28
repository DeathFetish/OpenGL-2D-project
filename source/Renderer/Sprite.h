#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Renderer.h"


namespace RenderEngine
{
	class Sprite
	{
	public:
		struct FrameDescription 
		{
			glm::vec2 leftBottomPoint;
			glm::vec2 rightTopPoint;
			glm::vec2 offset;
			glm::uvec2 size;
			double duration;
			
			FrameDescription(
				const glm::vec2& leftBottomPoint,
				const glm::vec2& rightTopPoint,
				const glm::vec2& offset,
				const glm::uvec2& size,
				const double duration)
				: leftBottomPoint(leftBottomPoint), rightTopPoint(rightTopPoint),
				size(size), offset(offset),
				duration(duration) {}
		};
	protected:
		const ShaderProgram* shader;
		const Texture* texture;

		vector<FrameDescription> framesDescriptions;

		mutable unsigned int prevFrameId;
		mutable glm::vec2 size;
		mutable glm::vec2 realPosition;

		VertexArray vertexArray;
		VertexBuffer vertexCoordsBuffer;
		VertexBuffer textureCoordsBuffer;
		IndexBuffer indexBuffer;

	public:
		Sprite(const Sprite&) = delete;
		Sprite& operator = (const Sprite&) = delete;

		Sprite(const ShaderProgram* shader,
			const Texture* texture,
			const std::string& initialSubTextureName)
			: shader(shader), texture(texture), prevFrameId(0)
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

			/*const GLfloat textureCoords[] =
			{
				subTexture.leftDownPoint.x * repeatsForWrapMode.x, subTexture.leftDownPoint.y * repeatsForWrapMode.y,
				subTexture.leftDownPoint.x * repeatsForWrapMode.x, subTexture.rightTopPoint.y * repeatsForWrapMode.y,
				subTexture.rightTopPoint.x * repeatsForWrapMode.x, subTexture.rightTopPoint.y * repeatsForWrapMode.y,
				subTexture.rightTopPoint.x * repeatsForWrapMode.x, subTexture.leftDownPoint.y * repeatsForWrapMode.y
			};*/

			const GLfloat textureCoords[] =
			{
				subTexture.leftDownPoint.x, subTexture.leftDownPoint.y,
				subTexture.leftDownPoint.x, subTexture.rightTopPoint.y,
				subTexture.rightTopPoint.x, subTexture.rightTopPoint.y,
				subTexture.rightTopPoint.x, subTexture.leftDownPoint.y
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

		virtual void render(const glm::vec2& position, const float rotation, const unsigned int frameId) const
		{
			if (prevFrameId != frameId)
			{
				prevFrameId = frameId;

				const GLfloat textureCoords[] = 
				{
					framesDescriptions[frameId].leftBottomPoint.x, framesDescriptions[frameId].leftBottomPoint.y,
					framesDescriptions[frameId].leftBottomPoint.x, framesDescriptions[frameId].rightTopPoint.y,
					framesDescriptions[frameId].rightTopPoint.x,   framesDescriptions[frameId].rightTopPoint.y,
					framesDescriptions[frameId].rightTopPoint.x,   framesDescriptions[frameId].leftBottomPoint.y,
				};

				textureCoordsBuffer.update(textureCoords, 8 * sizeof(GLfloat));
			}
		
			shader->use();
			
			glm::mat4 model(1.f);
			
			realPosition.x = position.x; //+ framesDescriptions[frameId].offset.x;
			realPosition.y = position.y; //+ framesDescriptions[frameId].offset.y;
			size.x = framesDescriptions[frameId].size.x;
			size.y = framesDescriptions[frameId].size.y;

			/*std::cout << "_____________" << std::endl;
			std::cout << framesDescriptions[frameId].offset.y << std::endl;
			std::cout << "position: " << position.x << "    " << position.y << std::endl;
			std::cout << "RealPosition: " << realPosition.x << "    " << realPosition.y << std::endl;*/

			model = glm::translate(model, vec3(realPosition, 0.f));
	//		model = glm::translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.f));
	//		model = glm::rotate(model, glm::radians(rotation), vec3(0.f, 0.f, 1.f));
	//		model = glm::translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
			model = glm::scale(model, vec3(size.x, size.y, 1.f));

			vertexArray.bind();
			shader->setMatrix("model_mat", model);

			glActiveTexture(GL_TEXTURE0);
			texture->bind();

			renderer.draw(vertexArray, indexBuffer, *shader);
		}

		void insertFrames(const vector<FrameDescription>& framesDescriptions)
		{
			this->framesDescriptions = framesDescriptions;
		}

		unsigned int Sprite::getFramesCount() const { return framesDescriptions.size(); }

		double getFrameDuration(const unsigned int frameId) const { return framesDescriptions[frameId].duration; }
	};
}