#pragma once
#include <array>
#include <vector>
#include <algorithm>

#include <glm/vec3.hpp>

#include "Managers/ResourceManager.h"
#include "Renderer/Camera.h"

#include "Renderer/SpriteAnimator.h"

#include "Hero.h"

#define game Game::get()

using std::vector;


class Game
{
private:
	std::vector<bool> keys;
	glm::vec2 mouseCoord;
	
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera* camera;
	Hero* hero;

	Game() 
	{
		keys.resize(349);
		fill(keys.begin(), keys.end(), false);
	}
	
	~Game() 
	{
		delete camera;
		delete hero;
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
		auto spriteShaderProgram = resources.getShader("spriteShader");
		if (!spriteShaderProgram)
			return false;


		camera = new Camera(glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec2(100, 100), glm::vec2(3, 3));
		hero = new Hero();

		spriteShaderProgram->use();
		spriteShaderProgram->setMatrix("view_projection_mat", camera->getViewProjectionMatrix());
		spriteShaderProgram->setInt("tex", 0);
		
		return true;
	}

	void render() 
	{
		hero->render();
	}

	void update(const double deltaTime) 
	{
		resources.getShader("spriteShader")->setMatrix("view_projection_mat", camera->getViewProjectionMatrix());
		hero->update(deltaTime, keys, camera->screenToWorldPoint(mouseCoord.x, mouseCoord.y));
	}

	void setKey(const int key, const int action) { keys[key] = action; }

	void setMouseCoord(const double x, const double y) { mouseCoord = glm::vec2(x, y); }
};