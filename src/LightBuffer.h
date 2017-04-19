#ifndef LIGHTBUFFER_H
#define LIGHTBUFFER_H

#include <GL/glew.h>
#include "Shader.h"

class LightBuffer
{
private:
	
	const GLuint SCR_WIDTH = 1280, SCR_HEIGHT = 720;
public:
	
	GLuint rbuffer;
	GLuint gbuffer;
	GLuint gposition;
	GLuint gnormal;
	GLuint gspec;
	
	LightBuffer();
	~LightBuffer()=default;
	
	void init();
	void bind();
	
};

#endif
