#include "SceneManager.h"

SceneManager::SceneManager(GLFWwindow* window)
{
	mShader = Shader();
	particleShader = Shader();
	mWindow = window;
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	mCamera = Camera(width, height);
	light = Light();

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

	mInputManager = InputManager(mWindow, &mCamera, &light);

	CreateModels();

	glEnable(GL_DEPTH_TEST);
}

void SceneManager::drawAll(double deltaTime)
{
	light.Render(mShader);

	GLuint currentShader = -1;
	//Models
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
	
	//Player
	mCamera.Render(mPlayer.GetShader(), deltaTime);
	glUseProgram(mPlayer.GetShader());
	mPlayer.Render(deltaTime);
	mPlayer.Update(deltaTime);

	//Spells
	bool temp = false;
	for (unsigned int i = 0; i < mSpellList.size(); i++) {
		temp = false;
		//cout << deltaTime << endl;
		for (unsigned int j = 0; j < mEntityList.size(); j++) {
			if (mSpellList[i].CheckCollusion(&mEntityList[j])) {
				temp = true;
				break;
			}
		}

		if (!temp && mSpellList[i].renderable == 1) {
			mSpellList[i].Move(deltaTime);
			mSpellList[i].Render(deltaTime);
		}

	}

	particleShader.Use();
	mCamera.Render(particleShader.GetID(), deltaTime);


	for (unsigned int i = 0; i < mSpellList.size(); i++) {

		mSpellList[i].RenderParticles(deltaTime, mCamera.c_position, mCamera.c_upVector, mCamera.c_rightVector);

	}


}

void SceneManager::UpdateScene() {

	light.Input(InputManager::mkey);

	for (unsigned int i = 0; i < mSpellList.size(); i++) {

		if (InputManager::rightFlag == 1) {
			if (InputManager::full == 0) {
				mPlayer.skillHold = true;
				if (mSpellList[i].CheckCollusion(mCamera.pointCollider)) {
					mCamera.spell = &mSpellList[i];
					InputManager::full = 1;
				}
			}
			else {
				(&mCamera)->spell->SetPosition(mCamera.pointCollider.position);
				(&mCamera)->spell->Rotate(glm::vec3(1, 1, 1), (&mCamera)->spell->theta * 180 / 3.14);
				if ((&mCamera)->spell->theta < 360.0f)
					(&mCamera)->spell->theta += 0.0001f;
				else
					(&mCamera)->spell->theta = 0.0f;
			}
		}
		//BUG ÇIKARIYO XDééééééé
		/*for (unsigned int i = 0; i < mSpellList.size(); i++) {
		//çalýþmamasý lazým ama çalýþýyo denediðim her koþulda xdé
		if (mSpellList[i].GetParticleSystemList()[1].looplife <= 0)
		mSpellList.erase(mSpellList.begin() + i);
		}*/
	}
}

