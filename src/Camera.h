#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <glfw/glfw3.h>

class Camera
{
private:
	glm::vec3 position;
	float yaw,pitch,roll;
	float fov = 45.f;
	
	Shader* shader;
	
public:
	Camera(Shader*);
	~Camera()=default;
	
	void attach();
	
};

#endif
