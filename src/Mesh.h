#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <sstream>
#include "Shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texcoords;
};

struct Texture {
	GLuint id;
	std::string type;
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
	
	void render(Shader*);
	
	std::vector<Vertex>& getVertices();
	std::vector<GLuint>& getIndices();
	std::vector<Texture>& getTextures();
	
};

#endif
