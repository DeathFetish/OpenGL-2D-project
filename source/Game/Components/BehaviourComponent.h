#pragma once
#include "Component.h"

class GameObject;

class BehaviourComponent : public Component
{
public:
	BehaviourComponent() = delete;
	BehaviourComponent(const BehaviourComponent&) = delete;
	BehaviourComponent& operator = (const BehaviourComponent&) = delete;

	BehaviourComponent(GameObject* gameObject);
	~BehaviourComponent();

	virtual void init() {}
	virtual void onCollision(GameObject* gameObject) {}
	virtual void update() {}
	virtual void lateUpdate() {}
	virtual void onPreRender() {}
};
