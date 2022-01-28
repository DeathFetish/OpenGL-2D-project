#pragma once
#include <map>
#include <math.h>

#include "Renderer/Sprite.h"
#include "Renderer/SpriteAnimator.h"

using std::map;
using namespace RenderEngine;

class Hero
{
private:
	glm::vec2 size = glm::vec2(36, 19);
	glm::vec2 position = glm::vec2(0, 0);
	glm::vec2 velocity = glm::vec2(0, 0);

	float rotation = 0;
	const float speed = 0.15;

	std::map<std::string, Sprite*> sprites;
	SpriteAnimator* spriteAnimator;

	std::string currentState;

	bool isUpdate = false;

public:

	Hero()
	{
		currentState = "walk";
		sprites.emplace(currentState, resources.getSprite("HeroRightPunch"));
	//	sprites.emplace(currentState, resources.getSprite("HeroWalk"));
		spriteAnimator = new SpriteAnimator(sprites.at(currentState));
	}
	void render()
	{
		sprites[currentState]->render(position, rotation, spriteAnimator->getCurrentFrame());
	}
	void update(const double delta, const std::vector<bool>& keys, const glm::vec2& mouseWorldPoint)
	{
		velocity.x = 0;
		velocity.y = 0;

		if (keys[GLFW_KEY_A] == GLFW_PRESS)
			velocity.x = -1;
		if (keys[GLFW_KEY_D] == GLFW_PRESS)
			velocity.x = 1;

		if (keys[GLFW_KEY_S] == GLFW_PRESS)
			velocity.y = -1;
		if (keys[GLFW_KEY_W] == GLFW_PRESS)
			velocity.y = 1;
		
		if (velocity.x != 0 && velocity.y != 0)
			velocity = glm::normalize(velocity);

		position.x += velocity.x * speed * delta;
		position.y += velocity.y * speed * delta;


	//	if (velocity.x != 0 || velocity.y != 0)
			spriteAnimator->update(delta);
		
		int x = mouseWorldPoint.x - (position.x + size.x / 2);
		int y = mouseWorldPoint.y - (position.y + size.y / 2);
		float angle = acos(y / sqrt(x * x + y * y)) * 180 / 3.1415;

		rotation = position.x + size.x / 2 > mouseWorldPoint.x ? angle : 360 - angle;
	}
};
