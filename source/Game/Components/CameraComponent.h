#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Resources/ResourceManager.h"
#include "../../System/InputHandler.h"
#include "../../System/Window.h"
#include "../../System/Time.h"
#include "../GameObject.h"

#include "BehaviourComponent.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"



class CameraComponent : public BehaviourComponent
{
private:
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);

	glm::vec3 scale = glm::vec3(0, 0, 0);

	glm::vec2 maxPositionXY = glm::vec2(0, 0);
	glm::vec2 minPositionXY = glm::vec2(0, 0);

	GameObject* objectInFocus = nullptr;

	float maxOffset = 100.f;
	float minOffset = 7.f;
	float speed = 0.7f;

	void recalculateMatrix()
	{
		glm::vec3 position = glm::vec3(gameObject->getComponent<TransformComponent>()->getGlobalPosition(), 0);

		viewMatrix = glm::translate(glm::mat4(1.0f), position);
		viewMatrix = inverse(viewMatrix);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

public:

	CameraComponent(GameObject* gameObject) : BehaviourComponent(gameObject) {}

	void init() override
	{
		objectInFocus = ObjectsManager::findByTag("hero");
	}

	void setParametrs(const rapidjson::Value& jsonObject) override
	{
		scale = glm::vec3(jsonObject["scaleX"].GetFloat(), jsonObject["scaleY"].GetFloat(), 0);
		maxPositionXY = glm::vec2(jsonObject["maxX"].GetFloat(), jsonObject["maxY"].GetFloat());
		minPositionXY = glm::vec2(jsonObject["minX"].GetFloat(), jsonObject["minY"].GetFloat());

		projectionMatrix = glm::ortho(0.f, Window::size.x / scale.x, 0.f, Window::size.y / scale.y, -100.f, 100.f);
		recalculateMatrix();
	}

	void printVec2(std::string name, glm::vec2 vec)
	{
		std::cout << name << ": " << vec.x << "\t" << vec.y << std::endl;
	}

	void onPreRender() override
	{
		auto TC = gameObject->getComponent<TransformComponent>();

		glm::vec2 focusPoint = objectInFocus->getComponent<TransformComponent>()->getGlobalPosition();
		auto curAnimation = objectInFocus->getComponent<SpriteRendererComponent>()->currentAnimation;
		focusPoint += curAnimation->localPosition + curAnimation->rotationPoint;

		glm::vec2 destinationPosition;
		destinationPosition.x = focusPoint.x - Window::size.x * 0.5f / scale.x;
		destinationPosition.y = focusPoint.y - Window::size.y * 0.5f / scale.y;
		destinationPosition.x = glm::clamp(destinationPosition.x, minPositionXY.x, maxPositionXY.x);
		destinationPosition.y = glm::clamp(destinationPosition.y, minPositionXY.y, maxPositionXY.y);

		float offset = InputHandler::getKeys()[GLFW_KEY_LEFT_SHIFT] == GLFW_PRESS ? maxOffset : minOffset;

		glm::vec2 oldPosition = TC->position;
		TC->position = destinationPosition;
		glm::vec2 focusToCursorDist = screenToWorldPoint(InputHandler::getMousePoint()) - (glm::vec2)focusPoint;
		TC->position = oldPosition;

		glm::vec2 velocity = glm::normalize(focusToCursorDist);

		focusToCursorDist.x = abs(focusToCursorDist.x) > offset ? offset : abs(focusToCursorDist.x);
		focusToCursorDist.y = abs(focusToCursorDist.y) > offset ? offset : abs(focusToCursorDist.y);

		destinationPosition.x += velocity.x * focusToCursorDist.x;
		destinationPosition.y += velocity.y * focusToCursorDist.y;

		double deltaTime = Time::getDeltaTime();
		glm::vec2 deltaDistance = destinationPosition - TC->position;
		
		velocity = glm::normalize(deltaDistance);
		if (isnan(velocity.x)) velocity.x = 0;
		if (isnan(velocity.y)) velocity.y = 0;

		glm::vec2 curOffset = glm::vec2(velocity.x * speed * deltaTime, velocity.y * speed * deltaTime);
		
		TC->position.x = abs(curOffset.x) > abs(deltaDistance.x) ? destinationPosition.x : TC->position.x + curOffset.x;
		TC->position.y = abs(curOffset.y) > abs(deltaDistance.y) ? destinationPosition.y : TC->position.y + curOffset.y;

		recalculateMatrix();

		gameObject->getComponent<SpriteRendererComponent>()->shader->setFloat("time", 4.f * sin(Time::getTime()));

		ResourceManager::getShader("spriteShader")->setMatrix("viewProjectionMat", viewProjectionMatrix);
		ResourceManager::getShader("shadowShader")->setMatrix("viewProjectionMat", viewProjectionMatrix);
		ResourceManager::getShader("filmGrainShader")->setMatrix("viewProjectionMat", viewProjectionMatrix);
	}

	void setObjectInFocus(GameObject* gameObject) { objectInFocus = gameObject; }

	glm::vec2 screenToWorldPoint(const glm::vec2& point) const
	{
		glm::vec2 position = gameObject->getComponent<TransformComponent>()->getGlobalPosition();

		return glm::vec2(position.x + point.x / scale.x, position.y + point.y / scale.y);
	}
};