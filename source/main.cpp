#include <iostream>
#include <typeinfo>
#include <vector>
#include <map>

#include "System/Window.h"
#include "System/Parser.h"
#include "System/Time.h"
#include "System/InputHandler.h"

#include "Resources/ResourceManager.h"
#include "Game/ObjectsManager.h"


int main(int argc, char** argv)
{
	Parser::setDirectoryPath(argv[0]);

	Window::init(glm::vec2(1000, 800));
	Window::setClearColor(11.f, 90.f, 88.f, 0.0f);

	InputHandler::init();

	ResourceManager::loadResources("resources\\resources.json");

	ObjectsManager::loadGameObjectsSamples("resources\\objects.json");
	ObjectsManager::loadGameObjects("resources\\levels\\1.json");

	Time::setUpdateFrequency(60.0);
	Time::start();

	auto& updateQueue = ObjectsManager::getUpdateQueue();
	for (auto it : updateQueue)
		it->init();

	while (!glfwWindowShouldClose(Window::window))
	{
		glfwPollEvents();

		Time::update();

		if (Time::ñheckFPS())
		{
			Window::clear();

			auto& updateQueue = ObjectsManager::getUpdateQueue();
			for (auto it : updateQueue)
				it->update();
			for (auto it : updateQueue)
				it->lateUpdate();
			for (auto it : updateQueue)
				it->onPreRender();

			auto& renderQueue = ObjectsManager::getRenderQueue();
			for (auto it : renderQueue)
			{
				it->update();
				it->render();
			}

			glfwSwapBuffers(Window::window);
		}
	}

	ObjectsManager::deleteGameObjects();
	ObjectsManager::deleteGameObjectsSamples();
	Window::quit();
	return 0;
}