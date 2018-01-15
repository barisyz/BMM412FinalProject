#version 330 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec3 vNormal;
layout ( location = 2 ) in vec2 texCoords;
layout ( location = 3 ) in ivec4 boneIds;
layout ( location = 4 ) in vec4 weights;

out vec3 fNormal;
out vec3 fPos;

const int MAX_BONES = 100;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 Bones[MAX_BONES];

void main()
{
	vec4 vertexIn = vec4(vPosition, 1.0);

	mat4 BoneTransform = (Bones[boneIds[0]]) * weights[0]
	+ Bones[boneIds[1]] * weights[1];
	+ Bones[boneIds[2]] * weights[2];
	+ Bones[boneIds[3]] * weights[3];

	vec4 bPos = BoneTransform * vec4(vPosition, 1.0);
	fPos = (ViewMatrix * ModelMatrix * bPos).xyz;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * bPos;
	fNormal = ( BoneTransform * vec4(vNormal, 0.0f)).xyz;
}