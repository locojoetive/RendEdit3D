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
	void Draw(
		Shader& shader,
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.f),
		glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f),
		glm::quat rotation = glm::quat(1.f, 0.f, 0.f, 0.f),
		glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f)
	);
};

