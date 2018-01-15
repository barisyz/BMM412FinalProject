#include "ParticleSystem.h"
#include <algorithm>
#include "stdio.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//create random engines
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0, 2 * 3.14);
std::uniform_real_distribution<double> dist2(0, 3.14);


ParticleSystem::ParticleSystem() {

	particle = SquareParticle();
	particleContainer = new SquareParticle[MaxParticles];
	for (int i = 0; i<MaxParticles; i++) {
		particleContainer[i].life = -1.0f;
		particleContainer[i].cameradistance = -1.0f;
	}

	loop = true;
	playable = true;
	randomdir = false;
	loopdisabler = 0;
}

ParticleSystem::ParticleSystem(int max) {

	particle = SquareParticle();
	this->MaxParticles = max;
	particleContainer = new SquareParticle[MaxParticles];
	for (int i = 0; i<MaxParticles; i++) {
		particleContainer[i].life = -1.0f;
		particleContainer[i].cameradistance = -1.0f;
	}

	loop = true;
	loopdisabler = 0;

}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::SetBuffers(Shader shader) {



	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// The VBO containing the 4 vertices of the particles.
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle.data), particle.data, GL_STATIC_DRAW);

	GLuint vPosition = glGetAttribLocation(shader.GetID(), "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	this->vao = vao;
	this->shader = shader.GetID();


}

void ParticleSystem::UpdateBuffers() {
	//empty

}

void ParticleSystem::Render(glm::vec3 rightVector, glm::vec3 upVector) {



	glUseProgram(shader);
	glBindVertexArray(vao);

	glUniform3fv(glGetUniformLocation(shader, "CameraUp_worldspace"), 1, &rightVector[0]);
	glUniform3fv(glGetUniformLocation(shader, "CameraRight_worldspace"), 1, &upVector[0]);

	for (int i = 0; i < MaxParticles; i++)
	{
		SquareParticle& p = particleContainer[i]; // shortcut
		if (p.life > 0.0f) {

			glUniform3f(glGetUniformLocation(shader, "location"), p.position.x, p.position.y, p.position.z);
			glUniform3f(glGetUniformLocation(shader, "scale"), p.size, p.size, p.size);
			glUniform4f(glGetUniformLocation(shader, "Colour"), p.colour.r, p.colour.b, p.colour.g, p.colour.a);

			/*cout << i;
			cout << "--";
			cout << p.cameradistance << endl;*/

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}

	}


}


void ParticleSystem::MainLoop(glm::vec3 cameraPosition) {
	this->looplife -= deltaTime;
	int ParticlesCount = 0;

	for (int i = 0; i<MaxParticles; i++) {

		Particle& p = particleContainer[i]; // shortcut

		if (p.life > 0.0f) {

			// Decrease life
			p.life -= deltaTime;
			if (p.life > 0.0f) {

				// Simulate simple physics, can add size over lifetime
				if (gravityEnable)
					p.speed += this->gravityVec * (float)deltaTime;

				p.position += p.speed * (float)deltaTime;
				p.cameradistance = glm::length(p.position - cameraPosition);
				p.cameradistance *= p.cameradistance;


			}
			else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameradistance = -1.0f;
			}

			ParticlesCount++;

		}


	}

	SortParticles();

	this->particlesCount = ParticlesCount;

}

