#include "test.h"

Test::Test()
{
	
	static const GLfloat vertices[] = {
		-1.f, -1.f, 0.f,
		1.f, -1.f, 0.f,
		0.f, 1.f, 0.f
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Test::render()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// glDisableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
