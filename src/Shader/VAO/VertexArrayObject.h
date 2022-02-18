#pragma once

#include <glad/glad.h>
#include "../VBO/VertexBufferObject.h"
class VertexArrayObject
{
public:
	// ID reference of Vertex Array Object
	GLuint ID;
	// Constructs a Vertex Array Object with an ID
	VertexArrayObject();

	// Links a Vertex Buffer Object to the 
	void LinkAttrib(VertexBufferObject* VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Bind the VAO
	void Bind();
	// Unbind the VAO
	void Unbind();
	// Delete the VAO
	void Delete();
};

