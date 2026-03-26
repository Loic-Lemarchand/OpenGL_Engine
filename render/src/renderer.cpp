
#include "renderer.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "model.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float toRadians = 3.14159265 / 180.0f;

Renderer::Renderer(int frameBufferWidth, int frameBufferHeight, Camera* camera) :
	bIsValid(true),
	myCamera(camera),
	myShader(nullptr),
	myAmbientLightingStrength(.2f),
	myAmbientLightingColor({1.0f, 1.0f, 1.0f}),
	myPointLight(nullptr)
{
	createBuffers();
	
	glEnable(GL_DEPTH_TEST);

	//Setup projection matrix
	myProjection = glm::perspective(45.0f, (GLfloat)frameBufferWidth / (GLfloat)frameBufferHeight, 0.1f, 100.0f);

	//Lights
	myPointLight = std::make_shared<PointLight>(glm::vec3(1.0f, .9f, .9f), 2.0f, glm::vec3(- 4.0f, 3.0f, 0.0f));
	myPointLight->applyUniforms(myShader, 0);
}

Renderer::~Renderer()
{
	
}

void Renderer::createBuffers()
{
	

	myShader = std::make_shared<Shader>(PROJECT_ROOT_DIR"/render/vertexShader.glsl", PROJECT_ROOT_DIR"/render/fragmentShader.glsl");

	std::shared_ptr<Model> Kitchen = std::make_shared<Model>();
	Kitchen->Load(PROJECT_ROOT_DIR"/Assets/k3/source/mistitown.fbx", myShader);
	Kitchen->position = glm::vec3(0.0f, 0.0f, 0.0f);
	Kitchen->scale = glm::vec3(0.001f);

	myModels.push_back(Kitchen);

	// Ensure shader is valid before getting uniform location
	if (myShader && myShader->bIsValid)
	{
		//TODO : remplacer par les methodes de la classe shader et ajouter une methode vec3
		myUniformModel = glGetUniformLocation(myShader->myProgramID, "model");
		myUniformProjection = glGetUniformLocation(myShader->myProgramID, "projection");
		myUniformView = glGetUniformLocation(myShader->myProgramID, "view");
		myUniformTexture = glGetUniformLocation(myShader->myProgramID, "texture1");
		myUniformAmbientLightingStrength = glGetUniformLocation(myShader->myProgramID, "ambientLightingIntensity");
		myUniformAmbientLightingColor = glGetUniformLocation(myShader->myProgramID, "ambientLightingColor");
	}
	else
	{
		utilities::log("Shader is invalid, cannot get uniform location");
		myUniformModel = -1;
		myUniformProjection = -1;
		myUniformView = -1;
		myUniformTexture = -1;
		myUniformAmbientLightingStrength = -1;
		myUniformAmbientLightingColor = -1;
	}
	
	utilities::log("Successfully created renderer");
}


void Renderer::update()
{
	glClearColor(0.10f, 0.12f, 0.18f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myShader->use();

		if (myUniformAmbientLightingStrength != -1 && myUniformAmbientLightingColor != -1)
		{
			glUniform1f(myUniformAmbientLightingStrength, myAmbientLightingStrength);
			glUniform3f(myUniformAmbientLightingColor, myAmbientLightingColor.x, myAmbientLightingColor.y, myAmbientLightingColor.z);
		}

		myPointLight->update();

		glUniformMatrix4fv(myUniformProjection, 1, GL_FALSE, glm::value_ptr(myProjection));

		// send the view matrix if camera is valid
		if (myCamera && myUniformView != -1)
		{
			glm::mat4 view = myCamera->CalculateViewMatrix();
			glUniformMatrix4fv(myUniformView, 1, GL_FALSE, glm::value_ptr(view));
		}

		for (auto& model : myModels)
		{
			if (model)
			{
				glm::mat4 modelMatrix{ 1.0f };
				modelMatrix = glm::translate(modelMatrix, model->position);
				modelMatrix = glm::rotate(modelMatrix, model->rotation * toRadians, model->rotationAxis);
				modelMatrix = glm::scale(modelMatrix, model->scale);

				glUniformMatrix4fv(myUniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
				model->Draw();
			}
		}

	glUseProgram(0);
}