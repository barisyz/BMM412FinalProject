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
	std::string vertexShaderPath = ShaderBase + "BasicVertex.glsl";
	std::string fragmentShaderPath = ShaderBase + "FragmentShader.glsl";

	std::string skinnigVertexShaderPath = ShaderBase + "SkinningVertex.glsl";
	std::string skinningFragmentShaderPath = ShaderBase + "SkinningFrag.glsl";

	std::string particleVertexShaderPath = ShaderBase + "particle.vert";
	std::string particleFragmentShaderPath = ShaderBase + "particle.frag";

	mShader.LoadShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	mSkinningShader.LoadShader(skinnigVertexShaderPath.c_str(), skinningFragmentShaderPath.c_str());
	particleShader.LoadShader(particleVertexShaderPath.c_str(), particleFragmentShaderPath.c_str());

	mInputManager = InputManager(mWindow, &mCamera);

	CreateModels();

	/*ParticleSystem sys = ParticleSystem();
	sys.SetBuffers(particleShader);
	sys.maindirType = 1;
	sys.SetStartVariables(0.5f, 10, 0.2f, glm::vec4(1, 0, 0, 1));
	sys.SetStartPosition(mEntityList.at(1).position);
	sys.positionType = 0;
	sys.SetTripleS(0.1, 0.1, 0);
	mEntityList.at(1).AddParticleSystem(sys);*/

	/*ParticleSystem sys2 = ParticleSystem();
	sys2.SetBuffers(particleShader);
	sys2.maindirType = 1;
	sys2.SetStartVariables(0.5f, 10, 0.2f, glm::vec4(1, 1, 0, 1));
	sys2.SetStartPosition(mEntityList.at(2).position);
	sys2.positionType = 0;
	sys2.SetTripleS(0.1, 0.1, 0);
	mEntityList.at(2).AddParticleSystem(sys2);
*/
	glEnable(GL_DEPTH_TEST);
}

void SceneManager::drawAll(double deltaTime)
{
	particleShader.Use();
	mCamera.Render(particleShader.GetID());

	GLuint currentShader = -1;

	for (unsigned int i = 0; i < mEntityList.size(); i++) {
		if(currentShader != mEntityList[i].GetShader())
		{
			currentShader = mEntityList[i].GetShader();
			glUseProgram(currentShader);
		}
		mCamera.Render(currentShader);
		mEntityList[i].Render(deltaTime);
		mEntityList[i].RenderParticles(deltaTime, mCamera.c_position, mCamera.c_upVector, mCamera.c_rightVector);
	}
}


void SceneManager::CreateModels()
{
	//model string
	std::string wizardStr = ModelBase + "wizard.dae";
	std::string cloudStr = ModelBase + "cloud.obj";
	std::string mountainStr = ModelBase + "part.obj";
	/*std::string wolfStr = ModelBase + "wolf1.dae";
	std::string spiderStr = ModelBase + "spider.dae";*/

	Entity wizard = Entity(wizardStr.c_str(), mSkinningShader, particleShader);
	//wizard.Rotate(glm::vec3(5.0, 4.0, 5.0), -90.0f);
	wizard.Scale(glm::vec3(0.025f, 0.025f, 0.025f));
	wizard.Translate(glm::vec3(2.20f, 0.15f, -0.2f));
	mEntityList.push_back(wizard);

	//Entity wolf = Entity(wolfStr.c_str(), mSkinningShader, particleShader);
	////wizard.Rotate(glm::vec3(5.0, 4.0, 5.0), -90.0f);
	//wolf.Scale(glm::vec3(0.4f, 0.4f, 0.4f));
	//wolf.Translate(glm::vec3(2.3f, 0.0f, -0.2f));
	//mEntityList.push_back(wolf);

	//Entity spider = Entity(spiderStr.c_str(), mSkinningShader, particleShader);
	////wizard.Rotate(glm::vec3(5.0, 4.0, 5.0), -90.0f);
	//spider.Scale(glm::vec3(0.45f, 0.45f, 0.45f));
	//spider.Translate(glm::vec3(2.3f, 0.0f, 0.8f));
	//mEntityList.push_back(spider);

	Entity cloud = Entity(cloudStr.c_str(), mShader, particleShader);
	cloud.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud.Translate(glm::vec3(-0.6, 3.0f, 0.0f));
	mEntityList.push_back(cloud);

	Entity cloud1 = Entity(cloudStr.c_str(), mShader, particleShader);
	cloud1.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud1.Translate(glm::vec3(0.0, 3.0f, 0.0f));
	mEntityList.push_back(cloud1);

	Entity cloud2 = Entity(cloudStr.c_str(), mShader, particleShader);
	cloud2.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud2.Translate(glm::vec3(0.6, 3.0f, 0.0f));
	mEntityList.push_back(cloud2);

	Entity mountain = Entity(mountainStr.c_str(), mShader, particleShader);
	mountain.Scale(glm::vec3(0.2, 0.2, 0.2));
	mEntityList.push_back(mountain);

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
}