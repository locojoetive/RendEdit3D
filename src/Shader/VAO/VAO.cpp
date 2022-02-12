#include "VAO.h"

// Constructs a Vertex Array Object with an ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Links a Vertex Buffer Object to the VAO using a certain layout
void VAO::LinkVBO(VBO VBO, GLuint layout)
{
	VBO.Bind();
	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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