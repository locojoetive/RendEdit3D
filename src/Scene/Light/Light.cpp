#include "Light.h"

Light::Light()
{
	// Define light features
	lightPosition = glm::vec3(5.f, 5.f, 5.f);
	lightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
}

Light::~Light()
{

}
