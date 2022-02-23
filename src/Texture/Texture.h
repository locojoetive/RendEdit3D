#pragma once
#include <glad/glad.h>
#include "../Shader/Shader.h"

class Texture
{
public:
	GLuint ID;
	GLuint unit;
	const char* type;
	Texture(const char* image, const char* texType, GLuint slot);
	~Texture();

	// Assigns a texture unit to a texture
	void texUnit(Shader &shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};
