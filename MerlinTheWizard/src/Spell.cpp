#include "Spell.h"


void Spell::Move(double deltaTime) {

	if (position.y < 5 && position.y > 0 && sqrt(position.x * position.x + position.z * position.z) < 3.75) {

		this->position += this->velocity * deltaTime;
		this->collider.position = this->position;

		Translate(this->position);

		for (unsigned i = 0; i < particleSystemList.size(); i++)
		{
			particleSystemList[i].SetStartPosition(this->position);
			particleSystemList[i].direction = -this->velocity * deltaTime;
		}
		return;
	}

	mModel.~Model();

	particleSystemList[0].playable = false;
	particleSystemList[1].playable = true;
	particleSystemList[2].playable = false;
}

void Spell::SetVelocity(glm::vec3 velocity) {

	particleSystemList[0].playable = true;
	particleSystemList[1].playable = false;
	particleSystemList[2].playable = false;
	this->velocity = velocity;
}

bool Spell::CheckCollusion(Entity *entity) {
	CubeCollider obj2 = (*entity).collider;
	if (obj2.initialized == false)
		return false;
	//check the X axis
	if (abs(collider.position.x - obj2.position.x) < collider.size.x + obj2.size.x)
	{
		//check the Y axis
		if (abs(collider.position.y - obj2.position.y) < collider.size.y + obj2.size.y)
		{
			//check the Z axis
			if (abs(collider.position.z - obj2.position.z) < collider.size.z  + obj2.size.z)
			{
				mModel.~Model();

				particleSystemList[0].playable = false;
				particleSystemList[1].playable = true;
				particleSystemList[2].playable = false;
				return true;
			}
		}
	}

	return false;
}

bool Spell::CheckCollusion(CubeCollider obj2) {

	if (obj2.initialized == false)
		return false;
	//check the X axis
	if (abs(collider.position.x - obj2.position.x) < collider.size.x + obj2.size.x)
	{
		//check the Y axis
		if (abs(collider.position.y - obj2.position.y) < collider.size.y + obj2.size.y)
		{
			//check the Z axis
			if (abs(collider.position.z - obj2.position.z) < collider.size.z + obj2.size.z)
			{
				return true;
			}
		}
	}

	return false;
}