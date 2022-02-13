#include "VAO.h"

// Constructs a Vertex Array Object with an ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Links a Vertex Buffer Object to the VAO using a certain layout
void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind(); 
	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	// (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Bind the VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Unbind the VAO
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Delete the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}