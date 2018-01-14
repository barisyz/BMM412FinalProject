#include "glm\ext.hpp"

struct CubeCollider {

	glm::vec3 position;
	float sizeMultiplier;
	glm::vec3 size;
	bool initialized;

	glm::vec3 mainData[8] = {
		glm::vec3(-0.5, -0.5, -0.5),
		glm::vec3(0.5, -0.5, -0.5),
		glm::vec3(0.5, -0.5, 0.5),
		glm::vec3(-0.5, -0.5, 0.5),
		glm::vec3(0.5, 0.5, 0.5),
		glm::vec3(0.5, 0.5, -0.5),
		glm::vec3(-0.5, 0.5, -0.5),
		glm::vec3(-0.5, 0.5, 0.5)
	};

};