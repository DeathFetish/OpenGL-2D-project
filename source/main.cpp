#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Managers/WindowManager.h"
#include "Managers/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "System/Time.h"
#include "Game/Game.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	game.setKey(key, action);
}

void CursorPosCallback(GLFWwindow* window, double x, double y)
{
	game.setMouseCoord(x, windowManger.screenSize.y - y);
}

int main(int argc, char** argv)
{
	if (!windowManger.init())
		return -1;

	glfwSetKeyCallback(windowManger.window, keyCallback);
	glfwSetCursorPosCallback(windowManger.window, CursorPosCallback);
	resources.setExecutablePath(argv[0]);
	resources.loadJSONResources("Resources\\resources.json");

	if (!game.init())
		return -1;
	
	renderer.setClearColor(0.29f, 0.0f, 0.51f, 0.0f);
	time.start();

	while (!glfwWindowShouldClose(windowManger.window))
	{
		time.update();

		if (time.сheckFPS())
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