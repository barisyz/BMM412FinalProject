#include "Entity.h"


class Spell:public Entity {

public:
	//call super class contructor
	Spell(const char *fileName, Shader shader, Shader particleShader):Entity(fileName, shader, particleShader) {
		collider.position = this->position;
		collider.size = glm::vec3(0.1, 0.1, 0.1);
	}

	void Move(double deltaTime);
	void SetVelocity(glm::vec3 velocity);
	bool CheckCollusion(Entity *entity);

private:
	glm::vec3 velocity;
};