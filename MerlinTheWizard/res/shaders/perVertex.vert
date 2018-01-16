#version 330 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec3 vNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec4 Ka; // ambient coefficient
uniform vec4 Kd; // diffuse coefficient
uniform vec4 Ks; // specular coefficient
uniform float Shininess; // specular exponent
uniform vec4 LightPosition;
uniform vec4 LightColor;

out vec4 fColour;

void main()
{


	vec3 pos = (ViewMatrix * ModelMatrix * vec4(vPosition, 1.0f)).xyz;

	vec3 normal = (transpose(inverse(ModelMatrix)) * vec4(vNormal, 1.0f)).xyz;

	
	vec3 N = normalize(normal); 
	vec3 L = normalize(LightPosition.xyz);
	vec3 E = normalize(pos);
	vec3 H = normalize(L + E);

	vec4 Diffuse = max(dot(L, N), 0.0) * Kd * LightColor;
	vec4 Specular = pow(max(dot(N, H), 0.0), Shininess) * Ks * LightColor;
	vec4 Ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f) * Ka;

	fColour = (Ambient +  Specular +  Diffuse);
	fColour.a = 1;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vPosition, 1.0f);
}