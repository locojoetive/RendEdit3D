#pragma once

#include <glad/glad.h>
#include "../VBO/VBO.h"
class VAO
{
public:
	// ID reference of Vertex Array Object
	GLuint ID;
	// Constructs a Vertex Array Object with an ID
	VAO();

	// Links a Vertex Buffer Object to the 
	void LinkAttrib(VBO* VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Bind the VAO
	void Bind();
	// Unbind the VAO
	void Unbind();
	// Delete the VAO
	void Delete();
};

