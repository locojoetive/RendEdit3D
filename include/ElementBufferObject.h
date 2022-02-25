#pragma once

#include <vector>
#include <glad/glad.h>

class ElementBufferObject
{
public:
	// ID reference of Element Buffer Object
	GLuint ID;
	// Constructs an Element Buffer Object and links it to indices
	ElementBufferObject(std::vector<GLuint> &indices);

	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};

