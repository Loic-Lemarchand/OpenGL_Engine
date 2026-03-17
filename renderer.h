#pragma once

#include "utilities.h"
#include <glad/glad.h>
#include <glm/glm.hpp>



/*static float vertices[] =
{
	0.0f, -0.5f, 0.0f,
	1.0f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f,
	-1.0f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};*/

static int indices[] =
{
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
};

static float vertices[] =
{
	-1.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

class Shader;
class Camera;

class Renderer
{
public:
	Renderer() = delete;
	Renderer(int frameBufferWidth, int frameBufferHeight, Camera* camera);
	~Renderer();

	void update(glm::vec3 translation, float rotation, glm::vec3 rotationAxe, glm::vec3 scale);
	//It's okay to be public since not read internally
	bool bIsValid;

private:
	
	std::unique_ptr<Shader> myShader;

	void createBuffers();

	Camera* myCamera;

	unsigned int myVAO;
	unsigned int myVBO;
	unsigned int myIBO;
	unsigned int myShaderProgram;

	glm::mat4 myProjection;

	GLuint myUniformModel;
	GLuint myUniformProjection;
	GLuint myUniformView;
};