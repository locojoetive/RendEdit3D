#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../Shader/Shader.h"

class Camera
{
public:
	// camera position and axis directions
	glm::vec3 position;
	glm::vec3 forward{ 0.f, 0.f, -1.f };
	glm::vec3 up{ 0.f, 1.f, 0.f };
	glm::vec3 right{ 1.f, 0.f, 0.f };
	glm::mat4 cameraMatrix = glm::mat4(1.f);

	// prevents camera from junping around when dragging with left mouse button
	bool isFirstClick = true;

	// scree width and height
	int width, height;

	// movement speed of the camera
	float speed = 0.005f;

	// rotation sensitivity of the camera
	float sensitivity = 100.f;

	Camera(int width, int height, glm::vec3 position);

	// updates and exports the camera matrix to the vertex shader
	void updateMatrix(float FOVdegree, float nearPlaneDistance, float farPlaneDistance);
	// exports the camera matrix to a shader
	void Matrix(Shader &shader, const char* uniform);
	void Inputs(GLFWwindow* window);
};

