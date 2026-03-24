#pragma once

#include "mesh.h"

class Shader;

struct aiNode;
struct aiScene;
struct aiMaterial;
struct aiMesh;
enum aiTextureType;


class Model
{
public:
	Model();
	~Model();

	void Load(const std::string& path, std::shared_ptr<Shader> shader);

	void Draw();

	bool bisValid;

	glm::vec3 position;
	glm::vec3 scale;
	float rotation;
	glm::vec3 rotationAxis;

private:

	void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);

	//Convert assimp mesh to our mesh type
	std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& transform);

	std::vector<std::shared_ptr<Texture>>  loadMaterialTextures(aiMaterial* material, aiTextureType textureType);




	std::vector<std::shared_ptr<Mesh>> myMeshes;

	std::string locationDirectory;

	//Texture cache
	std::unordered_map<std::string, std::shared_ptr<Texture>> myTexturesCache;

	std::shared_ptr<Shader> myShader;


};