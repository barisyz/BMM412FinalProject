#pragma once
#include "Shader.h"
#include "GL\glew.h"
#include "glm\vec4.hpp"
#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include <vector>
#include <iostream>


#define BUFFER_OFFSET(i) ((void*)(i))

using namespace std;

struct Vertex
{
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	Mesh();
	Mesh(vector<Vertex> vertices, vector<GLuint> indices);
	~Mesh();
	void Draw(Shader shader);
	void SetMaterialIndex(unsigned int materialIndex);
	unsigned int GetMaterialIndex();
private:
	vector<Vertex> mVertices;
	vector<GLuint> mIndices;
	unsigned int mMaterialIndex;
	GLuint mVAO, mVBO, mEBO;
	
	void SetupMesh();
};

