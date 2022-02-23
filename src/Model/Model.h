#pragma once

#include <json/json.h>
#include "../Mesh/Mesh.h"

using json = nlohmann::json;
using uchar = unsigned char;
using uint = unsigned int;
using ushort = unsigned short;

class Model
{
public:
	Model(const char* file);

	void Draw(Shader& shader, Camera& camera);

private:
	const char* file;
	std::vector<uchar> data;
	json JSON;

	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	std::vector<std::string> loadedTextureNames;
	std::vector<Texture> loadedTextures;

	std::vector<uchar> getData();
	std::vector<Texture> getTextures();

	void loadMesh(uint indexMesh);

	void traverseNode(uint nextNode, glm::mat4 matrix = glm::mat4(1.f));

	std::vector<GLfloat> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);

	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
	std::vector<Vertex> assembleVertices(
		std::vector<glm::vec3> position,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);
};