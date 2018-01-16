#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>


class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();
	void swapBuffers();
	GLFWwindow* getWindow();
private:
	GLFWwindow* mWindow;
};

