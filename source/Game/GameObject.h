#pragma once
#include <map>
#include <string>

#include <glm/vec2.hpp>

#include "../Renderer/SpriteAnimator.h"

using std::map;
using std::string;
using glm::vec2;
using RenderEngine::Sprite;
using RenderEngine::SpriteAnimator;

class IDrawable
{
protected:
	vec2 position;
	vec2 size;
	float rotation;
public:
	virtual void render() = 0;

};

class IAnimatable
{

};

class GameObject
{
	Sprite* sprite
};
