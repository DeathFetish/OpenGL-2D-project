#pragma once
#define _USE_MATH_DEFINES

#include <map>
#include <math.h>
#include <glm/vec2.hpp>

#include "../Renderer/Sprite.h"
#include "../Renderer/SpriteAnimator.h"
#include "../Renderer/Camera.h"
#include "../System/Stopwatch.h"
#include "../System/Time.h"
#include "Components.h"
#include "GameObject.h"
#include "GameHandler.h"

class Hero : public IGameObject
{
private:
	TransformComponent* const bodyTC;
	TransformComponent* const legsTC;
	MovableComponent* const MC;
	PhysicComponent* const PC;

	RenderComponent* const legsRC;

	std::map<std::string, RenderComponent* const> bodyStatesRC;

	RenderComponent* currentBodyRC;

	std::string currentState;

	Stopwatch fightTimer;
	Stopwatch weaponChangeTimer;
	IGameObject* currentWeapon = nullptr;

	bool isFlipped = false;

	bool cooldown = false;
public:

	Hero(
		TransformComponent* const bodyTÑ,
		TransformComponent* const legsTC,
		MovableComponent* const MC,
		PhysicComponent* const PC,
		RenderComponent* const legsRC,
		const std::map<std::string, RenderComponent* const> bodyStatesRC)
		: bodyTC(bodyTÑ), legsTC(legsTC), MC(MC), PC(PC), legsRC(legsRC), bodyStatesRC(bodyStatesRC), currentState("HeroWalk")
	{
		currentBodyRC = this->bodyStatesRC[currentState];

		((Circle*)(this->PC)->walkHitBox)->setCenter(
			glm::vec2(bodyTC->position.x + currentBodyRC->rotationPoint.x + currentBodyRC->pivotOffset.x,
				bodyTC->position.y + currentBodyRC->rotationPoint.y + currentBodyRC->pivotOffset.y));

		fightTimer.setCallback([&]()
			{
				currentState = "HeroWalk";
				currentBodyRC = this->bodyStatesRC[currentState];
				cooldown = false;
			});

		weaponChangeTimer.setCallback([&]()
			{
				cooldown = false;
			});
	}

	void render() override
	{
		if (MC->velocity.x != 0 || MC->velocity.y != 0)
			legsRC->sprite->render(legsTC->position, legsTC->rotation, legsRC->size, legsRC->rotationPoint, 
				legsRC->pivotOffset, legsRC->layer, legsRC->animator.getCurrentFrame());
		
		currentBodyRC->sprite->render(bodyTC->position, bodyTC->rotation,
			currentBodyRC->size, currentBodyRC->rotationPoint,
			currentBodyRC->pivotOffset, currentBodyRC->layer, currentBodyRC->animator.getCurrentFrame(), isFlipped);
		
	}
	void update() override {}

	void update(std::vector<MapObject*>& gameObjects)
	{
		const double delta = time.getDeltaTime();

		fightTimer.update(delta);
		weaponChangeTimer.update(delta);

		MC->velocity.x = 0;
		MC->velocity.y = 0;

		const std::vector<bool>& keys = gameHandler.getKeys();
		const std::vector<bool>& mouseButtons = gameHandler.getMouseButtons();

		if (keys[GLFW_KEY_A] == GLFW_PRESS)
			MC->velocity.x -= 1;
		if (keys[GLFW_KEY_D] == GLFW_PRESS)
			MC->velocity.x += 1;
		if (keys[GLFW_KEY_S] == GLFW_PRESS)
			MC->velocity.y -= 1;
		if (keys[GLFW_KEY_W] == GLFW_PRESS)
			MC->velocity.y += 1;
		
		if (MC->velocity.x != 0 && MC->velocity.y != 0)
			MC->velocity = glm::normalize(MC->velocity);
		
		glm::vec2 testPosition = glm::vec2(bodyTC->position.x + MC->velocity.x * MC->speed * delta,
			bodyTC->position.y + MC->velocity.y * MC->speed * delta);
	
		PC->walkHitBox->recalculateIShape(testPosition, bodyTC->rotation);

		glm::vec2 collisionDirection;
		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			if ((*it)->getPhysicComponent() && calculateCollision(*(PC->walkHitBox), *((*it)->getPhysicComponent()->walkHitBox), collisionDirection))
			{
				if ((*it)->getTag() == "door")
				{
					((*it)->getPhysicComponent())->walkHitBoxOnCollision(*it, this, collisionDirection);
				}

				if ((*it)->getTag() == "Bat" && (*it) != currentWeapon)
				{
					if (!cooldown && mouseButtons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS)
					{
						currentState = "HeroWalk" + (*it)->getTag();
						currentBodyRC = this->bodyStatesRC[currentState];
						currentWeapon = (*it);
						cooldown = true;
						weaponChangeTimer.start(200);

						((*it)->getPhysicComponent())->walkHitBoxOnCollision(*it, this, collisionDirection);
					}
				}

				else if ((*it)->getTag() == "wall")
				{
					if (MC->velocity.x != 0 && MC->velocity.y != 0)
					{
						PC->walkHitBox->recalculateIShape(glm::vec2(testPosition.x, bodyTC->position.y), bodyTC->rotation);
						if (calculateCollision(*(PC->walkHitBox), *((*it)->getPhysicComponent()->walkHitBox)))
							MC->velocity.x = 0;

						PC->walkHitBox->recalculateIShape(glm::vec2(bodyTC->position.x, testPosition.y), bodyTC->rotation);
						if (calculateCollision(*(PC->walkHitBox), *((*it)->getPhysicComponent()->walkHitBox)))
							MC->velocity.y = 0;

						if (MC->velocity.x == 0 && MC->velocity.y == 0)
							break;

						testPosition = glm::vec2(
							bodyTC->position.x + MC->velocity.x * MC->speed * delta,
							bodyTC->position.y + MC->velocity.y * MC->speed * delta);

						PC->walkHitBox->recalculateIShape(testPosition, bodyTC->rotation);
					}
					else
					{
						MC->velocity = glm::vec2(0, 0);
						break;
					}
				}
				
			}
		}

