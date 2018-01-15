#version 330 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec3 vNormal;
layout ( location = 2 ) in vec2 texCoords;

out vec3 fNormal;
out vec3 fPos;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	fPos = (ViewMatrix * ModelMatrix * vec4(vPosition, 1.0f)).xyz;
	gl_Position = ProjectionMatrix * vec4(fPos, 1.0f);
	//fNormal = (transpose(inverse(ModelMatrix)) * vec4(vNormal, 1.0f)).xyz;
	fNormal = vNormal;
}