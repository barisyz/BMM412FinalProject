#include <glm\ext.hpp>
#include "GL\glew.h"

struct Particle
{
	glm::vec3 position;
	glm::vec3 speed;
	glm::vec4 colour;
	float life; //remaining lifetime
	float size, angle, weight;
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class SquareParticle: public Particle {

public:
	glm::vec3 data[4];


	SquareParticle() {
		// data of the 4 vertices of the particles.
		data[0] = glm::vec3(-0.5f, -0.5f, 0.0f);
		data[1] = glm::vec3(-0.5f, 0.5f, 0.0f);
		data[2] = glm::vec3(0.5f, 0.5f, 0.0f);
		data[3] = glm::vec3(0.5f, -0.5f, 0.0f);
	}



};
