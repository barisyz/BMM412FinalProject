#ifndef ENTITIY_H
#define ENTITIY_H

#include "Model.h"
#include "glm\mat4x4.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "ParticleSystem.h"
#include "Collider.h"

struct sTrasformation
{
	glm::mat4 TranslationMatrix;
	glm::mat4 RotationMatrix;
	glm::mat4 ScalingMatrix;
};

class Entity
{
public:
	std::vector<ParticleSystem> particleSystemList;

	float theta;
	CubeCollider collider;

	Entity();
	Entity(const char * fileName, Shader shader);
	//~Entity();
	void Render(double deltaTime);
	void RenderParticles(double deltatime, glm::vec3 cameraPos, glm::vec3 cameraUp, glm::vec3 cameraRight);

	void SetParticleShader(Shader particleShader);
	Shader GetParticleShader();

	void Translate(glm::vec3 translateVec);
	void Rotate(glm::vec3 rotateVec, float angle);
	void Scale(glm::vec3 scaleVec);

	virtual void SetVelocity(glm::vec3 velocity);

	void InitiaizeCollider(glm::vec3 scale, bool initialized = true);

	void TranslateWithAnimation(glm::vec3 translateVector);

	void AddParticleSystem(ParticleSystem sys);
	std::vector<ParticleSystem> GetParticleSystemList();
	GLuint GetShader();
	glm::mat4 GetModelMatrix();
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 position);
	glm::vec3 GetScale();
	void SetScale(glm::vec3 scale);
	glm::vec3 GetRotation();
	void SetRotation(glm::vec3 rotation);
	Model GetModel();
	Model *GetModelPointer();
private:
	glm::vec3 mPosition;
	glm::vec3 mScale;
	glm::vec3 mRotation;

	Model mModel;
	glm::mat4 mModelMatrix;
	Shader mShader;
	Shader mParticleShader;
	glm::vec3 velocity;


	sTrasformation mTranformation;
};

#endif // !ENTITIY_H
