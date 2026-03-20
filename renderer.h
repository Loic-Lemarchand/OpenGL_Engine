#pragma once

#include "utilities.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// Changement: utiliser std::vector au lieu de tableaux statiques
static std::vector<unsigned int> indices = {
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
};

static std::vector<float> verticesData = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

class Shader;
class Texture;
class Camera;
class Mesh;

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
	
	std::shared_ptr<Shader> myShader;
	std::shared_ptr<Texture> myTexture;

	std::vector<std::shared_ptr<Mesh>> myMeshes;

	void createBuffers();

	Camera* myCamera;

	unsigned int myShaderProgram;

	glm::mat4 myProjection;

	GLuint myUniformModel;
	GLuint myUniformProjection;
	GLuint myUniformView;
	GLuint myUniformTexture;
};