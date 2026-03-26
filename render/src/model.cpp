#include "model.h"
#include "utilities.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h>

Model::Model() :
	bisValid(true),
	position(0.0f),
	scale(1.0f),
	rotation(0.0f),
	rotationAxis(0.0f, 1.0f, 0.0f)
{

}

Model::~Model()
{

}

void Model::Load(const std::string& path, std::shared_ptr<Shader> shader)
{
	myShader = shader;
	Assimp::Importer importer;
	locationDirectory = path.substr(0, path.find_last_of('/') + 1);

	const auto& Scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	if (!Scene || (Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
	{
		utilities::log("The Scene loaded from the model is not valid");
		bisValid = false;
		return;
	}

	processNode(Scene->mRootNode, Scene, glm::mat4(1.0f));
}

/*Un FBX contient un arbre de scène : chaque nœud a sa propre matrice de transformation (position, rotation, scale). 
Si on ne prend pas en compte les transformations des noeufs assimp → tous les meshes sont empilés à l'origine avec des orientations brutes.*/
void Model::processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform)
{
    utilities::log("Processing node: " + std::string(node->mName.C_Str()));

	//Assimp matrix conversion :
	aiMatrix4x4 aiMatrice = node->mTransformation;
	glm::mat4 nodeTransform = glm::mat4(
		aiMatrice.a1, aiMatrice.b1, aiMatrice.c1, aiMatrice.d1,
		aiMatrice.a2, aiMatrice.b2, aiMatrice.c2, aiMatrice.d2,
		aiMatrice.a3, aiMatrice.b3, aiMatrice.c3, aiMatrice.d3,
		aiMatrice.a4, aiMatrice.b4, aiMatrice.c4, aiMatrice.d4
	);

	glm::mat4 globalTransform = parentTransform * nodeTransform;
	
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		unsigned int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];
		if (mesh)
		{
			myMeshes.push_back(processMesh(mesh, scene, globalTransform));
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, globalTransform);
	}
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& transform)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::shared_ptr<Mesh> convertedMesh = std::make_shared<Mesh>(myShader);

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));


	//pos, texCoord, Normal
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		if (mesh->HasPositions())
		{
			glm::vec4 position = transform * glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
			vertex.Position = glm::vec3(position);
		}
		else
		{
			convertedMesh->bIsValid = false;
			utilities::log("The mesh has no valid positions");
		}

		if (mesh->HasNormals())
		{
			vertex.Normal = glm::normalize(normalMatrix * glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		}
		else{
			convertedMesh->bIsValid = false;
			utilities::log("The mesh has no valid normals");
		}

		if (mesh->HasTextureCoords(0))
		{
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	//faces -> indices

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	//Load of different texture types
	std::vector <std::shared_ptr<Texture>> DiffuseTex = loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
	convertedMesh->addTextures(DiffuseTex);

	//

	convertedMesh->addVertices(vertices);
	convertedMesh->addIndices(indices);
	
	convertedMesh->SetupMesh();

	return convertedMesh;
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* material, aiTextureType textureType)
{

	std::vector<std::shared_ptr<Texture>> textures;
	for (int i = 0; i < material->GetTextureCount(textureType); i++)
	{
		
		aiString relativePath;
		material->GetTexture(textureType, i, &relativePath);
		std::string texturePath = relativePath.C_Str();
		

		std::shared_ptr<Texture> texture;

		//Check cache
		auto it = myTexturesCache.find(relativePath.C_Str());
		if (it != myTexturesCache.end())
		{
			textures.push_back(it->second);
		}
		else
		{
			//We check if the path is absolute
			//TODO : Clean texture load
			bool isAbsolute = (texturePath.size() > 1 && texturePath[1] == ':') || texturePath[0] == '/';
			if (isAbsolute)
			{
				size_t lastSlash = texturePath.find_last_of("/\\");
				if (lastSlash != std::string::npos)
				{
					texturePath = texturePath.substr(lastSlash + 1);
				}
			}
			//

			texture = std::make_shared<Texture>(locationDirectory + texturePath);
			utilities::log("Successfully loaded texture " + texturePath);
			myTexturesCache[relativePath.C_Str()] = texture;
			textures.push_back(texture);
		}
	}

	return textures;
}

void Model::Draw()
{
	for (auto& mesh : myMeshes)
	{
		mesh->draw();
	}
}
