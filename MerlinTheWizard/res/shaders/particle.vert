#version 430
layout (location = 0) in vec3 vPosition;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform vec3 scale;
uniform vec3 location;
uniform vec3 vColor;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;

void main()
{

mat4 scaleMatrix = mat4(scale.x, 0, 0, 0,
						0, scale.y, 0, 0,
						0, 0, scale.z, 0,
						0, 0, 0, 1);

mat4 translationMatrix = mat4(1, 0, 0, 0,
							  0, 1, 0, 0,
							  0, 0, 1, 0,
							  location.x, location.y, location.z, 1);

mat4 modelMatrix = translationMatrix * scaleMatrix;

vec3 vPosition_new = CameraRight_worldspace * vPosition.x + CameraUp_worldspace * vPosition.y;

gl_Position = ProjectionMatrix * ViewMatrix *  modelMatrix * vec4(vPosition_new, 1);

}