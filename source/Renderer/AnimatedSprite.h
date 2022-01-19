#pragma once
#include <map>
#include <vector>
#include "Sprite.h"

namespace RenderEngine
{
	class AnimatedSprite : public Sprite
	{
	private:
		mutable bool isNewFrame = false;
		size_t currentFrame = 0;
		double currentAnimationTime = 0;

		map<string, vector<pair<string, double>>> states;
		map<string, vector<pair<string, double>>>::const_iterator currentAnimationDurations;

	public:
		AnimatedSprite(const ShaderProgram* shader,
			const Texture* texture,
			const string& initialSubTextureName,
			const vec2& position,
			const vec2& size,
			const vec2& repeatsForWrapMode,
			const float rotation)
			: Sprite(shader, texture, initialSubTextureName, position, size, repeatsForWrapMode, rotation)
		{
			currentAnimationDurations = states.end();
		}

		void insertState(const string& stateName, const vector<pair<string, double>>& subTexturesDuration)
		{
			states.emplace(stateName, subTexturesDuration);
		}

		void setState(string stateName)
		{
			auto it = states.find(stateName);

			if (it == states.end())
			{
				cout << stateName << " state not found" << endl;
				return;
			}

			if (it != currentAnimationDurations)
			{
				currentAnimationTime = currentFrame = 0;
				currentAnimationDurations = it;
				isNewFrame = true;
			}
		}

		void update(const double deltaTime)
		{
			if (currentAnimationDurations != states.end())
			{
				currentAnimationTime += deltaTime;
				while (currentAnimationTime >= currentAnimationDurations->second[currentFrame].second)
				{
					currentAnimationTime -= currentAnimationDurations->second[currentFrame].second;
					++currentFrame;
					isNewFrame = true;

					if (currentFrame == currentAnimationDurations->second.size())
						currentFrame = 0;
				}
			}
		}

		void render() const override
		{
			if (isNewFrame)
			{
				auto subTexture = texture->getSubTexture(currentAnimationDurations->second[currentFrame].first);

				const GLfloat textureCoords[] =
				{
					subTexture.leftDownPoint.x, subTexture.leftDownPoint.y,
					subTexture.leftDownPoint.x, subTexture.rightTopPoint.y,
					subTexture.rightTopPoint.x, subTexture.rightTopPoint.y,
					subTexture.rightTopPoint.x, subTexture.leftDownPoint.y,
				};
				textureCoordsBuffer.update(textureCoords, sizeof(textureCoords));
				isNewFrame = false;
			}

			Sprite::render();
		}
	};
}