#pragma once

#include "../../Resources/ResourceManager.h"
#include "../../System/Window.h"
#include "BehaviourComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "../GameObject.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraComponent : public BehaviourComponent
{
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewProjectionMatrix;

	glm::vec3 scale;

	glm::vec2 maxPositionXY;
	glm::vec2 minPositionXY;

	GameObject* objectInFocus;

	void recalculateMatrix()
	{
		glm::vec3 position = glm::vec3(gameObject->getComponent<TransformComponent>()->getGlobalPosition(), 0);

		viewMatrix = glm::translate(glm::mat4(1.0f), position);
		viewMatrix = inverse(viewMatrix);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

public:

	CameraComponent(GameObject* gameObject) : BehaviourComponent(gameObject) {}

	void onPreRender() override
	{
	/*	auto TC = gameObject->getComponent<TransformComponent>();
		glm::vec2 focusPoint = objectInFocus->getComponent<TransformComponent>()->getGlobalPosition();

		TC->position.x = focusPoint.x - Window::size.x * 0.5f / scale.x;
		TC->position.y = focusPoint.y - Window::size.y * 0.5f / scale.y;

	//	float maxOffset = gameHandler.getKeys()[GLFW_KEY_LEFT_SHIFT] == GLFW_PRESS ? 100.f : 7.f;

		TC->position.x = glm::clamp(TC->position.x, minPositionXY.x, maxPositionXY.x);
		TC->position.y = glm::clamp(TC->position.y, minPositionXY.y, maxPositionXY.y);

		glm::vec2 deltaDistance = screenToWorldPoint(gameHandler.getMousePoint()) - (vec2)focusPoint;
		glm::vec2 velocity = glm::normalize(deltaDistance);

		deltaDistance.x = abs(deltaDistance.x) > maxOffset ? maxOffset : abs(deltaDistance.x);
		deltaDistance.y = abs(deltaDistance.y) > maxOffset ? maxOffset : abs(deltaDistance.y);

		TC->position.x += velocity.x * deltaDistance.x;
		TC->position.y += velocity.y * deltaDistance.y;

		position.x -= std::remainderf(position.x, 1.f / 4.f);
		position.y -= std::remainderf(position.x, 1.f / 4.f); */
		recalculateMatrix(); 
		
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