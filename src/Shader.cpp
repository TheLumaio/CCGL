#include "Shader.h"

Shader::Shader(std::string vfile, std::string ffile)
{
	load(vfile, ffile);
}


const char* Shader::getSource(char* path)
{
	char* buffer = 0;
	long length;
	FILE* f = fopen(path, "rb");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		rewind(f);
		buffer = malloc(length * (sizeof(char)));
		if (buffer) {
			fread(buffer, sizeof(char), length, f);
		}
		fclose(f);
		buffer[length-1] = '\0';
	}
	return buffer;
}

void Shader::load(std::string vfile, std::string ffile)
{
	const char* vsource = getSource(vfile.c_str());
	const char* fsource = getSource(ffile.c_str());
	
	if (!vsource) {
		std::cout << "Couldn't load vertex source." << std::endl;
		return;
	}
	if (!fsource) {
		std::cout << "Coudn't load fragment source." << std::endl;
		return;
	}
	
	GLuint vid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vid, 1, &vsource, NULL);
	glCompileShader(vid);
	GLint success;
	GLchar info[512];
	glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vid, 512, NULL, info);
		std::cout << "Failed to compile vertex shader \\\n\t" << info << std::endl;
	}
	
	GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fid, 1, &fsource, NULL);
	glCompileShader(fid);
	glGetShaderiv(fid, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fid, 512, NULL, info);
		std::cout << "Failed to compile fragment shader \\\n\t" << info << std::endl;
	}
	
	program = glCreateProgram();
	glAttachShader(program, vid);
	glAttachShader(program, fid);
	// glBindFragDataLocation(program, 0, "outColor");
	glLinkProgram(program);

	vpos = glGetAttribLocation(program, "position");
	cpos = glGetAttribLocation(program, "color");
	mpos = glGetUniformLocation(program, "transform");
	
	glDetachShader(program, vid);
	glDetachShader(program, fid);
	
	glDeleteShader(vid);
	glDeleteShader(fid);
	
	std::cout << "Shader compiled (" << vfile << ":" << ffile << ")" << std::endl;
}

void Shader::use()
{
	glUseProgram(program);
}

GLuint& Shader::getProgram()
{
	return program;
}
