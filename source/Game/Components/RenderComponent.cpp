#pragma once
#include "../GameObject.h"
#include "../ObjectsManager.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent(GameObject* gameObject) : Component(gameObject)
{
	ObjectsManager::addRenderComponentToRenderQueue(this);
}

RenderComponent::~RenderComponent()
{
	ObjectsManager::removeFromRenderQueue(this);
}

