#include "PhysicComponent.h"

#include "../../Physic/IShape.h"
#include "../../Physic/Circle.h"
#include "../../Physic/Rectangle.h"

#include "TransformComponent.h"
#include "../GameObject.h"


PhysicComponent::PhysicComponent(GameObject* gameObject) : Component(gameObject) {}

void PhysicComponent::recalculateHitbox(const glm::vec2& position, const float rotation)
{
	hitbox->recalculateIShape(position + localPosition, rotation);
}

void PhysicComponent::setParametrs(const rapidjson::Value& jsonObject)
{
	localPosition = jsonObject.HasMember("localX") ?
		glm::vec2(jsonObject["localX"].GetFloat(), jsonObject["localY"].GetFloat()) : glm::vec2(0, 0);

	heightArea = glm::vec2(jsonObject["heightAreaMin"].GetFloat(), jsonObject["heightAreaMax"].GetFloat());

	if (jsonObject.HasMember("radius"))
	{
		hitbox = new Circle(localPosition, jsonObject["radius"].GetFloat());
	}
	else
	{
		glm::vec2 size = glm::vec2(jsonObject["w"].GetFloat(), jsonObject["h"].GetFloat());

		std::vector<glm::vec2> points =
		{
			glm::vec2(0, 0),
			glm::vec2(0, size.y),
			glm::vec2(size.x, size.y),
			glm::vec2(size.x, 0)
		};

		glm::vec2 rotationPoint = jsonObject.HasMember("rotationPointX") ?
			glm::vec2(jsonObject["rotationPointX"].GetFloat(), jsonObject["rotationPointY"].GetFloat()) : glm::vec2(size.x / 2, size.y / 2);

		hitbox = new Rectangle(points, rotationPoint);
	}
}