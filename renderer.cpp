
#include "renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() : bIsValid(true)
{
	createBuffers();
}

Renderer::~Renderer()
{

}

void Renderer::createBuffers()
{
	
	glGenVertexArrays(1, &myVAO);

	glBindVertexArray(myVAO);

	glGenBuffers(1, &myVBO);


	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Check compilation success
	int success;
	char infoLog[512];
	myShaderProgram = glCreateProgram();

	GLuint vertexShader = AddShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentShader = AddShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	glLinkProgram(myShaderProgram);

	glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(myShaderProgram, sizeof(infoLog), NULL, infoLog);
		utilities::log("Failed to link program : " + std::string(infoLog));
		bIsValid = false;
		return;
	}

	glValidateProgram(myShaderProgram);
	glGetProgramiv(myShaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(myShaderProgram, sizeof(infoLog), NULL, infoLog);
		utilities::log("Error Validating program : " + std::string(infoLog));
		bIsValid = false;
		return;
	}

	myUniformModel = glGetUniformLocation(myShaderProgram, "model");
	

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//p1 : location is 0 in the vertex shader so it is 0
	//p2 : 3 because of vec3
	//p3 : glsl vec are float values
	//p4 : No normalization of data
	//p5 : Stride - the space beetween consecutive vertex attribute (basically the size of a single vertex)
	//p6 : Offset of the beginning of the position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(myVAO);

	utilities::log("Successfully created renderer");
}

GLuint Renderer::AddShader(const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, theCode, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		utilities::log("Error compiling" + std::to_string(shaderType) + " : " + eLog);
		bIsValid = false;
		return -1;
	}

	glAttachShader(myShaderProgram, shader);

	return shader;
}

void Renderer::update(glm::vec3 translation)
{
	glClearColor(0.10f, 0.12f, 0.18f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaderProgram);

		glm::mat4 model{ 1.0f };
		model = glm::translate(model, translation);

		glUniformMatrix4fv(myUniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(myVAO);
	
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

	glUseProgram(0);
}