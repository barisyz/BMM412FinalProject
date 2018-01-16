#pragma once
#include "Camera.h"
#include "Light.h"
#include "GLFW\glfw3.h"
#include <iostream>

class InputManager
{
public:
	InputManager();
	InputManager(GLFWwindow* window, Camera* camera, Light* light);
	~InputManager();

	static int rightFlag;
	static int leftFlag;
	static int full;
	static int mkey;
	static int shaderInt;
private:
	GLFWwindow* mWindow;
	Camera* mCamera;
	Light *mLight;

	static void setRight(int number) {
		rightFlag = number;
	}
	static void setLeft(int number) {
		leftFlag = number;
		full = 0;
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//required for light input
		mkey = key;

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));

		if (key == GLFW_KEY_F && action == GLFW_PRESS)
		{
			cam->ToogleCamera();

			if (cam->IsInFreeMode())
			{
				cam->FollowPlayer();
			}
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			if (shaderInt == 0)
				shaderInt = 1;
			else
				shaderInt = 0;
		}

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

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			setRight(1); setLeft(0);



		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

			if (InputManager::full == 1) {
				Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));
				cam->SetVelocitySpell();
			}

			setRight(0); setLeft(1);
		}

	}
};

