#pragma once
#include <array>
#include <vector>
#include <algorithm>

#include <glm/vec3.hpp>

#include "../Managers/ResourceManager.h"
#include "../Renderer/Camera.h"
#include "../Renderer/SpriteAnimator.h"
#include "../Engine/Collision.h"

#include "Components.h"
#include "GameObject.h"
#include "Hero.h"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <algorithm>

#define game Game::get()

class ComponentsParser
{
private:
	static IShape* parseHitBox(const rapidjson::Value& jsonObject)
	{
		glm::vec2 pivotOffset = glm::vec2(0, 0);
		
		if (jsonObject.HasMember("pivotOffsetX"))
			pivotOffset = glm::vec2(jsonObject["pivotOffsetX"].GetFloat(), jsonObject["pivotOffsetY"].GetFloat());

		if (jsonObject.HasMember("radius"))
		{
			float radius = jsonObject["radius"].GetFloat();
			return new Circle(glm::vec2(0, 0), radius, pivotOffset);
		}
		else
		{
			glm::vec2 size = glm::vec2(jsonObject["width"].GetFloat(), jsonObject["height"].GetFloat());
			glm::vec2 rotationPoint = jsonObject.HasMember("rotationPointX") ?
				glm::vec2(jsonObject["rotationPointX"].GetFloat(), jsonObject["rotationPointY"].GetFloat()) : glm::vec2(size.x / 2, size.y / 2);

			return new Rectangle(glm::vec2(0, 0), size, rotationPoint, pivotOffset);
		}
	}

public:
	static TransformComponent* const parseTransformComponent(const rapidjson::Value& jsonObject)
	{
		glm::vec2 position = glm::vec2(jsonObject["x"].GetFloat(), jsonObject["y"].GetFloat());
		float rotation = jsonObject.HasMember("rotation") ? jsonObject["rotation"].GetFloat() : 0;
		return new TransformComponent(position, rotation);
	}

	static RenderComponent* const parseRenderComponent(const rapidjson::Value& jsonObject)
	{
		const std::string spriteName = jsonObject["sprite"].GetString();
		const float layer = jsonObject["layer"].GetFloat();

		const glm::vec2 size = glm::vec2(jsonObject["width"].GetFloat(), jsonObject["height"].GetFloat());
		
		const glm::vec2 rotationPoint = jsonObject.HasMember("rotationPointX") ?
			glm::vec2(jsonObject["rotationPointX"].GetFloat(), jsonObject["rotationPointY"].GetFloat()) :
			glm::vec2(size.x / 2.f, size.y / 2.f);

		const glm::vec2 pivotOffset = jsonObject.HasMember("pivotOffsetX") ?
			glm::vec2(jsonObject["pivotOffsetX"].GetFloat(), jsonObject["pivotOffsetY"].GetFloat()) :
			glm::vec2(0, 0);

		return new RenderComponent(resources.getSprite(spriteName), size, rotationPoint, pivotOffset, layer);
	}

	static PhysicComponent* const parsePhysicComponent(const rapidjson::Value& jsonObject)
	{
		IShape* walkHitBox = nullptr;
		std::function<void(IGameObject*, IGameObject*, const glm::vec2&)> walkHitBoxOnCollision;
		std::function<void(IGameObject*, IGameObject*, const glm::vec2&)> damageHitBoxOnCollision;
		
		std::string walkHitBoxCallbackName = jsonObject.HasMember("walkHitBoxCallbackName") ? jsonObject["walkHitBoxCallbackName"].GetString() : "default";
		
		walkHitBoxOnCollision = MapObject::getWalkHitBoxCallback(walkHitBoxCallbackName);
		damageHitBoxOnCollision = walkHitBoxOnCollision;

		if (jsonObject.HasMember("walkHitBox"))
			walkHitBox = ComponentsParser::parseHitBox(jsonObject["walkHitBox"]);

		IShape* damageHitBox = nullptr;
		if (jsonObject.HasMember("damageHitBox"))
			damageHitBox = ComponentsParser::parseHitBox(jsonObject["damageHitBox"]);

		PhysicComponent* const PC = new PhysicComponent(walkHitBox, damageHitBox);
		PC->setCallback(walkHitBoxOnCollision, damageHitBoxOnCollision);
		return PC;
	}

