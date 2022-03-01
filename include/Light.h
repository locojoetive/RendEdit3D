#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

enum class LightType {
	POINT,
	DIRECTIONAL,
	SPOT
};

class Light
{
private:
	LightType type;
	glm::vec3 position;

	Shader* shader = nullptr;
	glm::vec4 color;
public:
	Light();
	~Light();

	glm::vec3 getPosition();
	glm::vec4 getColor();
	void setColor(glm::vec4 _color);
};

