#include "VertexBufferObject.h"

// Constructs a Vertex Buffer Object and links it to vertices
VertexBufferObject::VertexBufferObject(std::vector<Vertex> &vertices)
{
	// Generate the Vertex Buffer Object
	glGenBuffers(1, &ID);
	// Bind the VBO specifying it is a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, ID);

	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// Binds the VBO
void VertexBufferObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VertexBufferObject::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VertexBufferObject::Delete()
{
	glDeleteBuffers(1, &ID);
}