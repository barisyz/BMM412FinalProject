#include <glm\ext.hpp>
#include "GL\glew.h"
#include "Particle.h"
#include <random>
#include <vector>
#include "Shader.h"

class ParticleSystem {
private:
	//particle info
	SquareParticle particle;
	int lastUsedParticle = 0;
	int particlesCount;

	//array and buffer objects
	GLuint vao;
	GLuint vertex_buffer, position_buffer, colour_buffer;
	GLuint shader;

	//random engines
	std::minstd_rand simple_rand;

	//particle customization
	float speedMultiplier;
	float sizeMultiplier;
	float spread; //how spread are the particles


	//start variables
	float sphericalRadius;
	float life;
	glm::vec3 startPosition;
	float startRadius;
	glm::vec4 colour;

public:
	//maindir type: 
	/*default upwards
	type 1: spherical
	*/
	int maindirType;
	//position type
	/*default a point
	type 1 circle
	*/
	int positionType;


	//particle info
	const int MaxParticles = 1000;
	SquareParticle particleContainer[1000];


	//some variables
	float deltaTime;
	bool loop;


	//consturctor
	ParticleSystem();
	//functions
	void Render(glm::vec3 rightVector, glm::vec3 upVector);
	void UpdateBuffers();
	void SetBuffers(Shader shader);
	void CreateParticles();
	void MainLoop(glm::vec3 cameraPosition);
	int FindUnusedParticle();
	void SortParticles();

	//helpers
	glm::vec3 ChooseMainDir();
	glm::vec3 ChoosePositionType();

	//setters
	void SetTripleS(float speed, float size, float spread);
	void SetStartVariables(float life, float radius, float startRadius, glm::vec4 colour);
	void SetStartPosition(glm::vec3 StartPosition);

};

