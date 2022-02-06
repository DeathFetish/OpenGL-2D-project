#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include "../Renderer/Renderer.h"

#define windowManger WindowManger::get()


class WindowManger final
{
private:
	WindowManger() = default;
	~WindowManger() = default;
	
public:
	GLFWwindow* window;
	glm::vec2 screenSize = glm::vec2(1200, 800);

	WindowManger(const WindowManger&) = delete;
	WindowManger& operator = (const WindowManger&) = delete;

	static WindowManger& get()
	{
		static WindowManger object;
		return object;
	}

	bool init()
	{
		if (!glfwInit())
		{
			std::cout << "glfwInit failed" << std::endl;
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		window = glfwCreateWindow(screenSize.x, screenSize.y, "DeathWish", nullptr, nullptr);

		/*window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
			glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "DeathWish",
			glfwGetPrimaryMonitor(), nullptr);
		screenSize = glm::vec2(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height);*/

		if (!window)
		{
			glfwTerminate();
			std::cout << "glfwCreateWindow failed" << std::endl;
			return false;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGL())
		{
			std::cout << "gladLoadGL failed" << std::endl;
			return false;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		return true;
	}

	void quit()	{ glfwTerminate(); }
};