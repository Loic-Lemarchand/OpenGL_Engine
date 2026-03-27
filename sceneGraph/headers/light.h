#pragma once

#include <glad/glad.h>
#include "component.h"

#include "shader.h"



class Light : public SceneComponent
{
public:
	Light(glm::vec3 color, float intensity);
	~Light();

	bool bIsValid;

	void virtual applyUniforms(std::shared_ptr<Shader> shader, int index) = 0;

	void virtual update() = 0;

	void setColor(glm::vec3 color) { myColor = color; }
	void setIntensity(int intensity) { myIntensity = intensity; }

	glm::vec3 getColor() { return myColor; }
	float getIntensity() { return myIntensity; }

protected:

	glm::vec3 myColor;
	float myIntensity;
};

class PointLight : public Light
{
public:
	PointLight(glm::vec3 color, float intensity, glm::vec3 position);
	~PointLight();

	void update() override;

	void applyUniforms(std::shared_ptr<Shader> shader, int index) override;


private:

	GLuint myUniformDiffuseLightPosition;
	GLuint myUniformDiffuseLightColor;
	GLuint myUniformDiffuseLightIntensity;
};


