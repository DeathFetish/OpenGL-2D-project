#include "InputHandler.h"
#include "Window.h"

void InputHandler::init()
{
	glfwSetKeyCallback(Window::window, setKey);
	glfwSetCursorPosCallback(Window::window, setMouseCoord);
	glfwSetMouseButtonCallback(Window::window, setMouseButton);

	keys.resize(348);
	std::fill(keys.begin(), keys.end(), false);

	mouseButtons.resize(10);
	std::fill(keys.begin(), keys.end(), false);
}


void InputHandler::setKey(GLFWwindow* window, int key, int scancode, int action, int mode) 
{ 
	if (key > 0)
		keys[key] = action;
}

void InputHandler::setMouseButton(GLFWwindow* window, int button, int action, int mode) 
{ 
	mouseButtons[button] = action; 
}

void InputHandler::setMouseCoord(GLFWwindow* window, double x, double y) 
{ 
	mouseCoord = glm::vec2(x, Window::size.y - y);
}

const std::vector<bool>& InputHandler::getKeys() { return keys; }
const std::vector<bool>& InputHandler::getMouseButtons() { return mouseButtons; }
const glm::vec2& InputHandler::getMousePoint() { return mouseCoord; }

std::vector<bool> InputHandler::mouseButtons;
std::vector<bool> InputHandler::keys;
glm::vec2 InputHandler::mouseCoord;