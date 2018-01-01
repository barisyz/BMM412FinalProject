#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include "SceneManager.h"

using namespace glm;

class Renderer
{
public:
	Renderer(GLFWwindow* window);
	~Renderer();
	void RenderScene();
private:
	SceneManager mSceneManager;
};

