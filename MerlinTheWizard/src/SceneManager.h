#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

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
	void UpdateScene();
	void CreateModels();
private:
	std::string ShaderBase = "res/shaders/";
	std::string ModelBase = "res/models/";
	Shader mShader;
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
#endif // !SCENE_MANAGER_H

