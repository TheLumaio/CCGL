
#include "Camera.h"

Camera::Camera(Shader* shader) :
	shader(shader)
{
	yaw=0;
	pitch=0;
	
	lastx = 1280/2;
	lasty = 720/2;
	cameraPos   = glm::vec3(0,0,0);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
}

void Camera::mouseMove(double x, double y)
{
	float xoff = x-lastx;
	float yoff = lasty-y;
	lastx = x;
	lasty = y;
	
	float sensitivity = 0.05f;
	xoff *= sensitivity;
	yoff *= sensitivity;
	
	yaw += xoff;
	pitch += yoff;
	
	if (pitch > 89.f) pitch = 89.f;
	if (pitch < -89.f) pitch = -89.f;
}

void Camera::update(float dt)
{
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
	
	
	glm::mat4 projection = glm::perspective(glm::radians(90.f), 1280.f/720.f, 0.1f, 1000.f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 model = glm::mat4(1.f);
	// glm::mat4 mvp = projection * view * model;
	
	GLuint matrixid = glGetUniformLocation(shader->getProgram(), "model");
	glUniformMatrix4fv(matrixid, 1, GL_FALSE, &model[0][0]);
	matrixid = glGetUniformLocation(shader->getProgram(), "view");
	glUniformMatrix4fv(matrixid, 1, GL_FALSE, &view[0][0]);
	matrixid = glGetUniformLocation(shader->getProgram(), "projection");
	glUniformMatrix4fv(matrixid, 1, GL_FALSE, &projection[0][0]);
	
}

void Camera::setPosition(glm::vec3 pos) { cameraPos = pos; }

glm::vec3 Camera::getPosition() {
	return cameraPos;
}
glm::vec3 Camera::getFront() {
	return cameraFront;
}
glm::vec3 Camera::getUp() {
	return cameraUp;
}
