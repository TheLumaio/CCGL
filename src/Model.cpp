#include "Model.h"

Model::Model(std::string path)
{
	position = glm::vec3(0,0,0);
	scale = glm::vec3(1,1,1);
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
		std::cout << "processed mesh\n" << std::endl;
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

	std::cout << "Loading model data size ..." << std::endl;
	vertices.reserve(mesh->mNumVertices);
	
	const aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
	
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
		vertex.normal = glm::vec3(0,0,0);
		
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
		
		glm::vec4 color(1,1,1,1);
		aiColor4D diffuse;
		if (aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS)
			color = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
		vertex.color = color;
		
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
	
	std::vector<Texture> diffuseMaps;
	std::vector<Texture> specularMaps;
	
	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	
		diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		specularMaps = loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		
		std::cout << "loaded all textures" << std::endl;
	} // <-- crash here? what the fuck?
	std::cout << "Loaded materials" << std::endl;
	
	// diffuseMaps.clear();
	// specularMaps.clear();
	
	std::cout << "Loaded model data?" << std::endl;
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadTextures(aiMaterial* mat, aiTextureType type, std::string name)
{
	std::cout << "loading " << name << "..." << std::endl;
	std::vector<Texture> _textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		std::cout << "startloop" << std::endl;
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), directory);
		texture.type = name;
		texture.path = str;
		_textures.push_back(texture);
		
		std::cout << "\t loaded texture \"" << name << "\" with id" << texture.id << std::endl;
	}
	std::cout << "exit" << std::endl;
	return _textures;
}

void Model::setPosition(glm::vec3 pos)
{
	position = pos;
}

void Model::setScale(glm::vec3 sca)
{
	scale = sca;
}

void Model::render(Shader* shader)
{
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	GLint modelLoc = glGetUniformLocation(shader->getProgram(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	
	for (int i = 0; i < meshes.size(); i++)
		meshes[i].render(shader, position);
	
}

GLint TextureFromFile(const char* path, std::string directory)
{
    //Generate texture ID and load texture data 
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}
