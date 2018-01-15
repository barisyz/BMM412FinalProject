#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

class Shader
{
public:
	Shader();
	~Shader();
	void LoadShader(const char* vertexPath, const char* fragmentPath);
	void Use();
	void Use(GLuint id);
	GLuint GetID();
private:
	GLuint mID;
	const char* ReadShaderFile(const char* path);
};