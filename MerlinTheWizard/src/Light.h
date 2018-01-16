#ifndef AAA_HEADER
#define AAA_HEADER

#include "glm/ext.hpp"
#include "Shader.h"

class Light{

public:
	glm::vec4 position;
	glm::vec4 colour;

	Light();

	void ChangeColour(glm::vec4 colour);
	void Translate(glm::vec4 position);
	void Render(Shader shader);

	void Input(int key);
};

#endif