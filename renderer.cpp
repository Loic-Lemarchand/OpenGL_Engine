
#include "renderer.h"
#include "shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float toRadians = 3.14159265 / 180.0f;

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

	myShader = std::make_unique<Shader>("D:/Dev/ProjetOpengl/vertexShader.glsl", "D:/Dev/ProjetOpengl/fragmentShader.glsl");

	// Ensure shader is valid before getting uniform location
	if (myShader && myShader->bIsValid)
	{
		myUniformModel = glGetUniformLocation(myShader->myProgramID, "model");
	}
	else
	{
		utilities::log("Shader is invalid, cannot get uniform location");
		myUniformModel = -1;
	}
	

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


void Renderer::update(glm::vec3 translation, float rotation, glm::vec3 rotationAxe, glm::vec3 scale)
{
	glClearColor(0.10f, 0.12f, 0.18f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//glUseProgram(myShaderProgram);
	myShader->use();

		glm::mat4 model{ 1.0f };
		model = glm::translate(model, translation);
		model = glm::rotate(model, rotation * toRadians, rotationAxe);
		model = glm::scale(model, scale);

		glUniformMatrix4fv(myUniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(myVAO);
	
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

	glUseProgram(0);
}