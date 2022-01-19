#pragma once
#include <math.h>
#include "Renderer/AnimatedSprite.h"
#include "Resources/ResourceManager.h"
#include "Resources/WindowManager.h"

class Hero
{
private:
	enum class HeroState
	{
		running,
		standing
	};

	HeroState state;
	AnimatedSprite* sprite;
	vec2 vectorRight = vec2(1.0f, 0.0f);

	vec2 position;
	vec2 size;
	vec2 velocity;

	IShape* walkHitbox;
	vector<vec2> points;

	float rotation = 0;
	float speed = 0.5;

	const double PI = 3.141592653589793238463;

	const vec2 vectorUp = vec2(0.0f, 1.0f);
public:
	Hero(const Hero&) = delete;
	Hero& operator = (const Hero&) = delete;

	Hero(const vec2& position, const vec2& size) : sprite(resources.getAnimatedSprite("heroSprite")),
												   state(HeroState::standing),
												   rotation(0.f),
												   size(size),
												   position(position)
	{
		walkHitbox = new Circle(0.7 * sqrt((size.x * size.x / 4 + size.y * size.y / 4)), vec2(position.x + size.x / 2, position.y + size.y / 2));
	}

	~Hero() {}

	void render() const { sprite->render(); }
	vec2 getPosition() const { return position; }
	vec2 getSize() const { return size; }
	vec2 getScreenPosition() 
	{
		return vec2(position.x + (size.x - windowManger.screenSize.x) / 2, position.y + (size.y - windowManger.screenSize.y) / 2);
	}

	void update(const double deltaTime, vector<bool>& keys, double mouseCoordX, double mouseCoordY, Camera* camera, Map* map)
	{
		switch (keys[GLFW_KEY_A])
		{
		case GLFW_PRESS:
			velocity.x = -1;
			break;
		case GLFW_RELEASE:
			switch (keys[GLFW_KEY_D])
			{
			case GLFW_PRESS:
				velocity.x = 1;
				break;
			case GLFW_RELEASE:
				velocity.x = 0;
				break;
			}
			break;
		}

		switch (keys[GLFW_KEY_W])
		{
		case GLFW_PRESS:
			velocity.y = 1;
			break;
		case GLFW_RELEASE:
			switch (keys[GLFW_KEY_S])
			{
			case GLFW_PRESS:
				velocity.y = -1;
				break;
			case GLFW_RELEASE:
				velocity.y = 0;
				break;
			}
		}

		mouseCoordX += velocity.x * speed;
		mouseCoordY += velocity.y * speed;

		vec2 heroCenter(position.x + size.x / 2 - camera->getPosition().x, position.y + size.y / 2 - camera->getPosition().y);
		vec2 result(mouseCoordX - heroCenter.x, mouseCoordY - heroCenter.y);
		result /= sqrt(result.x * result.x + result.y * result.y);
		
		if (heroCenter.x < mouseCoordX)
			rotation = 360 - acos(result.x * vectorUp.x + result.y * vectorUp.y) * 180 / PI;
		else
			rotation = acos(result.x * vectorUp.x + result.y * vectorUp.y) * 180 / PI;
		
		if (velocity.x != 0 && velocity.y != 0)
		{
			((Circle*)walkHitbox)->setCenter(vec2(position.x + size.x / 2 + velocity.x * speed, position.y + size.y / 2 + velocity.y * speed));
			if (map->calculateCollisions(*walkHitbox) == false)
			{
				position.x += velocity.x * speed;
				sprite->update(deltaTime);
			}
			else
			{
				((Circle*)walkHitbox)->setCenter(vec2(position.x + size.x / 2 + velocity.x * speed, position.y + size.y / 2));
				if (map->calculateCollisions(*walkHitbox) == false)
				{
					position.x += velocity.x * speed;
					sprite->update(deltaTime);
				}
				else
				{
					((Circle*)walkHitbox)->setCenter(vec2(position.x + size.x / 2, position.y + size.y / 2 + velocity.y * speed));
					if (map->calculateCollisions(*walkHitbox) == false)
					{
						position.y += velocity.y * speed;
						sprite->update(deltaTime);
					}
				}
			}
		}
		else if (velocity.x != 0 || velocity.y != 0)
		{
			((Circle*)walkHitbox)->setCenter(vec2(position.x + size.x / 2 + velocity.x * speed, position.y + size.y / 2 + velocity.y * speed));
			if (map->calculateCollisions(*walkHitbox) == false)
			{
				position += velocity * speed;
				sprite->update(deltaTime);
			}
		}
		
		sprite->setPosition(position);
		sprite->setRotation(rotation);
	}
};