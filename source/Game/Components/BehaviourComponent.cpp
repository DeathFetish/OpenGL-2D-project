#include "BehaviourComponent.h"
#include "../ObjectsManager.h"

BehaviourComponent::BehaviourComponent(GameObject* gameObject) : Component(gameObject)
{
	ObjectsManager::addBehaviourComponentToUpdateQueue(this);
}

BehaviourComponent::~BehaviourComponent()
{
	ObjectsManager::removeFromUpdateQueue(this);
}
