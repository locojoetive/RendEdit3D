#pragma once

#include <glad/glad.h>

class VBO
{
public:
	// ID reference of Vertex Buffer Object
	GLuint ID;
	// Constructs a Vertex Buffer Object and links it to vertices
	VBO(GLfloat* vertices, GLsizeiptr size);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

