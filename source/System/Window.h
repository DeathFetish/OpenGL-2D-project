#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class Window
{
public:
	static GLFWwindow* window;
	static glm::vec2 size;

	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

	static void init(const glm::vec2& windowSize);

	static void quit();

	static void setClearColor(const float r, const float g, const float b, const float a);

	static void clear();
};
