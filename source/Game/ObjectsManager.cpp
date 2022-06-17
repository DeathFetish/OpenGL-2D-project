#pragma once
#include <typeinfo>

#include "ObjectsManager.h"
#include "GameObject.h"

#include "Components/Component.h"
#include "Components/TransformComponent.h"
#include "Components/PhysicComponent.h"
#include "Components/CameraComponent.h"
#include "Components/HeroComponent.h"

void ObjectsManager::setComponentsInGameObject(GameObject* gameObject, const rapidjson::Value& jsonObject)
{
	if (jsonObject.HasMember("tag"))
		gameObject->tag = jsonObject["tag"].GetString();

	if (jsonObject.HasMember("PhysicComponent"))
	{
		auto component = gameObject->addComponent<PhysicComponent>();
		component->setParametrs(jsonObject["PhysicComponent"]);
	}

	if (jsonObject.HasMember("TransformComponent"))
	{
		auto component = gameObject->addComponent<TransformComponent>();
		component->setParametrs(jsonObject["TransformComponent"]);
	}

	if (jsonObject.HasMember("RenderComponent"))
	{
		auto component = gameObject->addComponent<RenderComponent>();
		component->setParametrs(jsonObject["RenderComponent"]);
	}

	if (jsonObject.HasMember("CameraComponent"))
	{
		auto component = gameObject->addComponent<CameraComponent>();
		component->setParametrs(jsonObject["CameraComponent"]);
	}

	if (jsonObject.HasMember("HeroComponent"))
	{
		auto component = gameObject->addComponent<HeroComponent>();
		component->setParametrs(jsonObject["HeroComponent"]);
	}

	auto gameObjectsIt = jsonObject.FindMember("GameObjects");

	if (gameObjectsIt != jsonObject.MemberEnd())
	{
		const auto objectsArray = gameObjectsIt->value.GetArray();
		for (const auto& jsonChildrenObject : objectsArray)
		{
			auto children = gameObject->addGameObject(jsonChildrenObject["tag"].GetString());
			setComponentsInGameObject(children, jsonChildrenObject);
		}
	}
}

void ObjectsManager::loadGameObjectsSamples(const std::string& filePath)
{
	std::cout << "---Game-Objects-Samples---" << std::endl;

	rapidjson::Document sourceDocument;
	Parser::parseJsonFile(filePath, &sourceDocument);

	auto gameObjectsIt = sourceDocument.FindMember("GameObject");
	if (gameObjectsIt != sourceDocument.MemberEnd())
	{
		const auto objects = gameObjectsIt->value.GetArray();
		for (auto i = 0; i < objects.Size(); i++)
		{
			std::string name = objects[i]["name"].GetString();

			std::cout << "\t" << name << std::endl;
			if (gameObjectsSamples.find(name) != gameObjectsSamples.end())
			{
				std::cout << "Error! GameObject sample already exists. Name: " << name << "." << std::endl;
				exit(-1);
			}

			rapidjson::Document* outDocument = new rapidjson::Document();
			outDocument->CopyFrom(objects[i], outDocument->GetAllocator());
			gameObjectsSamples[name] = outDocument;
		}
	}
}

void ObjectsManager::loadGameObjects(const std::string& filePath)
{
	rapidjson::Document document;
	Parser::parseJsonFile(filePath, &document);

	std::cout << "---Levels-Game--Objects---" << std::endl;

	auto gameObjectsIt = document.FindMember("GameObject");

	if (gameObjectsIt != document.MemberEnd())
	{
		const auto& objectsArray = gameObjectsIt->value.GetArray();
		for (const auto& jsonGameObject : objectsArray)
		{
			std::string name = jsonGameObject["name"].GetString();
			std::cout << "\t" << name << std::endl;

			if (gameObjectsSamples.find(name) == gameObjectsSamples.end())
			{
				std::cout << "Error! GameObject sample does not exist. Name: " << name << std::endl;
				exit(-1);
			}

			GameObject* object = new GameObject();

			auto component = object->addComponent<TransformComponent>();
			component->setParametrs(jsonGameObject["TransformComponent"]);

			setComponentsInGameObject(object, *gameObjectsSamples[name]);

			gameObjects.push_back(object);
		}
	}
}

void ObjectsManager::deleteGameObjects()
{
	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		delete (*it);
	gameObjects.clear();
}

void ObjectsManager::deleteGameObjectsSamples()
{
	for (auto it = gameObjectsSamples.begin(); it != gameObjectsSamples.end(); ++it)
		delete it->second;
	gameObjectsSamples.clear();
}

void ObjectsManager::addRenderComponentToRenderQueue(RenderComponent* RC)
{
	for (auto it = renderQueue.begin(); it != renderQueue.end(); ++it)
	{
		if (RC->layer >= (*it)->layer)
		{
			renderQueue.insert(it, RC);
			return;
		}
	}

	renderQueue.push_back(RC);
}

void ObjectsManager::removeFromRenderQueue(RenderComponent* RC)
{
	renderQueue.remove(RC);
}

void ObjectsManager::addBehaviourComponentToUpdateQueue(BehaviourComponent* BC)
{
	updateQueue.push_back(BC);
}

void ObjectsManager::removeFromUpdateQueue(BehaviourComponent* BC) 
{
	updateQueue.remove(BC);
}


std::list<RenderComponent*>& ObjectsManager::getRenderQueue() { return renderQueue; }

std::list<BehaviourComponent*>& ObjectsManager::getUpdateQueue() { return updateQueue; }

GameObject* ObjectsManager::findByTag(const std::string name)
{
	for (auto object : gameObjects)
	{
		if (object->tag == name)
			return object;
	}
	return nullptr;
}

std::map<std::string, rapidjson::Document*> ObjectsManager::gameObjectsSamples;
std::vector<GameObject*> ObjectsManager::gameObjects;
std::list<RenderComponent*> ObjectsManager::renderQueue;
std::list<BehaviourComponent*> ObjectsManager::updateQueue;
