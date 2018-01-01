#include "Entity.h"

Entity::Entity()
{
	mShader = Shader();
	mModel = Model();
	mTranformation = { glm::mat4(), glm::mat4(), glm::mat4()};
	mModelMatrix = glm::mat4();
}

Entity::Entity(const char * fileName, Shader shader)
{
	mShader = shader;
	mTranformation = { glm::mat4(), glm::mat4(), glm::mat4() };
	mModelMatrix = glm::mat4();
	mModel = Model(fileName);
}

Entity::~Entity()
{
}

void Entity::Render()
{
	mModelMatrix = mTranformation.TranslationMatrix * mTranformation.RotationMatrix * mTranformation.ScalingMatrix;
	glUniformMatrix4fv(glGetUniformLocation(mShader.GetID(), "ModelMatrix"), 1, GL_FALSE, &mModelMatrix[0][0]);
	mModel.Draw(mShader);
}

glm::mat4 Entity::GetModelMatrix()
{
	return mModelMatrix;
}

void Entity::Translate(glm::vec3 translateVec)
{
	mTranformation.TranslationMatrix = glm::translate(glm::mat4(), translateVec);

	//mModelMatrix = mglm::translate(mModelMatrix, translateVec);
}

void Entity::Rotate(glm::vec3 rotateVec, float angle)
{
	mTranformation.RotationMatrix = glm::rotate(glm::mat4(), angle, rotateVec);
	//mModelMatrix = glm::rotate(mModelMatrix, angle, rotateVec);
}

void Entity::Scale(glm::vec3 scaleVec)
{
	mTranformation.ScalingMatrix = glm::scale(glm::mat4(), scaleVec);
	//mModelMatrix = glm::scale(mModelMatrix, scaleVec);
}
