#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include "Mesh.h"
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLint TextureFromFile(const char* path, std::string directory);

class Model
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	
	void loadModel(std::string);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadTextures(aiMaterial*, aiTextureType, std::string);
	
	glm::vec3 position;
	
public:
	Model(std::string);
	~Model()=default;
	
	void setPosition(glm::vec3);
	
	void render(Shader* shader);
};

#endif
