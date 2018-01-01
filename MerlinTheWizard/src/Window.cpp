#include "Window.h"

void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	//todo camera frustumuda degistirilecek
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const char* title)
{
	mWindow = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!mWindow)
	{
		glfwTerminate();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwMakeContextCurrent(mWindow);
	glfwSetWindowSizeCallback(mWindow, WindowSizeCallback);

	//glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);

	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPos(mWindow, 0, 0);

	glfwSwapInterval(0);

	glfwGetFramebufferSize(mWindow, &width, &height);
	glViewport(0, 0, width, height);
	
}

Window::~Window()
{
	glfwDestroyWindow(mWindow);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(mWindow);
}

GLFWwindow * Window::getWindow()
{
	return mWindow;
}


