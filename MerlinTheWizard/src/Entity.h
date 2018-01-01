#pragma once
#include "Model.h"
#include "glm\mat4x4.hpp"
#include "glm\gtc\matrix_transform.hpp"


struct sTrasformation
{
	glm::mat4 TranslationMatrix;
	glm::mat4 RotationMatrix;
	glm::mat4 ScalingMatrix;
};

class Entity
{
public:
	Entity();
	Entity(const char * fileName, Shader shader);
	~Entity();
	void Render();
	
	void Translate(glm::vec3 translateVec);
	void Rotate(glm::vec3 rotateVec, float angle);
	void Scale(glm::vec3 scaleVec);

	glm::mat4 GetModelMatrix();

private:
	Model mModel;
	glm::mat4 mModelMatrix;
	Shader mShader;

	sTrasformation mTranformation;
};

