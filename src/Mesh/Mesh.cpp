#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& _vertices, std::vector<GLuint>& _indices, std::vector<Texture>& _textures)
	: vertices(_vertices),
	indices(_indices),
	textures(_textures)
{
	VAO.Bind();
	// Generate Vertex Buffer Object and Bind it to the vertices
	VertexBufferObject* VBO = new VertexBufferObject(vertices);
	// Generate Index Buffer Object and Bind it to the indices
	ElementBufferObject* EBO = new ElementBufferObject(indices);
	// Link VBO to VAO, Link shader attributes to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// Unbind all Objects to prevent accidentally modifying them
	VAO.Unbind();
	VBO->Unbind();
	EBO->Unbind();
}

void Mesh::Draw(Shader &shader, Camera &camera)
{
	shader.Activate();
	VAO.Bind();

	// iterate over diffuse and specular textures 
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;

		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	// why this here???
	camera.Matrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}