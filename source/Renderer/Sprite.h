#pragma once
#include <string>
#include <vector>
#include <map>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

struct FrameDescription
{
	glm::vec2 leftDownPoint;
	glm::vec2 rightTopPoint;
	double duration;

	FrameDescription(const glm::vec2& leftDownPoint, const glm::vec2& rightTopPoint, const double duration)
		: leftDownPoint(leftDownPoint), rightTopPoint(rightTopPoint), duration(duration) {}

	FrameDescription(const FrameDescription&) = default;
	FrameDescription& operator = (const FrameDescription&) = default;
};

struct SpriteAnimation
{
private:
	double calculateDuration(const std::vector<FrameDescription>& frames)
	{
		double time = 0;
		for (const auto& it : frames)
			time += it.duration;
		return time;
	}

public:
	const double duration;
	
	const glm::vec2 size;
	const glm::vec2 rotationPoint;
	const glm::vec2 localPosition;

	const std::vector<FrameDescription> frames;
	
	SpriteAnimation() : size(glm::vec2(0, 0)), rotationPoint(glm::vec2(0, 0)), localPosition(glm::vec2(0, 0)), duration(0) {}

	SpriteAnimation(const glm::vec2& localPosition,
					const glm::vec2& rotationPoint,
					const glm::vec2& size,
					const std::vector<FrameDescription>& frames)
		: localPosition(localPosition), rotationPoint(rotationPoint), size(size), frames(frames), duration(calculateDuration(frames)) {}
	~SpriteAnimation() {}

	SpriteAnimation(const SpriteAnimation&) = default;
	SpriteAnimation& operator = (const SpriteAnimation& animation) = delete;
};

class Sprite
{
private:
	friend class ResourceManager;

	const Texture* texture;

	VertexArray vertexArray;
	VertexBuffer vertexCoordsBuffer;
	VertexBuffer textureCoordsBuffer;
	IndexBuffer indexBuffer;

	mutable FrameDescription* prevFrameDescription = nullptr;

	Sprite(const Texture* texture) : texture(texture)
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

		const GLfloat textureCoords[] =
		{
			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
			1.0, 0.0
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

public:
	Sprite() = delete;
	Sprite(Sprite&) = delete;
	Sprite operator = (const Sprite&) = delete;

	void render(const ShaderProgram* shader,
				const glm::vec2& position,
				const glm::vec2& size,
				const glm::vec2& rotationPoint,
				const float rotation,
				const float layer,
				const FrameDescription* frameDescription,
				const bool isFlipped = false) const
	{
		if (prevFrameDescription != frameDescription)
		{
			prevFrameDescription = const_cast<FrameDescription*>(frameDescription);

			const GLfloat textureCoords[] =
			{
				frameDescription->leftDownPoint.x, frameDescription->leftDownPoint.y,
				frameDescription->leftDownPoint.x, frameDescription->rightTopPoint.y,
				frameDescription->rightTopPoint.x, frameDescription->rightTopPoint.y,
				frameDescription->rightTopPoint.x, frameDescription->leftDownPoint.y,
			};

			textureCoordsBuffer.update(textureCoords, 8 * sizeof(GLfloat));
		}


		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(position.x, position.y, 0.f));
		model = glm::translate(model, glm::vec3(rotationPoint.x, rotationPoint.y, 0.f));

		if (isFlipped)
			model = glm::scale(model, glm::vec3(-1, 1, 1.f));

		model = glm::rotate(model, glm::radians(rotation) * (isFlipped ? -1 : 1), glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model, glm::vec3(-1.f * rotationPoint.x, -1.f * rotationPoint.y, 0.f));
		model = glm::scale(model, glm::vec3(size.x, size.y, 1.f));

		vertexArray.bind();

		shader->setMatrix("modelMat", model);
		shader->setFloat("layer", layer);

		glActiveTexture(GL_TEXTURE0);
		texture->bind();

		shader->use();
		vertexArray.bind();
		indexBuffer.bind();

		glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
		shader->unUse();
	}

	const Texture* getTexture() const { return texture; }
};