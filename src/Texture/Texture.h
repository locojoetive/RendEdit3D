#pragma once
#include <glad/glad.h>
#include "../Shader/Shader.h"

class Texture
{
public:
	GLuint ID;
	GLuint unit;
	GLenum type;
	Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);
	~Texture();

	// Assigns a texture unit to a texture
	void texUnit(Shader* shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};
