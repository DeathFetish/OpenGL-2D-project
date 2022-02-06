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

class Game
{
private:
	std::vector<bool> mouseButtons;
	std::vector<bool> keys;
	glm::vec2 mouseCoord;
	
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	Camera* camera;
	Hero* hero;

	Game() 
	{
		keys.resize(348);
		std::fill(keys.begin(), keys.end(), false);

		mouseButtons.resize(10);
		std::fill(keys.begin(), keys.end(), false);
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

		hero = new Hero();
		camera = new Camera(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(2000, 2000), glm::vec2(3, 3), hero);

		spriteShaderProgram->use();
		spriteShaderProgram->setMatrix("viewProjectionMat", camera->getViewProjectionMatrix());
		spriteShaderProgram->setInt("tex", 0);
		
		return true;
	}

	void render() 
	{
		resources.getShader("spriteShader")->setMatrix("viewProjectionMat", camera->getViewProjectionMatrix());
		hero->render();
	}

	void update(const double deltaTime) 
	{
		hero->update(deltaTime, keys, mouseButtons, camera->screenToWorldPoint(mouseCoord.x, mouseCoord.y));
		camera->update();
	}

	void setKey(const int key, const int action) { if (key > 0) keys[key] = action; }
	void setMouseButton(const int button, const int action) { mouseButtons[button] = action; }
	void setMouseCoord(const double x, const double y) { mouseCoord = glm::vec2(x, y); }
};