int ParticleSystem::FindUnusedParticle() {

	for (int i = lastUsedParticle; i<MaxParticles; i++) {
		if (particleContainer[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i< lastUsedParticle; i++) {
		if (particleContainer[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}
	return 0;

}

void ParticleSystem::SortParticles() {
	//sort according to camera distance (108. line)
	std::sort(&particleContainer[0], &particleContainer[MaxParticles]);

}

void ParticleSystem::CreateParticles() {

	//max particle number per frame
	int newparticles = (int)(deltaTime*1000.0);
	if (newparticles > 16)
		newparticles = 16;

	for (int i = 0; i<newparticles; i++) {

		int particleIndex = FindUnusedParticle();
		particleContainer[particleIndex].life = this->life;

		particleContainer[particleIndex].position = ChoosePositionType();

		glm::vec3 maindir = ChooseMainDir();
		//give a random direction if wanted
		if (randomdir) {
			glm::vec3 randomdir = glm::vec3(
				(simple_rand() % 2000 - 1000.0f) / 1000.0f,
				(simple_rand() % 2000 - 1000.0f) / 1000.0f,
				(simple_rand() % 2000 - 1000.0f) / 1000.0f
			);
			particleContainer[particleIndex].speed = (maindir + randomdir * this->spread) * this->speedMultiplier;
		}
		else
			particleContainer[particleIndex].speed = (maindir) * this->speedMultiplier;



		particleContainer[particleIndex].colour = colour;

		particleContainer[particleIndex].size = ((simple_rand() % 1000) / 2000.0f) * this->sizeMultiplier;

	}


}

void ParticleSystem::CreateParticlesOnce() {

	for (int i = 0; i<MaxParticles; i++) {

		particleContainer[i].life = this->life;

		particleContainer[i].position = ChoosePositionType();

		glm::vec3 maindir = ChooseMainDir();
		//give a random direction
		if (randomdir) {
			glm::vec3 randomdir = glm::vec3(
				(simple_rand() % 2000 - 1000.0f) / 1000.0f,
				(simple_rand() % 2000 - 1000.0f) / 1000.0f,
				(simple_rand() % 2000 - 1000.0f) / 1000.0f
			);

			particleContainer[i].speed = (maindir + randomdir * this->spread) * this->speedMultiplier;
		}
		else
			particleContainer[i].speed = (maindir) * this->speedMultiplier;

		particleContainer[i].colour = colour;

		particleContainer[i].size = ((simple_rand() % 1000) / 2000.0f) * this->sizeMultiplier;

		/*	cout << particleContainer[i].position.x; cout << " ";
		cout << particleContainer[i].position.y; cout << " ";
		cout << particleContainer[i].position.z << endl;*/
	}

}

glm::vec3 ParticleSystem::ChooseMainDir() {

	glm::vec3 temp;
	switch (this->maindirType)
	{
	case 0:
		temp = glm::vec3(0, 1, 0);
		break;
	case 1:
	{
		float a = dist(mt);
		float b = dist2(mt);
		float x = sphericalRadius * cos(a);
		float y = sphericalRadius * sin(a);

		float z = sphericalRadius * cos(b);
		x *= sin(b);
		y *= sin(b);

		temp = glm::vec3(x, y, z);
		break;
	}
	case 2:
		temp = this->direction * 100;
		break;

	}

	return temp;
}

glm::vec3 ParticleSystem::ChoosePositionType() {

	switch (this->positionType)
	{
	case 1:
	{
		std::uniform_real_distribution<double> dist3(0, circularRadius);
		float a = dist(mt);
		float x = dist3(mt) * cos(a);
		float z = dist3(mt) * sin(a);

		glm::vec3 temp = glm::vec3(x, 0, z);

		if (direction.x == -0.0f && direction.y == -0.0f && direction.z == -0.0f) {

			return temp + startPosition;
		}
		else {
			float b = glm::dot(glm::normalize(direction), glm::vec3(0, 1, 0));
			float angle = glm::acos(b);


			glm::vec3 normal = glm::cross(glm::normalize(direction), glm::vec3(0, 1, 0));
			normal = glm::normalize(normal);

			glm::vec3 temp2;
			if (normal.x == normal.x) {

				temp2 = glm::rotate(temp, angle, normal);
			}
			else {
				temp2 = glm::rotate(temp, angle, glm::vec3(0, 1, 0));
			}


			return temp2 + startPosition;
		}

		break;
	}
	case 2: {
		float a = dist(mt);
		float b = dist2(mt);
		float x = sphericalRadius * cos(a);
		float y = sphericalRadius * sin(a);

		float z = sphericalRadius * cos(b);
		x *= sin(b);
		y *= sin(b);

		glm::vec3 temp = glm::vec3(x, y, z);
		return temp + startPosition;
		break;
	}
	default:
		return startPosition;
		break;
	}
}

void ParticleSystem::SetTripleS(float speed, float size, float spread) {

	this->spread = spread;
	this->sizeMultiplier = size;
	this->speedMultiplier = speed;

}

void ParticleSystem::SetStartVariables(float life, float sphericalRadius, float circularRadius, glm::vec4 colour) {


	this->life = life;
	this->sphericalRadius = sphericalRadius;
	this->circularRadius = circularRadius;
	this->colour = colour;

}

void ParticleSystem::SetStartPosition(glm::vec3 StartPosition) {

	this->startPosition = StartPosition;

}

void ParticleSystem::SetGravity(bool isTrue, glm::vec3 gravity) {

	this->gravityEnable = isTrue;
	this->gravityVec = gravity;
}