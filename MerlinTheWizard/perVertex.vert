#version 330 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec3 vNormal;
layout ( location = 2 ) in vec2 texCoords;



uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec4 Ka; // ambient coefficient
uniform vec4 Kd; // diffuse coefficient
uniform vec4 Ks; // specular coefficient
uniform float Shininess; // specular exponent

out vec3 fColor;

void main()
{
	vec3 fPos = (ViewMatrix * ModelMatrix * vec4(vPosition, 1.0f)).xyz;
	gl_Position = ProjectionMatrix * vec4(fPos, 1.0f);
	vec3 fNormal = (transpose(inverse(ModelMatrix)) * vec4(vNormal, 1.0f)).xyz;

	vec4 LightPosition	= vec4(0.0f, 15.0f, 0.0f, 1.0f); //todo uniforma cevirilecek
	vec4 LightColor		= vec4(1.0f, 1.0f, 1.0f, 1.0f); //todo uniforma cevirilecek
	
	vec3 N = normalize(fNormal); 
	vec3 L = normalize(LightPosition.xyz);
	vec3 E = normalize(fPos);

	vec3 H = normalize(L + E);

	vec4 Diffuse = max(dot(L, N), 0.0) * Kd * LightColor;
	vec4 Specular = pow(max(dot(N, H), 0.0), Shininess) * Ks * LightColor;
	vec4 Ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f) * Ka;

	fColor = (Ambient +  Specular +  Diffuse);
}