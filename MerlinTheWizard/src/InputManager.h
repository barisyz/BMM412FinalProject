#pragma once
#include "Camera.h"
#include "GLFW\glfw3.h"
#include <iostream>

class InputManager
{
public:
	InputManager();
	InputManager(GLFWwindow* window, Camera* camera);
	~InputManager();
private:
	GLFWwindow* mWindow;
	Camera* mCamera;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));
		cam->keyboard_event(key, scancode, action, mods);
	}

	static void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

		int width = 0, height = 0;
		glfwGetWindowSize(window, &width, &height);

		xpos = float(width / 2 - xpos);
		ypos = float(height / 2 - ypos);

		Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));
		cam->mouse_event(xpos, ypos);
	}

	static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {



	}
};

