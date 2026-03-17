
#include "renderer.h"
#include "shader.h"
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float toRadians = 3.14159265 / 180.0f;

Renderer::Renderer(int frameBufferWidth, int frameBufferHeight, Camera* camera) : 
	bIsValid(true), 
	myCamera(camera)
{
	createBuffers();
	
	glEnable(GL_DEPTH_TEST);

	//Setup projection matrix
	myProjection = glm::perspective(45.0f, (GLfloat)frameBufferWidth / (GLfloat)frameBufferHeight, 0.1f, 100.0f);
}

Renderer::~Renderer()
{

}

void Renderer::createBuffers()
{
	
	glGenVertexArrays(1, &myVAO);

	glBindVertexArray(myVAO);

	glGenBuffers(1, &myIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &myVBO);


	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	myShader = std::make_unique<Shader>("D:/Dev/ProjetOpengl/vertexShader.glsl", "D:/Dev/ProjetOpengl/fragmentShader.glsl");

	// Ensure shader is valid before getting uniform location
	if (myShader && myShader->bIsValid)
	{
		myUniformModel = glGetUniformLocation(myShader->myProgramID, "model");
		myUniformProjection = glGetUniformLocation(myShader->myProgramID, "projection");
		myUniformView = glGetUniformLocation(myShader->myProgramID, "view");
	}
	else
	{
		utilities::log("Shader is invalid, cannot get uniform location");
		myUniformModel = -1;
		myUniformProjection = -1;
		myUniformView = -1;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myShader->use();

		glm::mat4 model{ 1.0f };
		model = glm::translate(model, translation);
		model = glm::rotate(model, rotation * toRadians, rotationAxe);
		model = glm::scale(model, scale);

		glUniformMatrix4fv(myUniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(myUniformProjection, 1, GL_FALSE, glm::value_ptr(myProjection));

		// send the view matrix if camera is valid
		if (myCamera && myUniformView != -1)
		{
			glm::mat4 view = myCamera->CalculateViewMatrix();
			glUniformMatrix4fv(myUniformView, 1, GL_FALSE, glm::value_ptr(view));
		}

		glBindVertexArray(myVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIBO);
	
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(0);
}