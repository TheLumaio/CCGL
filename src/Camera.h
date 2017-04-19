#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <glfw/glfw3.h>

class Camera
{
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	
	float yaw,pitch,roll;
	float fov = 45.f;
	
	
	Shader* shader;
	
public:
	float lastx,lasty;
	Camera(Shader*);
	~Camera()=default;
	
	void mouseMove(double,double);
	void update(float);
	
	void setPosition(glm::vec3);
	
	glm::vec3 getPosition();
	glm::vec3 getFront();
	glm::vec3 getUp();
	
};

#endif
