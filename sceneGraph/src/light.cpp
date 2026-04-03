#include "shader.h"
#include "light.h"
#include "utilities.h"
#include <glm/gtc/type_ptr.hpp>


Light::Light(glm::vec3 color, float intensity) :
	bIsValid(true),
	myColor(color),
	myIntensity(intensity)
{

}


Light::~Light()
{

}


// Point Light
PointLight::PointLight(glm::vec3 color, float intensity, glm::vec3 position) :
	Light(color, intensity)
{
	setWorldPosition(position);
}

PointLight::~PointLight()
{

}

void PointLight::applyUniforms(std::shared_ptr<Shader> shader, int index)
{
	
	if (shader && shader->bIsValid)
	{
		myUniformDiffuseLightPosition = glGetUniformLocation(shader->myProgramID, "diffuseLightPosition");
		myUniformDiffuseLightColor = glGetUniformLocation(shader->myProgramID, "diffuseLightColor");
		myUniformDiffuseLightIntensity = glGetUniformLocation(shader->myProgramID, "diffuseLightIntensity");
	}
	else
	{
		utilities::log("Shader is invalid, cannot get uniform location");
		myUniformDiffuseLightPosition = -1;
		myUniformDiffuseLightColor = -1;
		myUniformDiffuseLightIntensity = -1;
	}
}

void PointLight::update()
{
	glUniform3f(myUniformDiffuseLightPosition, getWorldPosition().x, getWorldPosition().y, getWorldPosition().z);
	glUniform3f(myUniformDiffuseLightColor, myColor.x, myColor.y, myColor.z);
	glUniform1f(myUniformDiffuseLightIntensity, myIntensity);
}