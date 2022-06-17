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
#include "RenderComponent.h"



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

	void recalculateMatrix()
	{
		glm::vec3 position = glm::vec3(gameObject->getComponent<TransformComponent>()->getGlobalPosition(), 0);

		viewMatrix = glm::translate(glm::mat4(1.0f), position);
		viewMatrix = inverse(viewMatrix);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	float maxOffset = 100.f;
	float minOffset = 7.f;

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

	void onPreRender() override
	{
		glm::vec2 focusPoint = objectInFocus->getComponent<TransformComponent>()->getGlobalPosition();
		auto curAnimation = objectInFocus->getComponent<RenderComponent>()->currentAnimation;
		focusPoint += curAnimation->localPosition + curAnimation->rotationPoint;


		auto TC = gameObject->getComponent<TransformComponent>();
		TC->position.x = focusPoint.x - Window::size.x * 0.5f / scale.x;
		TC->position.y = focusPoint.y - Window::size.y * 0.5f / scale.y;

		float offset = InputHandler::getKeys()[GLFW_KEY_LEFT_SHIFT] == GLFW_PRESS ? maxOffset : minOffset;

		TC->position.x = glm::clamp(TC->position.x, minPositionXY.x, maxPositionXY.x);
		TC->position.y = glm::clamp(TC->position.y, minPositionXY.y, maxPositionXY.y);

		glm::vec2 deltaDistance = screenToWorldPoint(InputHandler::getMousePoint()) - (glm::vec2)focusPoint;
		glm::vec2 velocity = glm::normalize(deltaDistance);

		deltaDistance.x = abs(deltaDistance.x) > offset ? offset : abs(deltaDistance.x);
		deltaDistance.y = abs(deltaDistance.y) > offset ? offset : abs(deltaDistance.y);

		TC->position.x += velocity.x * deltaDistance.x;
		TC->position.y += velocity.y * deltaDistance.y;

		TC->position.x -= std::remainderf(TC->position.x, 1.f / 4.f);
		TC->position.y -= std::remainderf(TC->position.y, 1.f / 4.f);

		recalculateMatrix(); 
		
		gameObject->getComponent<RenderComponent>()->shader->setFloat("time", 4.f * sin(Time::getTime()));

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