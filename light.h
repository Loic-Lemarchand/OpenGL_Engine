#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader.h"



class Light
{
public:
	Light(glm::vec3 ambientColor, glm::vec3 diffuseColor, float diffuseIntensity);
	~Light();

	bool bIsValid;

	void virtual applyUniforms(Shader shader, int index) = 0;

	void setAmbientColor(glm::vec3 color) { myAmbientColor = color; }
	void setDiffuseColor(glm::vec3 color) { myDiffuseColor = color; }
	void setAmbientColor(int intensity) { myDiffuseIntensity = intensity; }

private:

	glm::vec3 myAmbientColor;
	glm::vec3 myDiffuseColor;
	float myDiffuseIntensity;
};

