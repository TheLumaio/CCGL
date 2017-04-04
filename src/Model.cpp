#include "Model.h"

Model::Model(std::string path)
{
	loadModel(path);
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Assimp error: " << import.GetErrorString() << std::endl;
		return;
	}
	
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	meshes.reserve(node->mNumMeshes);
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
		std::cout << "processed mesh" << std::endl;
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	std::cout << "Loading model data size = " << vertices.size() << " ..." << std::endl;
	vertices.reserve(mesh->mNumVertices);
	
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoords = vec;
		}
		else {
			vertex.texcoords = glm::vec2(0,0);
		}
		
		
		vertices.push_back(vertex);
	}
	std::cout << "Loaded vertices size = " << vertices.size() << " out of " << mesh->mNumVertices << std::endl;
	
	indices.reserve(mesh->mNumFaces);
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indices.emplace_back(face.mIndices[j]);
	}
	std::cout << "Loaded indices" << std::endl;
	
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = this->loadTextures(material, aiTextureType_DIFFUSE, "texture_diffse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = this->loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	std::cout << "Loaded materials" << std::endl;
	
	std::cout << "Loaded model data?" << std::endl;
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadTextures(aiMaterial* mat, aiTextureType type, std::string name)
{
	std::vector<Texture> _textures;
	_textures.reserve(mat->GetTextureCount(type));
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = 0;//TextureFromFile(str.C_Str(), directory);
		texture.type = type;
		//texture.path = str;
		_textures.emplace_back(texture);
	}
	return _textures;
}

void Model::render(Shader* shader)
{
	for (int i = 0; i < meshes.size(); i++)
		meshes[i].render(shader);
}
