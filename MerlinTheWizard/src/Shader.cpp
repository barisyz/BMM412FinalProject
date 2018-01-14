#include "Shader.h"

Shader::Shader()
{
	mID = -1;
}

Shader::~Shader()
{

}

void Shader::LoadShader(const char* vertexPath, const char* fragmentPath)
{
	const char* vertexFile = Shader::ReadShaderFile(vertexPath);
	const char* fragmentFile = Shader::ReadShaderFile(fragmentPath);

	GLint vertCompileStatus = -1;
	GLint fragCompileStatus = -1;

	GLuint program = glCreateProgram();

	if (vertexFile != NULL && fragmentFile !=NULL) 
	{
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexShaderID, 1, &vertexFile, NULL);
		glCompileShader(vertexShaderID);
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vertCompileStatus);

		glShaderSource(fragmentShaderID, 1, &fragmentFile, NULL);
		glCompileShader(fragmentShaderID);
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragCompileStatus);
		
		if (!vertCompileStatus) 
		{
			std::cerr << vertexFile << " failed to compile:" << std::endl;
			GLint  logSize;
			glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(vertexShaderID, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			throw std::runtime_error("Shader file cannot be loaded!");
		}
		else if (!fragCompileStatus)
		{
			std::cerr << fragmentFile << " failed to compile:" << std::endl;
			GLint  logSize;
			glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(fragmentShaderID, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			throw std::runtime_error("Shader file cannot be loaded!");
		}
		else 
		{
			glAttachShader(program, vertexShaderID);
			glAttachShader(program, fragmentShaderID);
			glLinkProgram(program);
		}
	}
	else
	{
		throw std::runtime_error("Shader file cant be found!");
	}

	mID = program;
}

void Shader::Use()
{
	glUseProgram(mID);
}

void Shader::Use(GLuint id)
{
	glUseProgram(id);
}

const char* Shader::ReadShaderFile(const char* path)
{
	FILE* fp = fopen(path, "r");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	size = fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

GLuint Shader::GetID()
{
	return mID;
}