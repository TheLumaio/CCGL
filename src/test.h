
#ifndef TEST_H
#define TEST_H

#include <GL/glew.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Test
{
private:
	GLuint VBO;
	GLuint VAO;
	
	GLuint CBO;
public:
	Test();
	~Test()=default;
	
	void render(Shader* shader);
	
};

#endif
