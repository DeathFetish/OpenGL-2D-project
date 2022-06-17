#pragma once
#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include "BehaviourComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RenderComponent.h"
#include "../GameObject.h"
#include "../../System/Time.h"
#include "../../System/InputHandler.h"

class HeroComponent : public BehaviourComponent
{
public:
	glm::vec2 velocity = glm::vec2(0, 0);
	float speed = 0.1f;

	HeroComponent(GameObject* gameObject) : BehaviourComponent(gameObject) {}

	void update() override
	{
		const double deltaTime = Time::getDeltaTime();
		auto TC = gameObject->getComponent<TransformComponent>();
		auto RC = gameObject->getComponent<RenderComponent>();

		velocity.x = 0;
		velocity.y = 0;

		const auto& keys = InputHandler::getKeys();
		const auto& mouseButtons = InputHandler::getMouseButtons();

		if (keys[GLFW_KEY_A] == GLFW_PRESS)
			velocity.x -= 1;
		if (keys[GLFW_KEY_D] == GLFW_PRESS)
			velocity.x += 1;
		if (keys[GLFW_KEY_S] == GLFW_PRESS)
			velocity.y -= 1;
		if (keys[GLFW_KEY_W] == GLFW_PRESS)
			velocity.y += 1;

		if (velocity.x != 0 && velocity.y != 0)
			velocity = glm::normalize(velocity);

		RC->animationEnabled = (velocity.x || velocity.y);
		TC->position.x += velocity.x * speed * deltaTime;
		TC->position.y += velocity.y * speed * deltaTime;

		const auto& mouseWorldPoint = ObjectsManager::findByTag("mainCamera")->getComponent<CameraComponent>()->screenToWorldPoint(InputHandler::getMousePoint());
		
		auto curAnimation = gameObject->getComponent<RenderComponent>()->currentAnimation;

		int x = mouseWorldPoint.x - (TC->position.x + curAnimation->rotationPoint.x + curAnimation->localPosition.x);
		int y = mouseWorldPoint.y - (TC->position.y + curAnimation->rotationPoint.y + curAnimation->localPosition.y);

		if (x != 0 && y != 0)
		{
			float bodyAngle = acos(y / sqrt(x * x + y * y));
			TC->rotation = TC->position.x + curAnimation->rotationPoint.x + curAnimation->localPosition.x > mouseWorldPoint.x ?
				glm::degrees(bodyAngle) : glm::degrees(2 * 3.14f - bodyAngle);
		}

	}

	void setParametrs(const rapidjson::Value& jsonObject) override {};
};