	static MovableComponent* const parseMovableComponent(const rapidjson::Value& jsonObject)
	{
		const float speed = jsonObject["speed"].GetFloat();
		return new MovableComponent(speed);
	}

	static ShadowComponent* const parseShadowComponent(const rapidjson::Value& jsonObject)
	{
		const float layer = jsonObject["layer"].GetFloat();
		const glm::vec2 offset = glm::vec2(jsonObject["objectOffsetX"].GetFloat(), jsonObject["objectOffsetY"].GetFloat());
		return new ShadowComponent(layer, offset);
	}
};

class Game
{
private:
	Hero* hero;
	
	IGameObject* filmGrain;

	std::vector<MapObject*> gameObjects;
	std::vector<IGameObject*> renderableObjects;

	std::map<std::string, rapidjson::Document*> gameObjectsSamples;

	Game() {}

	~Game()
	{
		if (!hero) delete hero;

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
			delete *it;
		
		gameObjects.clear();
	}

public:

	Game(const Game&) = delete;
	Game& operator = (const Game&) = delete;

	static Game& get()
	{
		static Game object;
		return object;
	}

	void loadLevel(const std::string& filePath)
	{
		const std::string JSONString = resources.getFileString(filePath);
		if (JSONString.empty())
		{
			std::cout << "No JSON gameObjects file!" << std::endl;
			exit(-1);
		}

		rapidjson::Document document;
		rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());

		if (!parseResult)
		{
			cout << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << endl;
			cout << "In JSON file: " << filePath << endl;
			exit(-1);
		}

