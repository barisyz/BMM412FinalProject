#pragma once
#include "GL/glew.h"
#include "Shader.h"
#include "InputManager.h"
#include "Entity.h"
#include "Spell.h"
#include "GLFW/glfw3.h"

class SceneManager
{
public:
	SceneManager(GLFWwindow* window);
	SceneManager();
	//~SceneManager();
	void CreateScene();
	void drawAll(double deltaTime);
	void UpdateScene();
private:
	std::string ShaderBase = "shaders";
	Shader mShader;
	Shader particleShader;
	std::vector<Entity> mEntityList;
	std::vector<Spell> mSpellList;
	InputManager mInputManager;
	Camera mCamera;
	GLFWwindow* mWindow;
};

