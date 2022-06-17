#include "TransformComponent.h"

TransformComponent::TransformComponent(GameObject* gameObject) 
	: Component(gameObject) {}

void TransformComponent::setParametrs(const rapidjson::Value& jsonObject)
{
	position = glm::vec2(jsonObject["x"].GetFloat(), jsonObject["y"].GetFloat());
	rotation = jsonObject.HasMember("rotation") ? jsonObject["rotation"].GetFloat() : 0;
}

glm::vec2& TransformComponent::getGlobalPosition()
{
	glm::vec2 result(0, 0);

	auto object = gameObject;
	do
	{
		auto TC = object->getComponent<TransformComponent>();
		result += TC->position;
	} while (object = object->getParent());

	return result;
}