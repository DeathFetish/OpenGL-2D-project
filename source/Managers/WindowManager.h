#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#define windowManger WindowManger::get()

using namespace glm;

class WindowManger final
{
private:
	WindowManger() = default;
	~WindowManger() = default;
	
public:
	GLFWwindow* window;
	const vec2 screenSize = vec2(1200, 800);

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
			glfwGetPrimaryMonitor(), nullptr);*/
		
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

		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		return true;
	}

	void quit()	{ glfwTerminate(); }
};