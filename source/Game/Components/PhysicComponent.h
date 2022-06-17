#pragma once
#include <glm/vec2.hpp>
#include <functional>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "Component.h"

class IShape;
class GameObject;

class PhysicComponent : public Component
{
public:
	glm::vec2 localPosition = glm::vec2(0, 0);
	glm::vec2 heightArea = glm::vec2(0, 0);
	IShape* hitbox = nullptr;

	PhysicComponent() = delete;
	PhysicComponent(const PhysicComponent&) = delete;
	PhysicComponent& operator = (const PhysicComponent&) = delete;

	PhysicComponent(GameObject* gameObject);
	~PhysicComponent() { delete hitbox; }

	void recalculateHitbox(const glm::vec2& position, const float rotation);
	void setParametrs(const rapidjson::Value& jsonObject) override;
};