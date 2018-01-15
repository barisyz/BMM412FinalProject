#include "Entity.h"

Entity::Entity()
{
	collider.position = GetPosition();
	collider.size = glm::vec3(0.1, 0.1, 0.1);
	collider.initialized = false;

	mShader = Shader();
	mModel = Model();
	mTranformation = { glm::mat4(), glm::mat4(), glm::mat4() };
	mModelMatrix = glm::mat4();
}

Entity::Entity(const char * modelPath, Shader shader)
{
	collider.position = GetPosition();
	collider.size = glm::vec3(0.1, 0.1, 0.1);
	collider.initialized = false;
	mShader = shader;
	mTranformation = { glm::mat4(), glm::mat4(), glm::mat4() };
	mModelMatrix = glm::mat4();
	mModel = Model(modelPath);
}

//Entity::~Entity()
//{
//}

void Entity::Render(double deltaTime)
{
	mShader.Use();
	mModelMatrix = mTranformation.TranslationMatrix * mTranformation.RotationMatrix * mTranformation.ScalingMatrix;
	glUniformMatrix4fv(glGetUniformLocation(mShader.GetID(), "ModelMatrix"), 1, GL_FALSE, &mModelMatrix[0][0]);
	mModel.Draw(mShader, deltaTime);
}

void Entity::RenderParticles(double deltatime, glm::vec3 cameraPos, glm::vec3 cameraUp, glm::vec3 cameraRight)
{
	for (unsigned int i = 0; i < this->particleSystemList.size(); i++) {

		if (this->particleSystemList[i].playable) {

			this->particleSystemList[i].deltaTime = deltatime;

			if (this->particleSystemList[i].loop)
				this->particleSystemList[i].CreateParticles();
			else if (!this->particleSystemList[i].loop && this->particleSystemList[i].loopdisabler == 0) {
				this->particleSystemList[i].CreateParticlesOnce();
				this->particleSystemList[i].loopdisabler = 1;
			}

			this->particleSystemList[i].MainLoop(cameraPos);
			this->particleSystemList[i].Render(cameraRight, cameraUp);
		}
	}
}

void Entity::SetParticleShader(Shader particleShader)
{
	mParticleShader = particleShader;
}

Shader Entity::GetParticleShader()
{
	return mParticleShader;
}

glm::mat4 Entity::GetModelMatrix()
{
	return mModelMatrix;
}

glm::vec3 Entity::GetPosition()
{
	return mPosition;
}

void Entity::SetPosition(glm::vec3 position)
{
	mPosition = position;
}

glm::vec3 Entity::GetScale()
{
	return mScale;
}

void Entity::SetScale(glm::vec3 scale)
{
	mScale = scale;
}

glm::vec3 Entity::GetRotation()
{
	return mRotation;
}

void Entity::SetRotation(glm::vec3 rotation)
{
	mRotation = rotation;
}

Model Entity::GetModel()
{
	return mModel;
}

void Entity::Translate(glm::vec3 translateVec)
{
	mTranformation.TranslationMatrix = glm::translate(glm::mat4(), translateVec);
	SetPosition(translateVec);
	collider.position = translateVec;
	mModelMatrix = glm::translate(mModelMatrix, translateVec);
}

void Entity::Rotate(glm::vec3 rotateVec, float angle)
{
	mTranformation.RotationMatrix = glm::rotate(glm::mat4(), angle, rotateVec);
	SetRotation(rotateVec * angle);
	//mModelMatrix = glm::rotate(mModelMatrix, angle, rotateVec);
}

void Entity::Scale(glm::vec3 scaleVec)
{
	mTranformation.ScalingMatrix = glm::scale(glm::mat4(), scaleVec);
	SetScale(scaleVec);
	//mModelMatrix = glm::scale(mModelMatrix, scaleVec);
}

void Entity::AddParticleSystem(ParticleSystem sys) {

	this->particleSystemList.push_back(sys);
}

GLuint Entity::GetShader()
{
	return mShader.GetID();
}

std::vector<ParticleSystem> Entity::GetParticleSystemList() {
	return this->particleSystemList;
}

void Entity::InitiaizeCollider(glm::vec3 scale, bool initialized) {

	this->collider.size = scale;
	this->collider.position = GetPosition();
	this->collider.initialized = initialized;
}

