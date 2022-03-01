#include "Model.h"
#include <cmath>

Model::Model(const char* file)
{
	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	Model::file = file;
	data = getData();

	traverseNode(0);

	// set model name as model folder name
	std::string filePath = std::string(file);
	filePath = filePath.substr(0, filePath.find_last_of('/'));
	name = filePath.substr(filePath.find_last_of('/') + 1, filePath.size());
}

void Model::Draw(Shader &shader, Camera &camera)
{
	for (uint i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader, camera, matricesMeshes[i], position, rotation, scale);
	}
}

void Model::setPosition(float _position[3])
{
	position = glm::vec3(_position[0], _position[1], _position[2]);
}

void Model::setRotation(float _rotation[3])
{
	// glm::quat orientation(glm::vec3(ofDegToRad(rotX), ofDegToRad(rotY), ofDegToRad(rotZ)));
	rotation = glm::quat(glm::vec3(
		glm::radians(_rotation[0]),
		glm::radians(_rotation[1]),
		glm::radians(_rotation[2])
	));
}

void Model::setScale(float _scale[3])
{
	scale = glm::vec3(_scale[0], _scale[1], _scale[2]);
}

glm::vec3 Model::getPosition()
{
	return position;
}

glm::vec3 Model::getRotation()
{
	/*
	* calculate euler angles from quaternion
	* https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_angles_conversion
	*/
	return glm::vec3(
		glm::degrees(atan2f(2 * (rotation.w * rotation.x + rotation.y * rotation.z), (1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y)))),
		glm::degrees(asinf(2 * (rotation.w * rotation.y - rotation.z * rotation.x))),
		glm::degrees(atan2f(2 * (rotation.w * rotation.z + rotation.x * rotation.y), (1 - 2 * (rotation.y * rotation.y + rotation.z * rotation.z))))
	);
}

glm::vec3 Model::getScale()
{
	return scale;
}

std::string Model::getName()
{
	return name;
}

void Model::loadMesh(uint indexMesh)
{
	uint positionAccessorIndex = JSON["meshes"][indexMesh]["primitives"][0]["attributes"]["POSITION"];
	uint normalAccessorIndex = JSON["meshes"][indexMesh]["primitives"][0]["attributes"]["NORMAL"];
	uint texCoordAccessorIndex = JSON["meshes"][indexMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	uint indicesAccessorIndex = JSON["meshes"][indexMesh]["primitives"][0]["indices"];

	std::vector<float> positionArray = getFloats(JSON["accessors"][positionAccessorIndex]);
	std::vector<glm::vec3> positions = groupFloatsVec3(positionArray);
	std::vector<float> normalArray = getFloats(JSON["accessors"][normalAccessorIndex]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalArray);
	std::vector<float> texCoordArray = getFloats(JSON["accessors"][texCoordAccessorIndex]);
	std::vector<glm::vec2> texUVs = groupFloatsVec2(texCoordArray);

	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indicesAccessorIndex]);
	std::vector<Texture> textures = getTextures();

	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traverseNode(uint nextNode, glm::mat4 matrix)
{
	json node = JSON["nodes"][nextNode];

	glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f);
	if (node.find("translation") != node.end())
	{
		float translationValues[3];
		for (uint i = 0; i < node["translation"].size(); i++)
		{
			translationValues[i] = (node["translation"][i]);
		}
		translation = glm::make_vec3(translationValues);
	}
	glm::quat rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
	if (node.find("rotation") != node.end())
	{
		float rotationValues[4] =
		{
			/* 
			* last index first since:
			* gltf saves quaternions as (x, y, z, w)
			* glm saves them as (w, x, y, z)
			*/
			
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotationValues);
	}
	glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (uint i = 0; i < node["scale"].size(); i++)
		{
			scaleValues[i] = (node["scale"][i]);
		}
		scale = glm::make_vec3(scaleValues);
	}
	glm::mat4 matNode = glm::mat4(1.f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (uint i = 0; i < node["matrix"].size(); i++)
		{
			matValues[i] = (node["matrix"][i]);
		}
		matNode = glm::make_mat4(matValues);
	}

	glm::mat4 translationMatrix = glm::mat4(1.f);
	glm::mat4 rotationMatrix = glm::mat4(1.f);
	glm::mat4 scaleMatrix = glm::mat4(1.f);

	translationMatrix = glm::translate(translationMatrix, translation);
	rotationMatrix = glm::mat4_cast(rotation);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	glm::mat4 matNextNode = matrix * matNode * translationMatrix * rotationMatrix * scaleMatrix;

	if (node.find("mesh") != node.end())
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	if (node.find("children") != node.end())
	{
		for (uint i = 0; i < node["children"].size(); i++)
		{
			traverseNode(node["children"][i], matNextNode);
		}
	}
}

