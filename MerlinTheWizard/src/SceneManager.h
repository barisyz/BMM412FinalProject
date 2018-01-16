#pragma once
#include "GL/glew.h"
#include "Shader.h"
#include "InputManager.h"
#include "Spell.h"
#include "GLFW/glfw3.h"
#include <fstream>
#include <string>
#include "Light.h"

class SceneManager
{
public:
	SceneManager(GLFWwindow* window);
	SceneManager();
	//~SceneManager();
	void CreateScene();
	void drawAll(double deltaTime);
	void UpdateScene(float deltaTime);
	void CreateModels();
	void ChangeShaders();
private:
	std::string ShaderBase = "res/shaders/";
	std::string ModelBase = "res/models/";
	Shader mShader;
	Shader vertexShader;
	Shader fragShader;
	Shader particleShader;
	Shader mSkinningShader;

	Light light;
	std::vector<Entity> mEntityList;
	std::vector<Spell> mSpellList;
	InputManager mInputManager;
	Camera mCamera;
	Player mPlayer;
	GLFWwindow* mWindow;
};

