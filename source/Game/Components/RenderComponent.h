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
	unsigned int layer = 0;

	RenderComponent() = delete;
	RenderComponent(const RenderComponent&) = delete;
	RenderComponent& operator = (const RenderComponent&) = delete;

	RenderComponent(GameObject* gameObject);
	~RenderComponent();

	virtual void render() = 0;
	virtual void update() {};

	void setParametrs(const rapidjson::Value& jsonObject) override {}
};