std::vector<uchar> Model::getData()
{
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = get_file_contents((fileDirectory + uri).c_str());

	std::vector<uchar> data(bytesText.begin(), bytesText.end());

	return data;
}

std::vector<Texture> Model::getTextures()
{
	std::vector<Texture> textures;
	
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	for (uint i = 0; i < JSON["images"].size(); i++)
	{
		std::string texPath = JSON["images"][i]["uri"];
		bool skip = false;

		for (uint j = 0; j < loadedTextureNames.size(); j++)
		{
			if (loadedTextureNames[j] == texPath)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			if (texPath.find("metallicRoughness") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTextures.size());
				textures.push_back(specular);
				loadedTextures.push_back(specular);
				loadedTextureNames.push_back(texPath);
			}
			else
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTextures.size());
				textures.push_back(diffuse);
				loadedTextures.push_back(diffuse);
				loadedTextureNames.push_back(texPath);
			}
		}

	}
	return textures;
}

/*
* This class supports loading 3d models of type *.gltf.
* Since it is in JSON formal, the fields are read using a JSON parser.
* Quick reference of gltf: https://www.khronos.org/files/gltf20-reference-guide.pdf
*/
std::vector<float> Model::getFloats(json accessor)
{
	std::vector<float> floatVector;

	uint buffViewIndex = accessor.value("bufferView", 1);
	uint count = accessor["count"];
	uint accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	json bufferView = JSON["bufferViews"][buffViewIndex];
	uint byteOffset = bufferView["byteOffset"];

	uint numPerVert;
	if (type == "SCALAR")
	{
		numPerVert = 1;
	}
	else if (type == "VEC2")
	{
		numPerVert = 2;
	}
	else if (type == "VEC3")
	{
		numPerVert = 3;
	}
	else if (type == "VEC4")
	{
		numPerVert = 4;
	}
	else
	{
		throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3 nor VEC4");
	}

	uint beginningOfData = byteOffset + accByteOffset;
	uint lengthOfData = count * 4 * numPerVert;
	for (uint i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		uchar bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVector.push_back(value);
	}

	return floatVector;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
	std::vector<GLuint> indices;
	// points to the correct
	uint buffViewIndex = accessor.value("bufferView", 1);
	// how many integers do we get?
	uint count = accessor["count"];
	uint accByteOffset = accessor.value("byteOffset", 0);
	uint componentType = accessor["componentType"];

	json bufferView = JSON["bufferViews"][buffViewIndex];
	uint byteOffset = bufferView["byteOffset"];

	uint beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (uint i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			uchar bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			uint value;
			std::memcpy(&value, bytes, sizeof(uint));
			indices.push_back((GLuint) value);
		}
	}
	else if (componentType == 5123)
	{
		for (uint i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			uchar bytes[] = { data[i++], data[i++] };
			ushort value;
			std::memcpy(&value, bytes, sizeof(ushort));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (uint i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			uchar bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::vector<Vertex> Model::assembleVertices(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.f, 1.f, 1.f),
				texUVs[i]
			}
		);
	}
	return vertices;
}