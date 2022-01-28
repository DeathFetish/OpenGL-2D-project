#pragma once
#include <map>
#include <vector>

#include "Sprite.h"

using namespace glm;

using std::map;
using std::pair;
using std::vector;
using std::string;

namespace RenderEngine
{
	class SpriteAnimator 
	{
	private:
		Sprite* sprite;
		
		double currentFrameDuration;
		unsigned int currentFrame = 0;
		double currentAnimationTime = 0;
		double totalDuration = 0;

	public:
		SpriteAnimator(Sprite* sprite)
			: sprite(sprite)
			, currentFrameDuration(sprite->getFrameDuration(0))
		{
			for (unsigned int currentFrameId = 0; currentFrameId < sprite->getFramesCount(); ++currentFrameId)
				totalDuration += sprite->getFrameDuration(currentFrameId);
		}
		
		void update(const double delta)
		{
			currentAnimationTime += delta;
			while (currentAnimationTime >= currentFrameDuration)
			{
				currentAnimationTime -= currentFrameDuration;
				++currentFrame;
				
				if (currentFrame == sprite->getFramesCount())
					currentFrame = 0;
				
				currentFrameDuration = sprite->getFrameDuration(currentFrame);
			}
		}
		
		void reset()
		{
			currentFrame = 0;
			currentFrameDuration = sprite->getFrameDuration(0);
			currentAnimationTime = 0;
		}

		unsigned int getCurrentFrame() const { return currentFrame; }
		
		double getTotalDuration() const { return totalDuration; }

		void setSprite(Sprite* sprite)
		{
			this->sprite = sprite;
			currentFrame = 0;
			currentFrameDuration = sprite->getFrameDuration(0);
			currentAnimationTime = 0;
			totalDuration = 0;

			for (unsigned int currentFrameId = 0; currentFrameId < sprite->getFramesCount(); ++currentFrameId)
				totalDuration += sprite->getFrameDuration(currentFrameId);
		}
	};
}