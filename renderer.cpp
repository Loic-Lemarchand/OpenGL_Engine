
#include "renderer.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "mesh.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float toRadians = 3.14159265 / 180.0f;

Renderer::Renderer(int frameBufferWidth, int frameBufferHeight, Camera* camera) : 
	bIsValid(true), 
	myCamera(camera),
	myShader(nullptr),
	myTexture(nullptr)
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
	

	myShader = std::make_shared<Shader>("../vertexShader.glsl", "../fragmentShader.glsl");

	std::string textName("../Wood_Color.jpg");
	myTexture = std::make_shared<Texture>(1024, 1024, 1, textName.c_str());

	// Conversion de verticesData en std::vector<Vertex>
	std::vector<Vertex> vertices;
	for (size_t i = 0; i < verticesData.size(); i += 5)
	{
		Vertex vertex;
		vertex.Position = glm::vec3(verticesData[i], verticesData[i+1], verticesData[i+2]);
		vertex.TexCoords = glm::vec2(verticesData[i+3], verticesData[i+4]);
		vertices.push_back(vertex);
	}

	std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>(myShader);
	newMesh->addIndices(indices);
	newMesh->addVertices(vertices);
	newMesh->addTexture(myTexture);
	myMeshes.push_back(newMesh);


	// Ensure shader is valid before getting uniform location
	if (myShader && myShader->bIsValid)
	{
		myUniformModel = glGetUniformLocation(myShader->myProgramID, "model");
		myUniformProjection = glGetUniformLocation(myShader->myProgramID, "projection");
		myUniformView = glGetUniformLocation(myShader->myProgramID, "view");
		myUniformTexture = glGetUniformLocation(myShader->myProgramID, "texture1");
	}
	else
	{
		utilities::log("Shader is invalid, cannot get uniform location");
		myUniformModel = -1;
		myUniformProjection = -1;
		myUniformView = -1;
		myUniformTexture = -1;
	}
	
	newMesh->SetupMesh();
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

		
		if (myUniformTexture != -1)
		{
			if (myTexture && myTexture->bIsValid)
			{
				myTexture->bind(0);
			}
			glUniform1i(myUniformTexture, 0);
		}

		glUniformMatrix4fv(myUniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(myUniformProjection, 1, GL_FALSE, glm::value_ptr(myProjection));

		// send the view matrix if camera is valid
		if (myCamera && myUniformView != -1)
		{
			glm::mat4 view = myCamera->CalculateViewMatrix();
			glUniformMatrix4fv(myUniformView, 1, GL_FALSE, glm::value_ptr(view));
		}

		for (auto mesh : myMeshes)
		{
			if (mesh)
			{
				mesh->draw();
			}
		}

	glUseProgram(0);
}