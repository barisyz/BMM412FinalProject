#include "SceneManager.h"

SceneManager::SceneManager(GLFWwindow* window)
{
	mShader = Shader();
	mWindow = window;
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	mCamera = Camera(width, height);
}

SceneManager::SceneManager()
{

}

void SceneManager::CreateScene()
{
	std::string vertexShaderPath	= ShaderBase + "/BasicVertex.glsl";
	std::string fragmentShaderPath	= ShaderBase + "/FragmentShader.glsl";

	mShader.LoadShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

	mInputManager = InputManager(mWindow, &mCamera);

	Entity wizard = Entity("models/wizard.obj", mShader);
	wizard.Scale(glm::vec3(0.05f, 0.05f, 0.05f));
	wizard.Translate(glm::vec3(0.0f, 2.0f, -0.2f));
	mEntityList.push_back(wizard);

	Entity cloud = Entity("models/cloud.obj", mShader);
	cloud.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud.Translate(glm::vec3(-0.6, 3.0f, 0.0f));
	mEntityList.push_back(cloud);

	Entity cloud1 = Entity("models/cloud.obj", mShader);
	cloud1.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud1.Translate(glm::vec3(0.0, 3.0f, 0.0f));
	mEntityList.push_back(cloud1);

	Entity cloud2 = Entity("models/cloud.obj", mShader);
	cloud2.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud2.Translate(glm::vec3(0.6, 3.0f, 0.0f));
	mEntityList.push_back(cloud2);

	//Entity tree = Entity("models/tree1.obj", mShader);
	//tree.Scale(glm::vec3(0.3f, 0.3f, 0.3f));
	//tree.Translate(glm::vec3(-0.9f, 0.0f, 0.0f));
	//mEntityList.push_back(tree);
	
	//Entity tree1 = Entity("models/tree2.obj", mShader);
	//tree1.Scale(glm::vec3(0.3f, 0.3f, 0.3f));
	//tree1.Translate(glm::vec3(0.5f, 0.0f, 0.0f));
	//mEntityList.push_back(tree1);

	//Entity surface = Entity("models/surface.obj", mShader);
	//surface.Scale(glm::vec3(1.0f, 1.0f, 0.5f));
	//surface.Translate(glm::vec3(0.0f, 0.0, 0.0f));
	//mEntityList.push_back(surface);
	
	Entity mountain = Entity("models/part.obj", mShader);
	mountain.Scale(glm::vec3(0.3, 0.3, 0.3));
	mEntityList.push_back(mountain);
	
	mShader.Use();
	glEnable(GL_DEPTH_TEST);
}

void SceneManager::drawAll()
{
	mCamera.Render(mShader);

	for (unsigned int i = 0; i <  mEntityList.size(); i++) {
		mEntityList[i].Render();
	}
}

