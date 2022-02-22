#pragma once

#include <string>
#include "../Shader/VAO/VertexArrayObject.h"
#include "../Shader/EBO/ElementBufferObject.h"
#include "../Camera/Camera.h"
#include "../Texture/Texture.h"

class Mesh
{

public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	// Store VAO in public so it can be used in the draw function
	VertexArrayObject VAO;

	// Initializes the mesh
	Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);
	// Draws the mesh
	void Draw(Shader &shader, Camera& camera);
};

