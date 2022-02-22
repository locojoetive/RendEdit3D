#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VertexBufferObject
{
public:
	// ID reference of Vertex Buffer Object
	GLuint ID;
	// Constructs a Vertex Buffer Object and links it to vertices
	VertexBufferObject(std::vector<Vertex> &vertices);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

