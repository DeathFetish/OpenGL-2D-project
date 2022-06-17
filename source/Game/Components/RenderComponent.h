#pragma once
#include <glm/vec2.hpp>
#include <functional>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "Component.h"
#include "../../System/Time.h"
#include "../../Resources/ResourceManager.h"

class RenderComponent : public Component
{
public:
	bool renderEnable = true;
	bool animationEnabled = true;
	
	unsigned int layer = 0;

	Sprite* sprite = nullptr;
	ShaderProgram* shader = nullptr;

	std::map<std::string, SpriteAnimation> animations;
	SpriteAnimation* currentAnimation = nullptr;

	unsigned int currentFrame = 0;
	double currentAnimationTime = 0;

public:

	RenderComponent() = delete;
	RenderComponent(const RenderComponent&) = delete;
	RenderComponent& operator = (const RenderComponent&) = delete;

	RenderComponent(GameObject* gameObject);

	void render(const bool isFlipped);
	void update();

	void addAnimation(const std::string name, SpriteAnimation& animation);
	void deleteAnimation(const std::string name);

	void setCurrentAnimation(const std::string& animationName, const unsigned int frameId = 0);

	void setParametrs(const rapidjson::Value& jsonObject) override;
};