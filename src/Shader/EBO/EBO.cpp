#include "EBO.h"

// Constructs an Element Buffer Object and links it to indices
EBO::EBO(GLuint* indices, GLsizeiptr size)
{
	// Generate the Index Buffer Object
	glGenBuffers(1, &ID);
	// Bind the EBO specifying its a GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	// Link EBO to the indices array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

}

// Binds the EBO
void EBO::Bind()
{
	// Unbínd EBO after VAO, since the EBO is stored inside the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the EBO
void EBO::Unbind()
{
	// Unbínd EBO after VAO, since the EBO is stored inside the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void EBO::Delete()
{
	// Delete the Index Buffer
	glDeleteBuffers(1, &ID);
}
