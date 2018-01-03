#include "SceneManager.h"

SceneManager::SceneManager(GLFWwindow* window)
{
	mShader = Shader();
	particleShader = Shader();
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
	std::string vertexShaderPath = ShaderBase + "/BasicVertex.glsl";
	std::string fragmentShaderPath = ShaderBase + "/FragmentShader.glsl";

	std::string particleVertexShaderPath = ShaderBase + "/particle.vert";
	std::string particleFragmentShaderPath = ShaderBase + "/particle.frag";

	mShader.LoadShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	particleShader.LoadShader(particleVertexShaderPath.c_str(), particleFragmentShaderPath.c_str());

	mInputManager = InputManager(mWindow, &mCamera);

	Entity wizard = Entity("models/wizard.obj", mShader, particleShader);
	wizard.Scale(glm::vec3(0.05f, 0.05f, 0.05f));
	wizard.Translate(glm::vec3(0.0f, 2.0f, -0.2f));
	mEntityList.push_back(wizard);

	Entity cloud = Entity("models/cloud.obj", mShader, particleShader);
	cloud.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud.Translate(glm::vec3(-0.6, 3.0f, 0.0f));

	ParticleSystem sys = ParticleSystem();
	sys.SetBuffers(particleShader);
	sys.maindirType = 1;
	sys.SetStartVariables(0.5f, 10, 0.2f, glm::vec4(1, 0, 0, 1));
	sys.SetStartPosition(cloud.position);
	sys.positionType = 0;
	sys.SetTripleS(0.1, 0.1, 0);
	cloud.AddParticleSystem(sys);

	mEntityList.push_back(cloud);

	Entity cloud1 = Entity("models/cloud.obj", mShader, particleShader);
	cloud1.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud1.Translate(glm::vec3(0.0, 3.0f, 0.0f));
	mEntityList.push_back(cloud1);

	Entity cloud2 = Entity("models/cloud.obj", mShader, particleShader);
	cloud2.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud2.Translate(glm::vec3(0.6, 3.0f, 0.0f));

	ParticleSystem sys2 = ParticleSystem();
	sys2.SetBuffers(particleShader);
	sys2.maindirType = 1;
	sys2.SetStartVariables(0.5f, 10, 0.2f, glm::vec4(1, 1, 0, 1));
	sys2.SetStartPosition(cloud2.position);
	sys2.positionType = 0;
	sys2.SetTripleS(0.1, 0.1, 0);
	cloud2.AddParticleSystem(sys2);

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

	Entity mountain = Entity("models/part.obj", mShader, particleShader);
	mountain.Scale(glm::vec3(0.3, 0.3, 0.3));
	mEntityList.push_back(mountain);



	glEnable(GL_DEPTH_TEST);
}

void SceneManager::drawAll(double deltaTime)
{
	particleShader.Use();
	mCamera.Render(particleShader);

	mShader.Use();
	mCamera.Render(mShader);

	for (unsigned int i = 0; i < mEntityList.size(); i++) {
		mEntityList[i].Render();
		mEntityList[i].RenderParticles(deltaTime, mCamera.c_position, mCamera.c_upVector, mCamera.c_rightVector);
	}


}


