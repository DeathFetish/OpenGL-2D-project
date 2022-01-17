#pragma once
#include <array>
#include <vector>
#include <algorithm>

#include "Resources/ResourceManager.h"
#include "Engine/Camera.h"
#include "Map.h"
#include "Hero.h"

#define game Game::get()
using namespace std;

class Game
{
private:
	Map* map;
	Hero* hero;
	Camera* camera;

	double mouseCoordX;
	double mouseCoordY;

	vec3 cameraPosition = vec3(0.0f, 0.0f, 0.0f);
	vector<bool> keys;

	Game() 
	{
		keys.resize(349);
		fill(keys.begin(), keys.end(), false);
	}
	
	~Game() 
	{
		delete map;
		delete hero;
		delete camera;
	}

public:

	Game(const Game&) = delete;
	Game& operator = (const Game&) = delete;

	static Game& get()
	{
		static Game object;
		return object;
	}

	bool init()
	{
		auto spriteShaderProgram = resources.loadShader("vertex", "fragment", "spriteShaderProgram");
		if (!spriteShaderProgram)
			return false;

		vector<pair<string, pair<vec2, vec2>>> heroSubTextures = { make_pair("pos1", make_pair(vec2(0.f, 0.f),  vec2(36.f, 19.f))),
																   make_pair("pos2", make_pair(vec2(36.f, 0.f), vec2(72.f, 19.f))),
																   make_pair("pos3", make_pair(vec2(72.f, 0.f), vec2(108.f, 19.f))) };

		auto heroTexture = resources.loadTextureAtlas("heroTexture", "hero", heroSubTextures);
		if (!heroTexture)
			return false;

		auto heroAnimatedSprite = resources.loadAnimatedSprite("heroSprite", "heroTexture", "pos1", "spriteShaderProgram", vec2(0, 0), vec2(36, 19));
		if (!heroAnimatedSprite)
			return false;

		vector<pair<string, uint64_t>> heroAnimations = { make_pair("pos1", 1.5e8),
														  make_pair("pos2", 1.5e8),
														  make_pair("pos1", 1.5e8),
														  make_pair("pos3", 1.5e8), };

		resources.loadTexture("parquet", "parquet", GL_REPEAT);
		resources.loadTexture("vertical wall", "vertical wall", GL_REPEAT);
		resources.loadTexture("horizontal wall", "horizontal wall", GL_REPEAT);
		resources.loadTexture("wall joint", "wall joint");
		resources.loadTexture("bed", "bed");

		resources.loadSprite("parquet", "parquet", "default", "spriteShaderProgram", vec2(100, 100), vec2(30, 30));
		resources.loadSprite("vertical wall", "vertical wall", "default", "spriteShaderProgram", vec2(100, 100), vec2(10, 30));
		resources.loadSprite("horizontal wall", "horizontal wall", "default", "spriteShaderProgram", vec2(100, 90), vec2(30, 10));
		resources.loadSprite("wall joint", "wall joint", "default", "spriteShaderProgram", vec2(100 - 8, 1270 - 8), vec2(14, 14));
		resources.loadSprite("bed", "bed", "default", "spriteShaderProgram", vec2(0, 0), vec2(113, 42));

		map = new Map(vec2(0, 0), vec2(2000, 2000));
	
		map->addFloor("parquet", vec2(100, 100), vec2(1200, 1200));
		map->addWall("vertical wall", vec2(100, 100), vec2(40, 1200));
		map->addWall("vertical wall", vec2(1260, 100), vec2(40, 1200));
		map->addWall("horizontal wall", vec2(100, 90), vec2(1200, 40));
		map->addWall("horizontal wall", vec2(100, 1270), vec2(1200, 40));

		map->addDecorativeObject("wall joint", vec2(100 - 8, 1270 - 8), 0);
		map->addDecorativeObject("wall joint", vec2(100 - 8, 90 - 8), 0);
		map->addDecorativeObject("wall joint", vec2(1260 - 8, 1270 - 8), 0);
		map->addDecorativeObject("wall joint", vec2(1260 - 8, 90 - 8), 0);
	//	map->addDecorativeObject("bed", vec2(140, 500), 0);
		map->addDecorativeObject("bed", vec2(140, 500), 90);

		hero = new Hero(vec2(500, 500), vec2(36 * 4, 19 * 4));
		
		camera = new Camera(cameraPosition, map->getLeftDownMapPoint(), map->getRightTopMapPoint());

		heroAnimatedSprite->insertState("running", heroAnimations);
		heroAnimatedSprite->setState("running");

		spriteShaderProgram->use();
		spriteShaderProgram->setMatrix("view_projection_mat", camera->getViewProjectionMatrix());

		spriteShaderProgram->setInt("tex", 0);
		return true;
	}

	void render() 
	{
		map->render();
		hero->render();
	}

	void update(const uint64_t deltaTime) 
	{
		resources.getShader("spriteShaderProgram")->setMatrix("view_projection_mat", camera->getViewProjectionMatrix());
		
		hero->update(deltaTime, keys, mouseCoordX, windowManger.screenSize.y - mouseCoordY, camera, map);
		camera->update(keys, hero->getScreenPosition());
	}

	void setKey(const int key, const int action) { keys[key] = action; }

	void setMouseCoord(const double x, const double y) { mouseCoordX = x; mouseCoordY = y; }
};