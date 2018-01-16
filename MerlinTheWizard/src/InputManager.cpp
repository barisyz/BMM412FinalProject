#include "InputManager.h"

int InputManager::rightFlag = 0;
int InputManager::leftFlag = 0;
int InputManager::full = 0;
int InputManager::mkey = 0;
int InputManager::shaderInt = 0;
InputManager::InputManager(GLFWwindow* window, Camera* camera, Light* light)
{
	mWindow = window;
	mCamera = camera;
	mLight = light;
	glfwSetWindowUserPointer(window, camera);
	glfwSetKeyCallback(window, &key_callback);
	glfwSetCursorPosCallback(window, &mouse_callback);
	glfwSetMouseButtonCallback(window, &mouse_button_callback);

}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}



