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

static float vertices[] =
{
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

class Shader;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void update(glm::vec3 translation, float rotation, glm::vec3 rotationAxe, glm::vec3 scale);
	//It's okay to be public since not read internally
	bool bIsValid;

private:
	
	std::unique_ptr<Shader> myShader;

	void createBuffers();

	GLuint AddShader(const char* shaderCode, GLenum shaderType);

	unsigned int myVAO;
	unsigned int myVBO;
	unsigned int myShaderProgram;

	GLuint myUniformModel;
};