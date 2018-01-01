#include "InputManager.h"

InputManager::InputManager(GLFWwindow* window, Camera* camera)
{
	mWindow = window;
	mCamera = camera;
	glfwSetWindowUserPointer(window, camera);
	glfwSetKeyCallback(window, &key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}



