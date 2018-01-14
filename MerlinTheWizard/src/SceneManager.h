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
	void drawAll(double deltaTime);
	void CreateModels();
private:
	std::string ShaderBase = "res/shaders/";
	std::string ModelBase = "res/models/";
	Shader mShader;
	Shader particleShader;
	Shader mSkinningShader;
	std::vector<Entity> mEntityList;
	std::vector<ParticleSystem> mParticleSystemList;
	InputManager mInputManager;
	Camera mCamera;
	GLFWwindow* mWindow;
};

