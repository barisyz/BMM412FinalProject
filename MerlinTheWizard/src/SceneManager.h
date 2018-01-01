#pragma once
#include "GL/glew.h"
#include "Shader.h"
#include "InputManager.h"
#include "Entity.h"
#include "GLFW/glfw3.h"

class SceneManager
{
public:
	SceneManager(GLFWwindow* window);
	SceneManager();
	//~SceneManager();
	void CreateScene();
	void drawAll();
private:
	std::string ShaderBase = "shaders";
	Shader mShader;
	std::vector<Entity> mEntityList;
	InputManager mInputManager;
	Camera mCamera;
	GLFWwindow* mWindow;
};

