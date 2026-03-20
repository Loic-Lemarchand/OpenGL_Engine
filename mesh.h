#include "utilities.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "texture.h"


struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoords;
};

class Shader;

class Mesh
{
public:
	Mesh(std::shared_ptr<Shader> shader);
	~Mesh();

	void draw();

	Vertex getVertex(int i) { return myVertices[i]; }
	unsigned int getIndice(int i) { return myIndices[i]; }
	Texture* getTexture(int i) { return myTextures[i].get(); }

	void addVertex(Vertex vertex);
	void addIndice(unsigned int indice);
	void addTexture(std::shared_ptr<Texture> texture);

	void addVertices(const std::vector<Vertex>& vertices);
	void addIndices(const std::vector<unsigned int>& indices);
	void addTextures(const std::vector<std::shared_ptr<Texture>>& textures);
	void SetupMesh();

	

private:

	std::weak_ptr<Shader> myShader;


	unsigned int myVAO;
	unsigned int myVBO;
	unsigned int myIBO;

	std::vector<Vertex> myVertices;
	std::vector<unsigned int> myIndices;
	std::vector<std::shared_ptr<Texture>> myTextures;
};