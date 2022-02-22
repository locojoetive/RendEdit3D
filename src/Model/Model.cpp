#include "Model.h"

Model::Model(const char* file)
{
	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	Model::file = file;
	data = getData();
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

/*
* This class supports loading 3d models of type *.gltf.
* Since it is in JSON formal, the fields are read using a JSON parser.
* Quick reference of gltf: https://www.khronos.org/files/gltf20-reference-guide.pdf
*/
std::vector<GLfloat> Model::getFloats(json accessor)
{
	std::vector<GLfloat> floatVector;
	// points to the correct
	uint buffViewIndex = accessor.value("bufferView", 1);
	// how many floats do we get?
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
		throw std::invalid_argument("Type is invalid (nor SCALAR, VEC2, VEC3 or VEC4");
	}

	uint beginningOfData = byteOffset + accByteOffset;
	uint lengthOfData = count * 4 * numPerVert;
	for (uint i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		uchar bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVector.push_back((GLfloat)value);
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