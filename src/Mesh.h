#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
	glm::vec4 color;
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
	Texture() {
		id = -1;
		type = "";
		path = "";
	}
	~Texture() {
	}
};

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	
	GLuint VAO,VBO,EBO;
	void setupMesh();
	
public:
	Mesh(std::vector<Vertex>, std::vector<GLuint>, std::vector<Texture>);
	~Mesh()=default;
	
	void render(Shader*, glm::vec3);
	
	std::vector<Vertex>& getVertices();
	std::vector<GLuint>& getIndices();
	std::vector<Texture>& getTextures();
	
};

#endif
