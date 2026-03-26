#pragma once

#include "utilities.h"
#include "light.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;
class Texture;
class Camera;
class Model;

class Renderer
{
public:
	Renderer() = delete;
	Renderer(int frameBufferWidth, int frameBufferHeight, Camera* camera);
	~Renderer();

	void update();
	//It's okay to be public since not read internally
	bool bIsValid;

private:

	std::shared_ptr<Shader> myShader;

	std::vector<std::shared_ptr<Model>> myModels;

	void createBuffers();

	Camera* myCamera;

	unsigned int myShaderProgram;

	glm::mat4 myProjection;

	GLuint myUniformModel;
	GLuint myUniformProjection;
	GLuint myUniformView;
	GLuint myUniformTexture;

	//Ambient Lighting
	float myAmbientLightingStrength;
	glm::vec3 myAmbientLightingColor;

	GLuint myUniformAmbientLightingStrength;
	GLuint myUniformAmbientLightingColor;

	//Diffuse Lighting
	std::shared_ptr<PointLight> myPointLight;

};