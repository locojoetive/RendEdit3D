#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class Light
{
private:
	glm::vec3 lightPosition;
	glm::vec4 lightColor;

	Shader* shader;
public:
	Light();
	~Light();
};

