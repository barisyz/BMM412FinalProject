#version 330 core

in vec3 fNormal;
in vec3 fPos;

uniform vec4 Ka; // ambient coefficient
uniform vec4 Kd; // diffuse coefficient
uniform vec4 Ks; // specular coefficient
uniform float Shininess; // specular exponent

uniform vec4 LightPosition;
uniform vec4 LightColor;

out vec4 fColor;

void main()
{
	//vec4 LightPosition	= vec4(0.0f, 15.0f, -10.0f, 1.0f); 
	//vec4 LightColor		= vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	
	vec3 N = normalize(fNormal);
	vec3 L = normalize(LightPosition.xyz);
	vec3 E = normalize(fPos);

	vec3 H = normalize(L + E);

	vec4 Diffuse = max(dot(L, N), 0.0) * Kd * LightColor;
	vec4 Specular = pow(max(dot(N, H), 0.0), Shininess) * Ks * LightColor;
	vec4 Ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f) * Ka;

	fColor = (Ambient +  Specular +  Diffuse);
	fColor.a = 1.0;
}