#pragma once
#include <vector>
#include <glm/vec2.hpp>

#define gameHandler GameHandler::get()

class GameHandler
{
private:
	GameHandler() 
	{
		keys.resize(348);
		std::fill(keys.begin(), keys.end(), false);

		mouseButtons.resize(10);
		std::fill(keys.begin(), keys.end(), false);
	}
	~GameHandler()
	{
		keys.clear();
		mouseButtons.clear();
	}

	std::vector<bool> mouseButtons;
	std::vector<bool> keys;
	glm::vec2 mouseCoord;

public:

	GameHandler(const GameHandler&) = delete;
	GameHandler& operator = (const GameHandler&) = delete;

	static GameHandler& get()
	{
		static GameHandler object;
		return object;
	}

	void setKey(const int key, const int action) { if (key > 0) keys[key] = action; }
	void setMouseButton(const int button, const int action) { mouseButtons[button] = action; }
	void setMouseCoord(const double x, const double y) { mouseCoord = glm::vec2(x, y); }

	const std::vector<bool>& getKeys() const { return keys; }
	const std::vector<bool>& getMouseButtons() const { return mouseButtons; }
	const glm::vec2& getMousePoint() const { return mouseCoord; }
};