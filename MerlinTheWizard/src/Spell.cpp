#include "Spell.h"


void Spell::Move(double deltaTime) {
	glm::vec3 pos = GetPosition();
	if (pos.y < 5 && pos.y > 0 && sqrt(pos.x * pos.x + pos.z * pos.z) < 3.75) {

		pos += velocity * deltaTime;
		this->collider.position = pos;

		SetPosition(pos);
		Translate(pos);

		for (unsigned i = 0; i < particleSystemList.size(); i++)
		{
			particleSystemList[i].SetStartPosition(pos);
			particleSystemList[i].direction = -velocity * deltaTime;
		}

		return;
	}
	this->renderable = 0;

	particleSystemList[0].playable = false;
	particleSystemList[1].playable = true;
	particleSystemList[2].playable = false;
}

void Spell::SetVelocity(glm::vec3 velocity) {

	if (this->renderable == 1) {
		particleSystemList[0].playable = true;
		particleSystemList[1].playable = false;
		particleSystemList[2].playable = false;

		this->velocity = velocity;
	}



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
			if (abs(collider.position.z - obj2.position.z) < collider.size.z + obj2.size.z)
			{
				this->renderable = 0;

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
