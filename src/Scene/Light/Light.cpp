#include "Light.h"

Light::Light()
{
	type = LightType::DIRECTIONAL;
	// Define light features
	position = glm::vec3(5.f, 5.f, 5.f);
	color = glm::vec4(1.f, 1.f, 1.f, 1.f);

	shader = new Shader("Resources/Shaders/light.vert", "Resources/Shaders/light.frag");
}

Light::~Light()
{
	shader->Delete();
}

glm::vec4 Light::getColor()
{
	return color;
}

void Light::setColor(glm::vec4 _color)
{
	color = _color;
}

glm::vec3 Light::getPosition()
{
	return position;
}
