#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include "Mesh.h"
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	
	
	void loadModel(std::string);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadTextures(aiMaterial*, aiTextureType, std::string);
public:
	Model(std::string);
	~Model()=default;
	
	void render(Shader* shader);
};

#endif