void SceneManager::CreateModels()
{
	//model string
	std::string wizardStr = ModelBase + "wizard1.dae";
	std::string cloudStr = ModelBase + "cloud.obj";
	std::string rockStr = ModelBase + "rock.obj";
	std::string sceneStr = ModelBase + "completeScene.obj";
	std::string targetStr = ModelBase + "untitled.obj";
	std::string spiderStr = ModelBase + "spider.dae";
	std::string wolfStr = ModelBase + "wolf.dae";

	mPlayer = Player(wizardStr.c_str(), mSkinningShader);
	//wizard.Rotate(glm::vec3(5.0, 4.0, 5.0), -90.0f);
	mPlayer.Scale(glm::vec3(0.03f, 0.03f, 0.03f));
	mPlayer.Translate(glm::vec3(0.0f, 0.2f, 0.0f));

	mPlayer.GetModelPointer()->AddAnimationInfo("walk", 0, 4, 0.80f);
	mPlayer.GetModelPointer()->AddAnimationInfo("idle", 0, 1, 0.50f);
	mPlayer.GetModelPointer()->AddAnimationInfo("skillCast", 3, 5, 0.15f);

	//mPlayer.InitiaizeCollider(glm::vec3(0.15, 2.0, 0.3));
	//mEntityList.push_back(mPlayer);

	Entity spider = Entity(spiderStr.c_str(), mSkinningShader);
	spider.Scale(glm::vec3(0.4f, 0.4f, 0.4f));
	spider.Translate(glm::vec3(4.73, 0.0f, 0.1f));
	mEntityList.push_back(spider);

	Entity wolf = Entity(wolfStr.c_str(), mSkinningShader);
	wolf.Scale(glm::vec3(0.45f, 0.45f, 0.35f));
	wolf.Translate(glm::vec3(-4.4, 0.0f, 1.1f));
	wolf.Rotate(glm::vec3(0.0, 1.0f, 0.0f), 90);
	mEntityList.push_back(wolf);

	Entity target = Entity(targetStr.c_str(), mShader);
	target.Scale(glm::vec3(0.1f, 0.1f, 0.1f));
	target.Translate(glm::vec3(-0.33, 0.7f, 2.78f));
	target.InitiaizeCollider(glm::vec3(0.5, 0.5, 0.1));
	//target.Rotate(glm::vec3(0, 1, 0), 90);
	mEntityList.push_back(target);

	Entity rock = Entity(rockStr.c_str(), mShader);
	rock.Scale(glm::vec3(1, 1, 1));
	rock.Translate(glm::vec3(-3.86f, 0, -0.6f));
	rock.InitiaizeCollider(glm::vec3(0.7, 0.2, 0.8));
	mEntityList.push_back(rock);

	Entity rock2 = Entity(rockStr.c_str(), mShader);
	rock2.Scale(glm::vec3(1, 1, 1));
	rock2.Translate(glm::vec3(3.10f, 0, 0.52f));
	rock2.InitiaizeCollider(glm::vec3(0.7, 0.2, 0.8));
	mEntityList.push_back(rock2);

	Entity scene = Entity(sceneStr.c_str(), mShader);
	scene.Scale(glm::vec3(1.0, 1, 1.0));
	scene.Translate(glm::vec3(1.0, -0.2, 0));

	ParticleSystem volcano = ParticleSystem(500);
	volcano.SetBuffers(particleShader);
	volcano.maindirType = 0;
	volcano.SetStartVariables(3.0f, 10, 0.2f, glm::vec4(1, 0, 0, 1));
	volcano.SetStartPosition(glm::vec3(10.5f, 5.37f, -6.95f));
	volcano.SetGravity(true);
	volcano.positionType = 0;
	volcano.randomdir = true;
	volcano.SetTripleS(1, 0.5f, 3);
	scene.AddParticleSystem(volcano);

	ParticleSystem fire1 = ParticleSystem(50);
	fire1.SetBuffers(particleShader);
	fire1.maindirType = 0;
	fire1.SetStartVariables(1.2f, 10, 0.2f, glm::vec4(1, 0, 0, 1));
	fire1.SetStartPosition(glm::vec3(-6.11f, 0.07f, 1.23f));
	fire1.SetGravity();
	fire1.positionType = 1;
	fire1.SetTripleS(1, 0.2f, 0);
	scene.AddParticleSystem(fire1);

	ParticleSystem fire2 = ParticleSystem(30);
	fire2.SetBuffers(particleShader);
	fire2.maindirType = 0;
	fire2.SetStartVariables(1, 10, 0.1f, glm::vec4(1, 0, 1, 1));
	fire2.SetStartPosition(glm::vec3(-6.11f, 0.07f, 1.23f));
	fire2.SetGravity();
	fire2.positionType = 1;
	fire2.SetTripleS(1, 0.2f, 0);
	scene.AddParticleSystem(fire2);

	mEntityList.push_back(scene);

	std::string spell;
	glm::vec4 colour;
	for (int i = 0; i < 10; i++)
	{

		if (i < 5) {
			spell = "res/models/sphere.obj";
			colour = glm::vec4(1, 0, 0, 1);
		}
		else {
			spell = "res/models/cone.obj";
			colour = glm::vec4(0, 1, 0, 1);
		}
		Spell *fire1 = new Spell(spell.c_str(), mShader);
		fire1->Scale(glm::vec3(0.1f, 0.1f, 0.1f));
		fire1->Translate(glm::vec3(-2.0f + (float)i / 3, 0.1f, 0.0f));

		ParticleSystem sys2 = ParticleSystem();
		sys2.SetBuffers(particleShader);
		sys2.maindirType = 2;
		sys2.SetStartVariables(0.2f, 0.1f, 0.1f, colour);
		sys2.SetStartPosition(fire1->GetPosition());
		sys2.SetGravity();
		sys2.positionType = 2;
		sys2.playable = false;
		sys2.SetTripleS(1, 0.1f, 0);
		fire1->AddParticleSystem(sys2);

		ParticleSystem sys = ParticleSystem();
		sys.SetBuffers(particleShader);
		sys.maindirType = 1;
		sys.SetStartVariables(3.0f, 10, 0.2f, colour);
		sys.SetStartPosition(fire1->GetPosition());
		sys.SetGravity();
		sys.positionType = 0;
		sys.loop = false;
		sys.playable = false;
		sys.looplife = 4.0f;
		sys.SetTripleS(0.1f, 0.1f, 0);
		fire1->AddParticleSystem(sys);

		ParticleSystem sys3 = ParticleSystem(20);
		sys3.SetBuffers(particleShader);
		sys3.maindirType = 0;
		sys3.SetStartVariables(0.2f, 0.15f, 0.1f, colour);
		sys3.SetStartPosition(fire1->GetPosition());
		sys3.SetGravity();
		sys3.positionType = 2;
		sys3.SetTripleS(1, 0.1f, 0);
		fire1->AddParticleSystem(sys3);

		//fire1.SetVelocity(glm::vec3(0.5, 0.03, 0));
		mSpellList.push_back(*fire1);
	}

	mCamera.AttachPlayer(&mPlayer);

	glEnable(GL_DEPTH_TEST);
}