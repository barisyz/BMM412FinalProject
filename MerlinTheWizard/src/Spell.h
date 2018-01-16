#include "Entity.h"

class Spell : public Entity {

public:
	//call super class contructor
	Spell(const char *fileName, Shader shader) :Entity(fileName, shader) {
		collider.position = GetPosition();
		collider.size = glm::vec3(0.1, 0.1, 0.1);
		renderable = 1;
	}

	int renderable = 0;

	void Move(double deltaTime);
	void SetVelocity(glm::vec3 velocity);
	bool CheckCollusion(Entity *entity);
	bool CheckCollusion(CubeCollider obj2);

private:
	glm::vec3 velocity;
};