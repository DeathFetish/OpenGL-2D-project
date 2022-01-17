#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <chrono>

#include <GLFW/glfw3.h>

#include "Resources/WindowManager.h"
#include "Resources/ResourceManager.h"

#include "Engine/Camera.h"
#include "Game/Game.h"

using namespace std;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	game.setKey(key, action);
}

void CursorPosCallback(GLFWwindow* window, double x, double y)
{
	game.setMouseCoord(x, y);
}

int main()
{
	if (!windowManger.init())
		return -1;

	if (!game.init())
		return -1;

	glfwSetKeyCallback(windowManger.window, keyCallback);
	glfwSetCursorPosCallback(windowManger.window, CursorPosCallback);

	glClearColor(0.29f, 0.0f, 0.51f, 0.0f);

	auto previousTime = chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(windowManger.window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		auto currentTime = chrono::high_resolution_clock::now();
		uint64_t deltaTime = chrono::duration_cast<chrono::nanoseconds>(currentTime - previousTime).count();
		previousTime = currentTime;

		game.update(deltaTime);

		game.render();
		
		for (long long i = 0; i < 199999; ++i);

		glfwSwapBuffers(windowManger.window);
		glfwPollEvents();
	}

	resources.deleteAllResources();
	windowManger.quit();
	return 0;
}