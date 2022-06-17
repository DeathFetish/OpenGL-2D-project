#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "Components/RenderComponent.h"
#include "Components/BehaviourComponent.h"

#include "../System/Parser.h"

class GameObject;

class ObjectsManager final
{
private:
	static std::map<std::string, rapidjson::Document*> gameObjectsSamples;
	static std::vector<GameObject*> gameObjects;

	static std::list<RenderComponent*> renderQueue;
	static std::list<BehaviourComponent*> updateQueue;

	static void setComponentsInGameObject(GameObject* gameObject, const rapidjson::Value& jsonObject);

public:
	ObjectsManager() = delete;
	~ObjectsManager() = delete;
	ObjectsManager(const ObjectsManager&) = delete;
	ObjectsManager& operator = (const ObjectsManager&) = delete;

	static void loadGameObjectsSamples(const std::string& filePath);
	static void loadGameObjects(const std::string& filePath);
	
	static void deleteGameObjects();
	static void deleteGameObjectsSamples();

	static void addRenderComponentToRenderQueue(RenderComponent* RC);
	static void removeFromRenderQueue(RenderComponent* RC);

	static void addBehaviourComponentToUpdateQueue(BehaviourComponent* BC);
	static void removeFromUpdateQueue(BehaviourComponent* BC);

	static std::list<RenderComponent*>& getRenderQueue();
	static std::list<BehaviourComponent*>& getUpdateQueue();

	static GameObject* findByTag(const std::string name);
};
