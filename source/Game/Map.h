#pragma once
#include <vector>
#include "Renderer/Sprite.h"
#include "Engine/Collision.h"

using namespace std;
using namespace glm;

class Floor
{
private:
	Sprite* sprite;
	vec2 size;
	vec2 position;
	float rotation;
	vec2 repeatsForWrapMode;

public:
	Floor& operator = (const Floor&) = delete;
	Floor(const Floor&) = default;
	~Floor() = default;

	Floor(const string& spriteName, const vec2& position, const vec2& size, const float rotation)
		: sprite(resources.getSprite(spriteName)),
		size(sprite->getSize()), position(position), rotation(rotation),
		repeatsForWrapMode(size.x / this->size.x, size.y / this->size.y) {}

	void render() const
	{
		sprite->setSize(vec2(size.x * repeatsForWrapMode.x, size.y * repeatsForWrapMode.y));
		sprite->setPosition(position);
		sprite->setRotation(rotation);
		sprite->render(repeatsForWrapMode);
	}
};

class Wall
{
private:
	Sprite* sprite;
	vec2 size;
	vec2 position;
	float rotation;
	vec2 repeatsForWrapMode;

public:
	IShape* hitBox;
	Wall& operator = (const Wall&) = delete;
	Wall(const Wall&) = default;
	~Wall() = default;

	Wall(const string& spriteName, const vec2& position, const vec2& size, const float rotation)
		: sprite(resources.getSprite(spriteName)),
		size(sprite->getSize()), position(position), rotation(rotation),
		repeatsForWrapMode(size.x / this->size.x, size.y / this->size.y) 
	{
		hitBox = new Rectangle();

		((Rectangle*)hitBox)->points.push_back(vec2(position.x, position.y));
		((Rectangle*)hitBox)->points.push_back(vec2(position.x, position.y + size.y));
		((Rectangle*)hitBox)->points.push_back(vec2(position.x + size.x, position.y));
		((Rectangle*)hitBox)->points.push_back(vec2(position.x + size.x, position.y + size.y));
	}

	void render() const
	{
		sprite->setSize(vec2(size.x * repeatsForWrapMode.x, size.y * repeatsForWrapMode.y));
		sprite->setPosition(position);
		sprite->setRotation(rotation);
		sprite->render(repeatsForWrapMode);
	}
};

class DecorativeObject
{
private:
	Sprite* sprite;
	vec2 size;
	vec2 position;
	float rotation;

public:
	DecorativeObject& operator = (const DecorativeObject&) = delete;
	DecorativeObject(const DecorativeObject&) = default;
	~DecorativeObject() = default;

	DecorativeObject(const string& spriteName, const vec2& position, const float rotation)
		: sprite(resources.getSprite(spriteName)),
		size(sprite->getSize()), position(position), rotation(rotation) {}

	void render() const
	{
		sprite->setPosition(position);
		sprite->setRotation(rotation);
		sprite->render();
	}

};

class Map
{
private:
	vector<Floor> floors;
	vector<Wall> walls;
	vector<DecorativeObject> decorativeObjects;
	vec2 leftDownMapPoint;
	vec2 rightTopMapPoint;

public:
	Map() = delete;
	Map(const Map&) = delete;
	Map& operator = (const Map&) = delete;

	Map(const vec2& leftDownMapPoint, const vec2& rightTopMapPoint) : leftDownMapPoint(leftDownMapPoint), rightTopMapPoint(rightTopMapPoint) {}

	vec2 getLeftDownMapPoint() const { return leftDownMapPoint; }
	vec2 getRightTopMapPoint() const { return rightTopMapPoint; }

	void addObject(const string& spriteName, const vec2& position, const vec2& size, const float rotation, bool collision)
	{

	}

	void addFloor(const string& spriteName, const vec2& position, const vec2& size, const float rotation = 0.f)
	{ 
		floors.push_back(Floor(spriteName, position, size, rotation));
	}
	
	void addWall(const string& spriteName, const vec2& position, const vec2& size, const float rotation = 0.f)
	{
		walls.push_back(Wall(spriteName, position, size, rotation));
	}

	void addDecorativeObject(const string& spriteName, const vec2& position, const float rotation = 0.f)
	{
		decorativeObjects.push_back(DecorativeObject(spriteName, position, rotation));
	}

	void render() const
	{
		for (auto it = floors.cbegin(); it != floors.cend(); ++it)
			it->render();

		for (auto it = walls.cbegin(); it != walls.cend(); ++it)
			it->render();

		for (auto it = decorativeObjects.cbegin(); it != decorativeObjects.cend(); ++it)
			it->render();
	}

	bool calculateCollisions(IShape& hero)
	{
		for (auto it = walls.begin(); it != walls.end(); ++it)
		{
			if (calculateCollision(hero, *(it->hitBox)) == true)
				return true;
		}
		return false;
	}
};