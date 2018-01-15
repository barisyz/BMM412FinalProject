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
	wizard.Translate(glm::vec3(4.0f, 0.0f, -0.0f));
	wizard.InitiaizeCollider(glm::vec3(0.15, 2.0, 0.3));
	mEntityList.push_back(wizard);

	Entity rock = Entity("models/rock.obj", mShader, particleShader);
	rock.Scale(glm::vec3(0.8f, 0.8f, 0.8f));
	rock.Translate(glm::vec3(3, 0.1f, 0.0f));
	rock.InitiaizeCollider(glm::vec3(0.8,0.8,0.8));
	mEntityList.push_back(rock);

	Entity cloud = Entity("models/cloud.obj", mShader, particleShader);
	cloud.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud.Translate(glm::vec3(-0.6, 3.0f, 0.0f));
	mEntityList.push_back(cloud);

	Entity cloud1 = Entity("models/cloud.obj", mShader, particleShader);
	cloud1.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud1.Translate(glm::vec3(0.0, 3.0f, 0.0f));
	mEntityList.push_back(cloud1);

	Entity cloud2 = Entity("models/cloud.obj", mShader, particleShader);
	cloud2.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	cloud2.Translate(glm::vec3(0.6, 3.0f, 0.0f));
	mEntityList.push_back(cloud2);

	Entity scene = Entity("models/completeScene.obj", mShader, particleShader);
	scene.Scale(glm::vec3(1, 1, 1));
	scene.Translate(glm::vec3(1.5, -0.2, 0));
	mEntityList.push_back(scene);

	UpdateScene();

	glEnable(GL_DEPTH_TEST);
}

void SceneManager::drawAll(double deltaTime)
{
	particleShader.Use();
	mCamera.Render(particleShader, deltaTime);

	mShader.Use();
	mCamera.Render(mShader, deltaTime);

	for (unsigned int i = 0; i < mEntityList.size(); i++) {
		//cout << deltaTime << endl;
		//mEntityList[i].Move(deltaTime);
		mEntityList[i].Render();
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
		if (InputManager::rightFlag == 1){
				if (InputManager::full == 0) {
					if (mSpellList[i].CheckCollusion(mCamera.pointCollider)) {
						mCamera.spell = &mSpellList[i];
						InputManager::full = 1;
					}
				}
				else {	
					(&mCamera)->spell->position = mCamera.pointCollider.position;					
				}		
		}

		if (!temp) {
			mSpellList[i].Move(deltaTime);
			mSpellList[i].Render();
		}

	}
	for (unsigned int i = 0; i < mSpellList.size(); i++) {

		mSpellList[i].RenderParticles(deltaTime, mCamera.c_position, mCamera.c_upVector, mCamera.c_rightVector);

	}

	for (unsigned int i = 0; i < mSpellList.size(); i++) {
		//çalýþmamasý lazým ama çalýþýyo denediðim her koþulda xdé
		if (mSpellList[i].GetParticleSystemList()[1].looplife <= 0)
			mSpellList.erase(mSpellList.begin() + i);

	}


}

void SceneManager::UpdateScene() {


	Spell fire1 = Spell("models/sphere.obj", mShader, particleShader);
	fire1.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	fire1.Translate(glm::vec3(1.0f, 0.1f, 0.0f));

	ParticleSystem sys2 = ParticleSystem();
	sys2.SetBuffers(particleShader);
	sys2.maindirType = 2;
	sys2.SetStartVariables(0.4f, 0.1f, 0.1f, glm::vec4(0.71, 0, 0, 1));
	sys2.SetStartPosition(fire1.position);
	sys2.SetGravity();
	sys2.positionType = 2;
	sys2.playable = false;
	sys2.SetTripleS(2, 0.1f, 0);
	fire1.AddParticleSystem(sys2);

	ParticleSystem sys = ParticleSystem();
	sys.SetBuffers(particleShader);
	sys.maindirType = 1;
	sys.SetStartVariables(3.0f, 10, 0.2f, glm::vec4(1, 0, 0, 1));
	sys.SetStartPosition(fire1.position);
	sys.SetGravity();
	sys.positionType = 0;
	sys.loop = false;
	sys.playable = false;
	sys.looplife = 4.0f;
	sys.SetTripleS(0.1f, 0.1f, 0);
	fire1.AddParticleSystem(sys);

	ParticleSystem sys3 = ParticleSystem(20);
	sys3.SetBuffers(particleShader);
	sys3.maindirType = 0;
	sys3.SetStartVariables(0.2f, 0.15f, 0.1f, glm::vec4(1, 0, 0, 1));
	sys3.SetStartPosition(fire1.position);
	sys3.SetGravity();
	sys3.positionType = 2;
	sys3.SetTripleS(1, 0.1f, 0);
	fire1.AddParticleSystem(sys3);

	//rock.SetVelocity(glm::vec3(0.5, 0.03, 0));
	mSpellList.push_back(fire1);
}