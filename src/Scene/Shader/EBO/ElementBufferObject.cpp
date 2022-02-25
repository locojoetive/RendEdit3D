#include "ElementBufferObject.h"

// Constructs an Element Buffer Object and links it to indices
ElementBufferObject::ElementBufferObject(std::vector<GLuint> &indices)
{
	// Generate the Index Buffer Object
	glGenBuffers(1, &ID);
	// Bind the EBO specifying its a GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	// Link EBO to the indices array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

}

// Binds the EBO
void ElementBufferObject::Bind()
{
	// Unbínd EBO after VAO, since the EBO is stored inside the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the EBO
void ElementBufferObject::Unbind()
{
	// Unbínd EBO after VAO, since the EBO is stored inside the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void ElementBufferObject::Delete()
{
	// Delete the Index Buffer
	glDeleteBuffers(1, &ID);
}
