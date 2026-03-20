#include "shader.h"
#include "light.h"


Light::Light(glm::vec3 ambientColor, glm::vec3 diffuseColor, float diffuseIntensity) :
	bIsValid(true),
	myAmbientColor(ambientColor),
	myDiffuseColor(diffuseColor),
	myDiffuseIntensity(diffuseIntensity)
{

}


Light::~Light()
{

}