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
	mInputManager.sceneEntityList = &mEntityList;

	CreateModels();

	glEnable(GL_DEPTH_TEST);
	
	/*ParticleSystem sys3 = ParticleSystem(300);
	sys3.SetBuffers(particleShader);
	sys3.maindirType = 0;
	sys3.SetStartVariables(2.0f, 10, 0.2f, glm::vec4(1, 0, 0, 1));
	sys3.SetStartPosition(wizard.position - glm::vec3(0, 0.5, 0));
	sys3.SetGravity(true);
	sys3.positionType = 0;
	sys3.SetTripleS(1, 0.3, 3);
	sys3.randomdir = true;
	wizard.AddParticleSystem(sys3);*/


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
}

void SceneManager::drawAll(double deltaTime)
{
	particleShader.Use();
	mCamera.Render(particleShader.GetID(), deltaTime);

	GLuint currentShader = -1;

	for (unsigned int i = 0; i < mEntityList.size(); i++) {
		if (currentShader != mEntityList[i].GetShader())
		{
			currentShader = mEntityList[i].GetShader();
			glUseProgram(currentShader);
		}
		mCamera.Render(currentShader, deltaTime);
		mEntityList[i].Render(deltaTime);
		mEntityList[i].RenderParticles(deltaTime, mCamera.c_position, mCamera.c_upVector, mCamera.c_rightVector);
	}

	bool temp = false;
	for (unsigned int i = 0; i < mSpellList.size(); i++) {
		//cout << deltaTime << endl;
		for (unsigned int j = 0; j < mEntityList.size(); j++) {
			if (mSpellList[i].CheckCollusion(&mEntityList[j])) {
				temp = true;
				continue;
			}
		}
		if (!temp) {
			mSpellList[i].Move(deltaTime);
			mSpellList[i].Render(deltaTime);
		}

	}
	for (unsigned int i = 0; i < mSpellList.size(); i++) {

		mSpellList[i].RenderParticles(deltaTime, mCamera.c_position, mCamera.c_upVector, mCamera.c_rightVector);

	}

	//for (unsigned int i = 0; i < mSpellList.size(); i++) {
	//	//çalýþmamasý lazým ama çalýþýyo denediðim her koþulda xdé
	//	if (mSpellList[i].GetParticleSystemList()[1].looplife <= 0)
	//		mSpellList.erase(mSpellList.begin() + i);
	//}
}

void SceneManager::UpdateScene() {

	Spell rock = Spell("res/models/rock.obj", mShader);
	rock.Scale(glm::vec3(0.2f, 0.2f, 0.1f));
	rock.Translate(glm::vec3(-4.9f, 0.0f, 0.0f));

	ParticleSystem sys2 = ParticleSystem();
	sys2.SetBuffers(particleShader);
	sys2.maindirType = 2;
	sys2.SetStartVariables(0.4f, 0.1f, 0.1f, glm::vec4(0.71, 0, 0, 1));
	sys2.SetStartPosition(rock.GetPosition());
	sys2.SetGravity();
	sys2.positionType = 2;
	sys2.playable = false;
	sys2.SetTripleS(2, 0.1f, 0);
	rock.AddParticleSystem(sys2);

	ParticleSystem sys = ParticleSystem();
	sys.SetBuffers(particleShader);
	sys.maindirType = 1;
	sys.SetStartVariables(3.0f, 10, 0.2f, glm::vec4(1, 0, 0, 1));
	sys.SetStartPosition(rock.GetPosition());
	sys.SetGravity();
	sys.positionType = 0;
	sys.loop = false;
	sys.playable = false;
	sys.looplife = 4.0f;
	sys.SetTripleS(0.1f, 0.1f, 0);
	rock.AddParticleSystem(sys);

	ParticleSystem sys3 = ParticleSystem(20);
	sys3.SetBuffers(particleShader);
	sys3.maindirType = 0;
	sys3.SetStartVariables(0.2f, 0.15f, 0.1f, glm::vec4(0, 1, 0, 1));
	sys3.SetStartPosition(rock.GetPosition());
	sys3.SetGravity();
	sys3.positionType = 2;
	sys3.SetTripleS(1, 0.1f, 0);
	rock.AddParticleSystem(sys3);

	rock.SetVelocity(glm::vec3(0.5, 0.03, 0));
	mSpellList.push_back(rock);
}

void SceneManager::CreateModels()
{
	//model string
	std::string wizardStr = ModelBase + "wizard.dae";
	std::string cloudStr = ModelBase + "cloud.obj";
	std::string mountainStr = ModelBase + "surface.obj";
	/*std::string wolfStr = ModelBase + "wolf1.dae";
	std::string spiderStr = ModelBase + "spider.dae";*/

	mPlayer = Player(wizardStr.c_str(), mSkinningShader);
	//wizard.Rotate(glm::vec3(5.0, 4.0, 5.0), -90.0f);
	mPlayer.Scale(glm::vec3(0.03f, 0.03f, 0.03f));
	mPlayer.Translate(glm::vec3(0.0f, 0.52f, 0.3f));
	mPlayer.InitiaizeCollider(glm::vec3(0.15, 2.0, 0.3));
	mEntityList.push_back(mPlayer);


	Entity cloud = Entity(cloudStr.c_str(), mShader);
	cloud.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud.Translate(glm::vec3(-0.6, 3.0f, 0.0f));
	mEntityList.push_back(cloud);

	Entity cloud1 = Entity(cloudStr.c_str(), mShader);
	cloud1.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud1.Translate(glm::vec3(0.0, 3.0f, 0.0f));
	mEntityList.push_back(cloud1);

	Entity cloud2 = Entity(cloudStr.c_str(), mShader);
	cloud2.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud2.Translate(glm::vec3(0.6, 3.0f, 0.0f));
	mEntityList.push_back(cloud2);

	Entity mountain = Entity(mountainStr.c_str(), mShader);
	mountain.Scale(glm::vec3(1.0, 0.1, 1.0));
	mEntityList.push_back(mountain);

	mCamera.AttachPlayer(&mEntityList[0]);

	//UpdateScene();
	glEnable(GL_DEPTH_TEST);
}