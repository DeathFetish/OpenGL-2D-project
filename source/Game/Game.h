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
#include "Hero.h"

#define game Game::get()

class Cursor
{
private:
	Sprite* cursor;
	SpriteAnimator* cursorAnimator;
	glm::vec2 position;
public:
	Cursor()
	{
		cursor = resources.getSprite("Cursor");
		cursorAnimator = new SpriteAnimator(cursor);
	}

	void update()
	{
		this->position = position;
		cursorAnimator->update(time.getDeltaTime());
	}

	void render()
	{
		cursor->render(camera.screenToWorldPoint(gameHandler.getMousePoint()), 0.f, cursorAnimator->getCurrentFrame());
	}
};

class Game
{
private:

	Hero* hero;
	Cursor* cursor;
	std::vector<PhysicalObject*> gameObjects;

	Game() {}

	~Game()
	{
		if (!hero) delete hero;
		if (!cursor) delete cursor;
		
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

	void loadJsonGameObjects(const std::string& JSONPath)
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
		std::cout << "_0_" << std::endl;

		auto gameObjectsIt = document.FindMember("gameObjects");
		if (gameObjectsIt != document.MemberEnd())
		{
			std::cout << "start: gameObjects parsing" << std::endl;

			for (const auto& currentGameObject : gameObjectsIt->value.GetArray())
			{
				TransformComponent* TC = nullptr;
				RenderComponent* RC = nullptr;
				PhysicComponent* PC = nullptr;

				if (currentGameObject.HasMember("TransformComponent"))
				{
					const auto& TCIt = currentGameObject["TransformComponent"];
					const glm::vec2 position = glm::vec2(TCIt["x"].GetFloat(), TCIt["y"].GetFloat());
					const float rotation = TCIt["rotation"].GetFloat();
					TC = new TransformComponent(position, rotation);
					std::cout << "_TC_" << std::endl;
				}

				if (currentGameObject.HasMember("RenderComponent"))
				{
					const auto& RCIt = currentGameObject["RenderComponent"];
					const std::string spriteName = RCIt["sprite"].GetString();
					const glm::vec2 size = glm::vec2(RCIt["width"].GetFloat(), RCIt["height"].GetFloat());

					const glm::vec2 rotationPoint = RCIt.HasMember("rotationPointX") ?
						glm::vec2(RCIt["rotationPointX"].GetFloat(), RCIt["rotationPointY"].GetFloat()) :
						glm::vec2(size.x / 2, size.y / 2);

					const glm::vec2 pivotOffset = RCIt.HasMember("pivotOffsetX") ?
						glm::vec2(RCIt["pivotOffsetX"].GetFloat(), RCIt["pivotOffsetY"].GetFloat()) :
						glm::vec2(size.x / 2, size.y / 2);

					RC = new RenderComponent(spriteName, size, rotationPoint, pivotOffset);
					std::cout << "_RC_" << std::endl;
				}

				if (currentGameObject.HasMember("PhysicComponent"))
				{
					const auto& PCIt = currentGameObject["PhysicComponent"];

					IShape* walkHitBox;
					if (PCIt.HasMember("walkHitBox"))
					{
						const auto& walkHitBoxIt = PCIt["walkHitBox"];
						if (walkHitBoxIt.HasMember("radius"))
						{
							float radius = walkHitBoxIt["radius"].GetFloat();
							glm::vec2 center = glm::vec2(walkHitBoxIt["x"].GetFloat(), walkHitBoxIt["y"].GetFloat());
							walkHitBox = new Circle(radius, center);
						}
						else
						{
							float width = walkHitBoxIt["width"].GetFloat();
							float height = walkHitBoxIt["height"].GetFloat();
							std::vector<glm::vec2> points =
							{
								glm::vec2(TC->position.x, TC->position.y),
								glm::vec2(TC->position.x, TC->position.y + height),
								glm::vec2(TC->position.x + width, TC->position.y + height),
								glm::vec2(TC->position.x + width, TC->position.y),
							};
							walkHitBox = new Rectangle(points);
						}
					}

					IShape* damageHitBox;
					if (PCIt.HasMember("damageHitBox"))
					{
						const auto& damageHitBoxIt = PCIt["damageHitBox"];
						if (damageHitBoxIt.HasMember("radius"))
						{
							float radius = damageHitBoxIt["radius"].GetFloat();
							glm::vec2 center = glm::vec2(damageHitBoxIt["x"].GetFloat(), damageHitBoxIt["y"].GetFloat());
							damageHitBox = new Circle(radius, center);
						}
						else
						{
							float width = damageHitBoxIt["width"].GetFloat();
							float height = damageHitBoxIt["height"].GetFloat();
							std::vector<glm::vec2> points =
							{
								glm::vec2(TC->position.x, TC->position.y),
								glm::vec2(TC->position.x, TC->position.y + height),
								glm::vec2(TC->position.x + width, TC->position.y + height),
								glm::vec2(TC->position.x + width, TC->position.y),
							};
							damageHitBox = new Rectangle(points);
						}
					}

					PC = new PhysicComponent(walkHitBox, damageHitBox);
				}

				std::cout << "final" << std::endl;

				gameObjects.emplace_back(new PhysicalObject(TC, RC, PC));
			}
		}

		const auto& HeroObjIt = document["Hero"];
		TransformComponent* bodyTC = nullptr;
		TransformComponent* legsTC = nullptr;
		MovableComponent* MC = nullptr;
		PhysicComponent* PC = nullptr;
		RenderComponent* legsRC = nullptr;
		std::map<std::string, RenderComponent* const> bodyStatesRC;

		{
			const auto& bodyTCIt = HeroObjIt["bodyTransformComponent"];
			const glm::vec2 position = glm::vec2(bodyTCIt["x"].GetFloat(), bodyTCIt["y"].GetFloat());
			const float rotation = bodyTCIt["rotation"].GetFloat();
			bodyTC = new TransformComponent(position, rotation);
		}

		{
			const auto& legsTCIt = HeroObjIt["legsTransformComponent"];
			const glm::vec2 position = glm::vec2(legsTCIt["x"].GetFloat(), legsTCIt["y"].GetFloat());
			const float rotation = legsTCIt["rotation"].GetFloat();
			legsTC = new TransformComponent(position, rotation);
		}

		{
			const auto& MCIt = HeroObjIt["MovableComponent"];
			const float speed = MCIt["speed"].GetFloat();
			MC = new MovableComponent(speed);
		}

		{
			const auto& PCIt = HeroObjIt["PhysicComponent"];

			IShape* walkHitBox;
			const auto& walkHitBoxIt = PCIt["walkHitBox"];
			if (walkHitBoxIt.HasMember("radius"))
			{
				float radius = walkHitBoxIt["radius"].GetFloat();
				glm::vec2 center = glm::vec2(walkHitBoxIt["x"].GetFloat(), walkHitBoxIt["y"].GetFloat());
				walkHitBox = new Circle(radius, center);
			}
			else
			{
				float width = walkHitBoxIt["width"].GetFloat();
				float height = walkHitBoxIt["height"].GetFloat();
				std::vector<glm::vec2> points =
				{
					glm::vec2(bodyTC->position.x, bodyTC->position.y),
					glm::vec2(bodyTC->position.x, bodyTC->position.y + height),
					glm::vec2(bodyTC->position.x + width, bodyTC->position.y + height),
					glm::vec2(bodyTC->position.x + width, bodyTC->position.y),
				};
				walkHitBox = new Rectangle(points);
				std::cout << "Rect_1" << std::endl;

			}

			IShape* damageHitBox;
			const auto& damageHitBoxIt = PCIt["damageHitBox"];
			if (damageHitBoxIt.HasMember("radius"))
			{
				float radius = damageHitBoxIt["radius"].GetFloat();
				glm::vec2 center = glm::vec2(damageHitBoxIt["x"].GetFloat(), damageHitBoxIt["y"].GetFloat());
				damageHitBox = new Circle(radius, center);
			}
			else
			{
				float width = damageHitBoxIt["width"].GetFloat();
				float height = damageHitBoxIt["height"].GetFloat();
				std::vector<glm::vec2> points =
				{
					glm::vec2(bodyTC->position.x, bodyTC->position.y),
					glm::vec2(bodyTC->position.x, bodyTC->position.y + height),
					glm::vec2(bodyTC->position.x + width, bodyTC->position.y + height),
					glm::vec2(bodyTC->position.x + width, bodyTC->position.y),
				};
				damageHitBox = new Rectangle(points);
			}

			PC = new PhysicComponent(walkHitBox, damageHitBox);
		}

		{
			const auto& legsRCIt = HeroObjIt["legsRenderComponent"];
			const std::string spriteName = legsRCIt["sprite"].GetString();
			const glm::vec2 size = glm::vec2(legsRCIt["width"].GetFloat(), legsRCIt["height"].GetFloat());

			const glm::vec2 rotationPoint = legsRCIt.HasMember("rotationPointX") ?
				glm::vec2(legsRCIt["rotationPointX"].GetFloat(), legsRCIt["rotationPointY"].GetFloat()) :
				glm::vec2(size.x / 2, size.y / 2);

			const glm::vec2 pivotOffset = legsRCIt.HasMember("pivotOffsetX") ?
				glm::vec2(legsRCIt["pivotOffsetX"].GetFloat(), legsRCIt["pivotOffsetY"].GetFloat()) :
				glm::vec2(size.x / 2, size.y / 2);

			legsRC = new RenderComponent(spriteName, size, rotationPoint, pivotOffset);
		}

		{
			auto bodyStatesRCIt = HeroObjIt.FindMember("bodyStatesRenderComponents");
			for (const auto& currentBodyRC : bodyStatesRCIt->value.GetArray())
			{
				const std::string state = currentBodyRC["state"].GetString();
				const std::string spriteName = currentBodyRC["sprite"].GetString();
				const glm::vec2 size = glm::vec2(currentBodyRC["width"].GetFloat(), currentBodyRC["height"].GetFloat());

				const glm::vec2 rotationPoint = currentBodyRC.HasMember("rotationPointX") ?
					glm::vec2(currentBodyRC["rotationPointX"].GetFloat(), currentBodyRC["rotationPointY"].GetFloat()) :
					glm::vec2(size.x / 2, size.y / 2);

				const glm::vec2 pivotOffset = currentBodyRC.HasMember("pivotOffsetX") ?
					glm::vec2(currentBodyRC["pivotOffsetX"].GetFloat(), currentBodyRC["pivotOffsetY"].GetFloat()) :
					glm::vec2(size.x / 2, size.y / 2);
				bodyStatesRC.emplace(state, new RenderComponent(spriteName, size, rotationPoint, pivotOffset));
			}
		}

		hero = new Hero(
			const_cast<TransformComponent* const>(bodyTC),
			const_cast<TransformComponent* const>(legsTC),
			const_cast<MovableComponent* const>(MC), 
			const_cast<PhysicComponent* const>(PC), 
			const_cast<RenderComponent* const>(legsRC),
			bodyStatesRC);
	}

	bool init()
	{
		camera.init(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(2222000, 2222000), glm::vec2(3.5f, 3.5f), hero);
		cursor = new Cursor();

		auto spriteShaderProgram = resources.getShader("spriteShader");
		spriteShaderProgram->use();
		spriteShaderProgram->setMatrix("viewProjectionMat", camera.getViewProjectionMatrix());
		spriteShaderProgram->setInt("tex", 0);

		return true;
	}

	void render()
	{
		resources.getShader("spriteShader")->setMatrix("viewProjectionMat", camera.getViewProjectionMatrix());

		hero->render();
		cursor->render();

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			(*it)->render();
			//std::cout << "1" << std::endl;
		}
	}

	void update(const double deltaTime) 
	{
		hero->update();

		camera.update();

		cursor->update();
	}
};