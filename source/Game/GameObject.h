#pragma once
#include <map>
#include <string>

#include "Components/Component.h"

class GameObject final
{
private:
	friend class ObjectsManager;

	std::string tag;
	GameObject* parent;
	std::map<std::string, Component*> components;
	std::map<std::string, GameObject*> children;

	GameObject() : parent(nullptr) {};

	~GameObject() 
	{
		for (auto it = components.begin(); it != components.end(); ++it)
			delete (it->second);
		components.clear();

		for (auto it = children.begin(); it != children.end(); ++it)
			delete (it->second);
		children.clear();
	}

public:

	template <typename T>
	T* const addComponent()
	{
		return static_cast<T*>(components[typeid(T).name()] = new T(this));
	}

	template <typename T>
	T* const getComponent()
	{
		return static_cast<T*>(components[typeid(T).name()]);
	}

	GameObject* addGameObject(const std::string& gameObjectTag)
	{
		auto object = new GameObject();
		object->parent = this;
		return (children[gameObjectTag] = object);
	}

	GameObject* getParent() { return parent; }

	const std::string& getTag() { return tag; }
	void setTag(const std::string) { this->tag = tag; }
};
