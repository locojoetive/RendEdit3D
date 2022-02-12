#pragma once
#include <glad/glad.h>
class EBO
{
public:
	// ID reference of Element Buffer Object
	GLuint ID;
	// Constructs an Element Buffer Object and links it to indices
	EBO(GLuint* indices, GLsizeiptr size);

	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};

