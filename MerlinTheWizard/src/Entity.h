#pragma once
#include "Model.h"
#include "glm\mat4x4.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "ParticleSystem.h"

struct sTrasformation
{
	glm::mat4 TranslationMatrix;
	glm::mat4 RotationMatrix;
	glm::mat4 ScalingMatrix;
};

class Entity
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	Entity();
	Entity(const char * fileName, Shader shader, Shader particleShader);
	~Entity();
	void Render();
	void RenderParticles(double deltatime, glm::vec3 cameraPos, glm::vec3 cameraUp, glm::vec3 cameraRight);

	void Translate(glm::vec3 translateVec);
	void Rotate(glm::vec3 rotateVec, float angle);
	void Scale(glm::vec3 scaleVec);

	void AddParticleSystem(ParticleSystem sys);
	glm::mat4 GetModelMatrix();

private:
	Model mModel;
	glm::mat4 mModelMatrix;
	Shader mShader;
	Shader mParticleShader;

	std::vector<ParticleSystem> particleSystemList;

	sTrasformation mTranformation;
};

