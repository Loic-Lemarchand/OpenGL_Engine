#version 330 core

uniform sampler2D texture1;


//Light
uniform float ambientLightingIntensity;
uniform vec3 ambientLightingColor;

uniform vec3 diffuseLightPosition;
uniform vec3 diffuseLightColor;
uniform float diffuseLightIntensity;

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 fragPos;

out vec4 FragColor;


void main()
{
	//Ambient
	vec3 ambientLighting = vec3(ambientLightingColor.x * ambientLightingIntensity, ambientLightingColor.y * ambientLightingIntensity, ambientLightingColor.z * ambientLightingIntensity);
	

	//Diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(diffuseLightPosition - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * diffuseLightColor * diffuseLightIntensity;

	FragColor = texture(texture1, vTexCoord);
	vec3 resultingLight = (ambientLighting + diffuse);
	FragColor.rgb =  FragColor.rgb * resultingLight;
};