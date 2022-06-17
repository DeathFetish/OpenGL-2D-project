#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class InputHandler
{
private:
	static std::vector<bool> mouseButtons;
	static std::vector<bool> keys;
	static glm::vec2 mouseCoord;

public:
	static void init();

	static void setKey(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void setMouseButton(GLFWwindow* window, int button, int action, int mode);
	static void setMouseCoord(GLFWwindow* window, double x, double y);

	static const std::vector<bool>& getKeys();
	static const std::vector<bool>& getMouseButtons();
	static const glm::vec2& getMousePoint();
};

