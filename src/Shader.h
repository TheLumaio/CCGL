
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <iostream>

class Shader
{
private:
	GLuint program;
	GLint vpos;
	GLint cpos;
	GLint mpos;
	
	const char* getSource(char*);
	
public:
	Shader()=default;
	Shader(std::string, std::string);
	~Shader()=default;
	
	void load(std::string, std::string);
	
	void use();
	GLuint& getProgram();
};

#endif
