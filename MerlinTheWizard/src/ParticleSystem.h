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
	float circularRadius;
	glm::vec4 colour;

	//gravity
	bool gravityEnable;
	glm::vec3 gravityVec;

public:
	//maindir type: 
	/*default upwards
	type 1: spherical
	type 2: negative direction
	*/
	int maindirType;
	//position type
	/*default a point
	type 1: circle
	type 2: spherical
	*/
	int positionType;


	glm::vec3 direction;

	//particle info
	int MaxParticles = 100;
	SquareParticle* particleContainer;
	//std::vector<SquareParticle> particleContainer;


	//some variables
	float deltaTime;
	bool loop;
	int loopdisabler;
	bool playable;
	bool randomdir;
	float looplife;

	//consturctor
	ParticleSystem();
	ParticleSystem(int max);
	~ParticleSystem();
	//functions
	void Render(glm::vec3 rightVector, glm::vec3 upVector);
	void UpdateBuffers();
	void SetBuffers(Shader shader);
	void CreateParticles();
	void CreateParticlesOnce();
	void MainLoop(glm::vec3 cameraPosition);
	int FindUnusedParticle();
	void SortParticles();

	//helpers
	glm::vec3 ChooseMainDir();
	glm::vec3 ChoosePositionType();

	//setters
	void SetTripleS(float speed, float size, float spread);
	void SetStartVariables(float life, float sphericalRadius, float circularRadius, glm::vec4 colour);
	void SetStartPosition(glm::vec3 StartPosition);
	void SetGravity(bool isTrue = false, glm::vec3 gravity = glm::vec3(0, -9.81f, 0));



};

