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

void Mesh::Draw(
	Shader &shader,
	Camera &camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
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
	shader.SetUniform3f("cameraPos", camera.position);
	camera.Matrix(shader, "camMatrix");

	glm::mat4 translationMatrix = glm::mat4(1.f);
	glm::mat4 rotationMatrix = glm::mat4(1.f);
	glm::mat4 scaleMatrix = glm::mat4(1.f);

	translationMatrix = glm::translate(translationMatrix, translation);
	rotationMatrix = glm::mat4_cast(rotation);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	shader.SetUniformMatrix4f("model", matrix * translationMatrix * rotationMatrix * scaleMatrix);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}