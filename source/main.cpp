#include <glad/glad.h>
#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>

#include "Resources/WindowManager.h"
#include "Resources/ResourceManager.h"
#include "System/Timer.h"
#include "Renderer/Renderer.h"
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

	renderer.setClearColor(0.29f, 0.0f, 0.51f, 0.0f);
	time.start();

	while (!glfwWindowShouldClose(windowManger.window))
	{
		time.update();

		if (time.CheckFPS())
		{
			game.update(time.getDeltaTime());
			renderer.clear();
			game.render();

			glfwSwapBuffers(windowManger.window);
		}
		glfwPollEvents();
	}

	resources.deleteAllResources();
	windowManger.quit();
	return 0;
}