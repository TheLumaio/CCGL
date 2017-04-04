
#include "Camera.h"

Camera::Camera(Shader* shader) :
	shader(shader)
{
}

void Camera::attach()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.f), 1280.f/720.f, 0.1f, 100.f);
	glm::mat4 view = glm::lookAt(glm::vec3(cos(glfwGetTime())*50,30,sin(glfwGetTime())*50), glm::vec3(0,10,0), glm::vec3(0,1,0));
	glm::mat4 model = glm::mat4(1.f);
	glm::mat4 mvp = projection * view * model;
	
	GLuint matrixid = glGetUniformLocation(shader->getProgram(), "MVP");
	glUniformMatrix4fv(matrixid, 1, GL_FALSE, &mvp[0][0]);
	
	
}
