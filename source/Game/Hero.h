#pragma once
#define _USE_MATH_DEFINES

#include <map>
#include <math.h>

#include "Renderer/Sprite.h"
#include "Renderer/SpriteAnimator.h"
#include "GameObject.h"


#include <functional>

using namespace RenderEngine;

class Timer 
{
public:
	Timer() : isRunning(false), timeLeft(0) {}
	void update(const double delta)
	{
		if (isRunning)
		{
			timeLeft -= delta;
			if (timeLeft <= 0)
			{
				isRunning = false;
				callback();
			}
		}
	}
	void start(const double duration)
	{
		timeLeft = duration;
		isRunning = true;
	}
	void setCallback(std::function<void()> callback) { this->callback = callback; }

private:
	std::function<void()> callback;
	double timeLeft;
	bool isRunning;
};



class Hero : public GameObject
{
private:
	glm::vec2 size = glm::vec2(36, 19);
	glm::vec2 velocity = glm::vec2(0, 0);

	float bodyRotation = 0;
	float legRotation;

	const float speed = 0.2f;
	std::map<std::string, Sprite*> sprites;
	Sprite* legs;

	SpriteAnimator* legsAnimator;
	SpriteAnimator* bodyAnimator;

	std::string currentState;

	Timer fightTimer;

	bool isPunch = false;

public:

	Hero()
	{
		currentState = "HeroWalk";
		sprites.emplace("HeroRightPunch", resources.getSprite("HeroRightPunch"));
		sprites.emplace(currentState, resources.getSprite("HeroWalk"));
		bodyAnimator = new SpriteAnimator(sprites[currentState]);
		legs = resources.getSprite("HeroLegs");
		legsAnimator = new SpriteAnimator(legs);

		fightTimer.setCallback([&]()
			{
				currentState = "HeroWalk";
				bodyAnimator->setSprite(sprites[currentState]);
				isPunch = false;
			});

		position = glm::vec2(0, 0);
	}
	void render()
	{

		if (velocity.x != 0 || velocity.y != 0)
			legs->render(position, legRotation, legsAnimator->getCurrentFrame());

		sprites[currentState]->render(position, bodyRotation, bodyAnimator->getCurrentFrame());

	}
	void update(const double delta, const std::vector<bool>& keys, const std::vector<bool>& mouseButtons, const glm::vec2& mouseWorldPoint)
	{
		fightTimer.update(delta);

		velocity.x = 0;
		velocity.y = 0;

		if (keys[GLFW_KEY_A] == GLFW_PRESS)
			velocity.x -= 1;
		if (keys[GLFW_KEY_D] == GLFW_PRESS)
			velocity.x += 1;
		if (keys[GLFW_KEY_S] == GLFW_PRESS)
			velocity.y -= 1;
		if (keys[GLFW_KEY_W] == GLFW_PRESS)
			velocity.y += 1;

		if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT] && !isPunch)
		{
			isPunch = true;
			currentState = "HeroRightPunch";
			bodyAnimator->setSprite(sprites[currentState]);
			fightTimer.start(bodyAnimator->getTotalDuration());
		}

		if (velocity.x != 0 && velocity.y != 0)
			velocity = glm::normalize(velocity);

		position.x += velocity.x * speed * delta;
		position.y += velocity.y * speed * delta;


		if (velocity.x != 0 || velocity.y != 0 || currentState.find("Walk") == string::npos)
			bodyAnimator->update(delta);

		if (velocity.x != 0 || velocity.y != 0)
			legsAnimator->update(delta);
		else if (velocity.x == 0 && velocity.y == 0)
			legsAnimator->reset();
	
		int x = mouseWorldPoint.x - (position.x + size.x / 2);
		int y = mouseWorldPoint.y - (position.y + size.y / 2);
	//	float angle = acos(y / sqrt(x * x + y * y)) * 180 / 3.1415;
		float bodyAngle = acos(y / sqrt(x * x + y * y));
		float legsAngle = acos((velocity.y) / sqrt(velocity.x * velocity.x + velocity.y * velocity.y));


		bodyRotation = position.x + size.x / 2 > mouseWorldPoint.x ? bodyAngle : 2 * M_PI - bodyAngle;
		legRotation = velocity.x > 0 ? 2 * M_PI - legsAngle : legsAngle;
	}
};
