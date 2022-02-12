#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>


std::string get_file_contents(const char* filename);


class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;

	// Build Shader Program from a Vertex- and a Fragment Shader
	Shader(const char* verteFile, const char* fragmentFile);
	~Shader();

	void Activate();
	void Deactivate();
	void Delete();

	void setFloatInVertexShader(std::string name, GLfloat value);
	void setColorInFragmentShader(std::string name, GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a);
};