		auto gameObjectsIt = document.FindMember("objects");
		if (gameObjectsIt != document.MemberEnd())
		{
			for (const auto& currentGameObject : gameObjectsIt->value.GetArray())
			{
				std::string name = currentGameObject["name"].GetString();
				TransformComponent* TC = ComponentsParser::parseTransformComponent(currentGameObject);
				
				auto sampleObject = gameObjectsSamples[name];
				
				std::string tag = sampleObject->HasMember("tag") ? (*sampleObject)["tag"].GetString() : "";

				RenderComponent* RC = nullptr;
				if (sampleObject->HasMember("RenderComponent"))
					RC = ComponentsParser::parseRenderComponent((*sampleObject)["RenderComponent"]);

				PhysicComponent* PC = nullptr;
				if (sampleObject->HasMember("PhysicComponent"))
					PC = ComponentsParser::parsePhysicComponent((*sampleObject)["PhysicComponent"]);

				MovableComponent* MC = nullptr;
				if (sampleObject->HasMember("MovableComponent"))
					MC = ComponentsParser::parseMovableComponent((*sampleObject)["MovableComponent"]);

				ShadowComponent* SC = nullptr;
				if (sampleObject->HasMember("ShadowComponent"))
					SC = ComponentsParser::parseShadowComponent((*sampleObject)["ShadowComponent"]);

				MapObject* obj = new MapObject(tag, TC, RC, PC, MC, SC);
				string updateFunctionName = sampleObject->HasMember("UpdateFunction") ? (*sampleObject)["UpdateFunction"].GetString() : "default";
				string renderFunctionName = sampleObject->HasMember("RenderFunction") ? (*sampleObject)["RenderFunction"].GetString() : "default";

				obj->setUpdateFunction(MapObject::getUpdateFunction(updateFunctionName));
				obj->setRenderFunction(MapObject::getRenderFunction(renderFunctionName));

				if (name == "FilmGrain")
					filmGrain = obj;
				
				gameObjects.push_back(obj);
				if (RC)	renderableObjects.push_back(obj);
			}
		}
	}

	void loadGameObjectsSamples(const std::string& filePath)
	{
		const std::string JSONString = resources.getFileString(filePath);
		if (JSONString.empty())
		{
			std::cout << "No JSON gameObjects file!" << std::endl;
			exit(-1);
		}

		rapidjson::Document sourceDocument;
		rapidjson::ParseResult parseResult = sourceDocument.Parse(JSONString.c_str());

		if (!parseResult)
		{
			cout << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << endl;
			cout << "In JSON file: " << filePath << endl;
			exit(-1);
		}

		auto gameObjectsIt = sourceDocument.FindMember("gameObjects");
		if (gameObjectsIt != sourceDocument.MemberEnd())
		{
			const auto objects = gameObjectsIt->value.GetArray();
			for (auto i = 0; i < objects.Size(); i++)
			{
				string name = objects[i]["name"].GetString();

				rapidjson::Document* outDocument = new rapidjson::Document();
				outDocument->CopyFrom(objects[i], outDocument->GetAllocator());

				gameObjectsSamples.insert(std::make_pair(name, outDocument));
			}
		}
	}

	void loadHero(const std::string& JSONPath)
	{
		const std::string JSONString = resources.getFileString(JSONPath);
		if (JSONString.empty())
		{
			std::cout << "No JSON gameObjects file!" << std::endl;
			exit(-1);
		}

		rapidjson::Document document;
		rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());

		if (!parseResult)
		{
			cout << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << endl;
			cout << "In JSON file: " << JSONPath << endl;
			exit(-1);
		}

		const auto& HeroObjIt = document["Hero"];
		
		TransformComponent* bodyTC = ComponentsParser::parseTransformComponent(HeroObjIt["bodyTransformComponent"]);
		TransformComponent* legsTC = ComponentsParser::parseTransformComponent(HeroObjIt["legsTransformComponent"]);
		MovableComponent* MC = ComponentsParser::parseMovableComponent(HeroObjIt["MovableComponent"]);
		PhysicComponent* PC = ComponentsParser::parsePhysicComponent(HeroObjIt["PhysicComponent"]);
		RenderComponent* legsRC = ComponentsParser::parseRenderComponent(HeroObjIt["legsRenderComponent"]);

		std::map<std::string, RenderComponent* const> bodyStatesRC;

		auto bodyStatesRCIt = HeroObjIt.FindMember("bodyStatesRenderComponents");
		for (const auto& currentBodyRC : bodyStatesRCIt->value.GetArray())
		{
			const std::string state = currentBodyRC["state"].GetString();
			bodyStatesRC.emplace(state, ComponentsParser::parseRenderComponent(currentBodyRC));
		}

		hero = new Hero(
			const_cast<TransformComponent* const>(bodyTC),
			const_cast<TransformComponent* const>(legsTC),
			const_cast<MovableComponent* const>(MC),
			const_cast<PhysicComponent* const>(PC),
			const_cast<RenderComponent* const>(legsRC),
			bodyStatesRC);

		renderableObjects.push_back(hero);
	}

	bool init()
	{
		camera.init(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(4000, 4000), glm::vec2(4.f, 4.f), hero);
		
		std::sort(renderableObjects.begin(), renderableObjects.end(), [](IGameObject* a, IGameObject* b) -> bool
			{ return a->getRenderComponent()->layer < b->getRenderComponent()->layer; });

		return true;
	}

	void render()
	{
		resources.getShader("spriteShader")->setMatrix("viewProjectionMat", camera.getViewProjectionMatrix());
		resources.getShader("shadowShader")->setMatrix("viewProjectionMat", camera.getViewProjectionMatrix());
		resources.getShader("filmGrainShader")->setMatrix("viewProjectionMat", camera.getViewProjectionMatrix());

		filmGrain->getTransformComponent()->position = camera.getPosition();

		for (auto it = renderableObjects.begin(); it != renderableObjects.end(); ++it)
		{
			(*it)->render();
		}

		std::cout << "__________" << std::endl;
	}

	void update(const double deltaTime) 
	{
		hero->update(gameObjects);

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			(*it)->update();
		}

		camera.update();
	}
};