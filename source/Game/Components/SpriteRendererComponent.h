#pragma once
#include <glm/vec2.hpp>
#include <functional>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "RenderComponent.h"
#include "../../System/Time.h"
#include "../../Resources/ResourceManager.h"

class SpriteRendererComponent : public RenderComponent
{
public:
	bool renderEnable = true;
	bool animationEnabled = true;


	Sprite* sprite = nullptr;
	ShaderProgram* shader = nullptr;

	std::map<std::string, SpriteAnimation> animations;
	SpriteAnimation* currentAnimation = nullptr;

	unsigned int currentFrame = 0;
	double currentAnimationTime = 0;

public:

	SpriteRendererComponent() = delete;
	SpriteRendererComponent(const SpriteRendererComponent&) = delete;
	SpriteRendererComponent& operator = (const SpriteRendererComponent&) = delete;

	SpriteRendererComponent(GameObject* gameObject) : RenderComponent(gameObject) {};

	void render() override;
	void update() override;

	void addAnimation(const std::string name, SpriteAnimation& animation);
	void deleteAnimation(const std::string name);
	void setCurrentAnimation(const std::string& animationName, const unsigned int frameId = 0);

	void setParametrs(const rapidjson::Value& jsonObject) override;
};