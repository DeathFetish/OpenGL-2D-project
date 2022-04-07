#pragma once

#include <glm/vec2.hpp>
#include <functional>
#include "GameObject.h"
#include "../Engine/Collision.h"
#include "../Managers/ResourceManager.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/SpriteAnimator.h"

class IGameObject;

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
	float layer;

	RenderComponent() = delete;
	RenderComponent(const RenderComponent&) = delete;
	RenderComponent& operator = (const RenderComponent&) = delete;

	RenderComponent(
		RenderEngine::Sprite* sprite,
		const glm::vec2& size,
		const glm::vec2& rotationPoint,
		const glm::vec2& pivotOffset,
		const float layer)
		: sprite(sprite), size(size), rotationPoint(rotationPoint),
		pivotOffset(pivotOffset), animator(sprite), layer(layer) 
	{
		std::cout << layer << std::endl;
	}

	~RenderComponent() = default;
};

class PhysicComponent
{
public:

	IShape* walkHitBox;
	IShape* damageHitBox;
	
	std::function<void(IGameObject*, IGameObject*, const glm::vec2&)> walkHitBoxOnCollision;
	std::function<void(IGameObject*, IGameObject*, const glm::vec2&)> damageHitBoxOnCollision;

	PhysicComponent() = delete;
	PhysicComponent(const PhysicComponent&) = delete;
	PhysicComponent& operator = (const PhysicComponent&) = delete;

	PhysicComponent(IShape* walkHitBox, IShape* damgeHitBox) 
		: walkHitBox(walkHitBox), damageHitBox(damgeHitBox) {}

	void setCallback(
		const std::function<void(IGameObject*, IGameObject*, const glm::vec2&)> walkHitBoxOnCollision,
		const std::function<void(IGameObject*, IGameObject*, const glm::vec2&)> damageHitBoxOnCollision)
	{
		this->walkHitBoxOnCollision = walkHitBoxOnCollision;
		this->damageHitBoxOnCollision = damageHitBoxOnCollision;
	}

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

class ShadowComponent
{
public: 
	float layer;
	glm::vec2 objectOffset;

	ShadowComponent() = delete;
	ShadowComponent(const ShadowComponent&) = delete;
	ShadowComponent& operator = (const ShadowComponent&) = delete;

	ShadowComponent(const float layer, const glm::vec2& objectOffset) : layer(layer), objectOffset(objectOffset) {}

	~ShadowComponent() = default;
};