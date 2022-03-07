#pragma once

#include <glm/vec2.hpp>
#include <functional>

#include "../Engine/Collision.h"
#include "../Managers/ResourceManager.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/SpriteAnimator.h"

class TransformComponent
{
public:
	glm::vec2 position;
	float rotation;

	TransformComponent() = delete;
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator = (const TransformComponent&) = delete;

	TransformComponent(const glm::vec2& position, const float rotation) 
		: position(position), rotation(rotation) {}

	~TransformComponent() = default;
};

class RenderComponent
{
public:
	RenderEngine::Sprite* sprite;
	RenderEngine::SpriteAnimator animator;

	glm::vec2 size;
	glm::vec2 rotationPoint;
	glm::vec2 pivotOffset;

	RenderComponent() = delete;
	RenderComponent(const RenderComponent&) = delete;
	RenderComponent& operator = (const RenderComponent&) = delete;

	RenderComponent(
		const std::string& spriteName,
		const glm::vec2& size,
		const glm::vec2& rotationPoint,
		const glm::vec2& pivotOffset)
		: sprite(resources.getSprite(spriteName)), size(size), rotationPoint(rotationPoint), pivotOffset(pivotOffset), animator(sprite) {}

	~RenderComponent() = default;
};

class PhysicComponent
{
public:
	IShape* walkHitBox;
	IShape* damageHitBox;
	std::function<void()> walkHitboxCallback;
	std::function<void()> damageHitboxCallback;

	PhysicComponent() = delete;
	PhysicComponent(const PhysicComponent&) = delete;
	PhysicComponent& operator = (const PhysicComponent&) = delete;

	PhysicComponent(IShape* walkHitBox, IShape* damgeHitBox) 
		: walkHitBox(walkHitBox), damageHitBox(damgeHitBox) {}

	~PhysicComponent()
	{
		delete walkHitBox;
		delete damageHitBox;
	};
};

class MovableComponent
{
public:
	glm::vec2 velocity;
	float speed;

	MovableComponent() = delete;
	MovableComponent(const MovableComponent&) = delete;
	MovableComponent& operator = (const MovableComponent&) = delete;

	MovableComponent(const float speed) : speed(speed) {}

	~MovableComponent() = default;
};

class GameObj
{
public:
	virtual void render() = 0;
	virtual void update() = 0;
	virtual PhysicComponent* const getPhysicComponent() = 0;
};

class PhysicalObject : public GameObj
{
private:
	TransformComponent* const TC;
	RenderComponent* const RC;
	PhysicComponent* const PC;

public:
	PhysicalObject(
		TransformComponent* const transformComponent,
		RenderComponent* const renderComponent,
		PhysicComponent* const physicComponent)
		: TC(transformComponent),
		RC(renderComponent),
		PC(physicComponent) {}

	void render() override
	{
	//	std::cout << TC->position.x << "\t" << TC->position.y;
		RC->sprite->render(TC->position, TC->rotation, RC->size, RC->rotationPoint, RC->pivotOffset, RC->animator.getCurrentFrame());
	}

	void update() override { }

	PhysicComponent* const getPhysicComponent() override { return PC; }

	~PhysicalObject()
	{
		if (!TC) delete TC;
		if (!RC) delete RC;
		if (!PC) delete PC;
	}
};