#pragma once
#include <glm/vec2.hpp>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "Component.h"
#include "../GameObject.h"

class TransformComponent : public Component
{
public:
	glm::vec2 position = glm::vec2(0, 0);
	float rotation = 0;

	TransformComponent() = delete;
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator = (const TransformComponent&) = delete;
	~TransformComponent() = default;

	TransformComponent(GameObject* gameObject);
	void setParametrs(const rapidjson::Value& jsonObject) override;

	glm::vec2& getGlobalPosition();
};