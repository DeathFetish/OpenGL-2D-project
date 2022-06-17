#include "Window.h"

void Window::init(const glm::vec2& windowSize)
{
	if (!glfwInit())
	{
		std::cout << "Error! GLFW initialization failed." << std::endl;
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	size = windowSize;
	window = glfwCreateWindow(size.x, size.y, "OpenGL-Project", nullptr, nullptr);

	/*window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
		glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "DeathWish",
		glfwGetPrimaryMonitor(), nullptr);
	screenSize = glm::vec2(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height);*/

	if (!window)
	{
		glfwTerminate();
		std::cout << "Error! GLFW window creating failed." << std::endl;
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (!gladLoadGL())
	{
		std::cout << "Error! GLAD initialization failed." << std::endl;
		exit(-1);
	}

	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::quit()
{
	glfwTerminate();
}

void Window::setClearColor(const float r, const float g, const float b, const float a)
{
	glClearColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void Window::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

GLFWwindow* Window::window;
glm::vec2 Window::size;