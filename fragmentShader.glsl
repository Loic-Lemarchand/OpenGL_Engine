#version 330 core

uniform sampler2D texture1;

//Light
uniform float ambientLightingIntensity;
uniform vec3 ambientLightingColor;

out vec4 FragColor;

in vec2 vTexCoord;


void main()
{
	vec3 ambientLighting = vec3(ambientLightingColor.x * ambientLightingIntensity, ambientLightingColor.y * ambientLightingIntensity, ambientLightingColor.z * ambientLightingIntensity);
	
	FragColor = texture(texture1, vTexCoord);

	FragColor.rgb =  FragColor.rgb * ambientLighting;
};