#include "mesh.h"
#include "glm/glm.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstddef>


Mesh::Mesh(std::shared_ptr<Shader> shader) :
	bIsValid(true),
	myVAO(0),
	myVBO(0),
	myIBO(0)
{
	myShader = shader;
	utilities::log("Successfully created Mesh");
}

Mesh::~Mesh()
{
	GLuint buffers[2] = { myIBO, myVBO };
	GLuint vArrays[1] = { myVAO };

	glDeleteBuffers(2, buffers);
	glDeleteVertexArrays(1, vArrays);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &myVAO);

	glBindVertexArray(myVAO);

	glGenBuffers(1, &myIBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, myIndices.size() * sizeof(unsigned int), &myIndices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &myVBO);

	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, myVertices.size() * sizeof(Vertex), &myVertices[0], GL_STATIC_DRAW);

	//p1 : location is 0 in the vertex shader so it is 0
	//p2 : 3 because of vec3
	//p3 : glsl vec are float values
	//p4 : No normalization of data
	//p5 : Stride - the space beetween consecutive vertex attribute (basically the size of a single vertex)
	//p6 : Offset of the beginning of the position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//UVs
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	glBindVertexArray(0);
}

void Mesh::draw()
{
	glBindVertexArray(myVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIBO);

	glDrawElements(GL_TRIANGLES, myIndices.size(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::addVertex(Vertex vertex)
{
	myVertices.push_back(vertex);
}

void Mesh::addIndice(unsigned int indice)
{
	myIndices.push_back(indice);
}

void Mesh::addTexture(std::shared_ptr<Texture> texture)
{
	myTextures.push_back(texture);
}

void Mesh::addVertices(const std::vector<Vertex>& vertices)
{
	myVertices.insert(myVertices.end(), vertices.begin(), vertices.end());
}

void Mesh::addIndices(const std::vector<unsigned int>& indices)
{
	myIndices.insert(myIndices.end(), indices.begin(), indices.end());
}

void Mesh::addTextures(const std::vector<std::shared_ptr<Texture>>& textures)
{
	myTextures.insert(myTextures.end(), textures.begin(), textures.end());
}

