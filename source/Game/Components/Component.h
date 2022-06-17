#pragma once
#include <rapidjson/document.h>

class GameObject;

class Component 
{
protected:
	GameObject* gameObject;
public:
	Component(GameObject* object) : gameObject(object) {}
	GameObject* const getGameObject() { return gameObject; }
	virtual void setParametrs(const rapidjson::Value& jsonObject) = 0;
};