		if (!cooldown && mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
		{
			cooldown = true;
			isFlipped = !isFlipped;
			currentState = "HeroPunch";
			currentBodyRC = bodyStatesRC[currentState];
			fightTimer.start(currentBodyRC->animator.getTotalDuration());
			currentBodyRC->animator.reset();
		}

		bodyTC->position.x += MC->velocity.x * MC->speed * delta;
		bodyTC->position.y += MC->velocity.y * MC->speed * delta;

		legsTC->position = bodyTC->position;

		if (MC->velocity.x != 0 || MC->velocity.y != 0 || currentState.find("Walk") == string::npos)
			currentBodyRC->animator.update(delta);

		if (MC->velocity.x != 0 || MC->velocity.y != 0)
			legsRC->animator.update(delta);
		else if (MC->velocity.x == 0 && MC->velocity.y == 0)
			legsRC->animator.reset();
		
		const auto& mouseWorldPoint = camera.screenToWorldPoint(gameHandler.getMousePoint());

		int x = mouseWorldPoint.x - (bodyTC->position.x + currentBodyRC->rotationPoint.x + currentBodyRC->pivotOffset.x);
		int y = mouseWorldPoint.y - (bodyTC->position.y + currentBodyRC->rotationPoint.y + currentBodyRC->pivotOffset.y);
		
		if (x != 0 && y != 0)
		{
			float bodyAngle = acos(y / sqrt(x * x + y * y));
			float legsAngle = acos((MC->velocity.y) / sqrt(MC->velocity.x * MC->velocity.x + MC->velocity.y * MC->velocity.y));

			bodyTC->rotation = bodyTC->position.x + currentBodyRC->rotationPoint.x + currentBodyRC->pivotOffset.x > mouseWorldPoint.x ?
				glm::degrees(bodyAngle) : glm::degrees(2 * M_PI - bodyAngle);
			legsTC->rotation = MC->velocity.x > 0 ? glm::degrees(2 * M_PI - legsAngle) : glm::degrees(legsAngle);
		}

		if (!cooldown && mouseButtons[GLFW_MOUSE_BUTTON_RIGHT] && currentWeapon)
		{
			cooldown = true;
			weaponChangeTimer.start(300);
			
			currentWeapon->enableRender(true);
			currentWeapon->enableUpdate(true);
			
			currentWeapon->getMovableComponent()->speed = 0.4f;
			currentWeapon->getMovableComponent()->velocity = glm::normalize(glm::vec2(x, y));
			currentWeapon->getTransformComponent()->position = bodyTC->position;
			
			currentWeapon->getPhysicComponent()->walkHitBox->recalculateIShape(bodyTC->position, currentWeapon->getTransformComponent()->rotation);

			currentWeapon = nullptr;
			currentState = "HeroWalk";
			currentBodyRC = bodyStatesRC[currentState];
		}
	}

	PhysicComponent* const getPhysicComponent() override { return PC; }
	RenderComponent* const getRenderComponent() override { return currentBodyRC; }
	TransformComponent* const getTransformComponent() override { return bodyTC; }
	MovableComponent* const getMovableComponent() override { return MC; }
	ShadowComponent* const getShadowComponent() override { return nullptr; }
};