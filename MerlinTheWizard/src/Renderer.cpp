#include "Renderer.h"

Renderer::Renderer(GLFWwindow* window)
{
	glewInit();

	mSceneManager = SceneManager(window);

	mSceneManager.CreateScene();
}

Renderer::~Renderer()
{
}

void Renderer::RenderScene(double deltatime)
{
	glClearColor(0.25f, 0.67f, 0.96f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mSceneManager.drawAll(deltatime);
}
