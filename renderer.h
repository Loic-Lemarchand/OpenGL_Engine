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

static const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"   gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

static const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";


class Renderer
{
public:
	Renderer();
	~Renderer();

	void update(glm::vec3 translation);
	//It's okay to be public since not read internally
	bool bIsValid;

private:
	

	void createBuffers();

	GLuint AddShader(const char* shaderCode, GLenum shaderType);

	unsigned int myVAO;
	unsigned int myVBO;
	unsigned int myShaderProgram;

	GLuint myUniformModel;
};