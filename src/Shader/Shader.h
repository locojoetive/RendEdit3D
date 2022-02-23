#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
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

	void SetUniform1f(std::string key, float value);
	void SetUniform2f(std::string key, glm::vec2 value);
	void SetUniform3f(std::string key, glm::vec3 value);
	void SetUniform4f(std::string key, glm::vec4 value);

private:
	void compileErrors(unsigned int shader, const char* type);
};
