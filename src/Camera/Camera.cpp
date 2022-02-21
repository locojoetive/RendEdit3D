#include "Camera.h"

Camera::Camera(int _width, int _height, glm::vec3 _position)
	: width(_width),
	height(_height),
	position(_position)
{}

// updates and exports the camera matrix to the vertex shader
void Camera::updateMatrix(float FOVdegree, float nearPlaneDistance, float farPlaneDistance)
{
	// initializes matrices since otherwise they will be the null matrix
	glm::mat4 view(1.f);
	glm::mat4 projection(1.f);

	// makes camera look in the right direction
	view = glm::lookAt(position, position + forward, up);

	// declares the view frustrum and adds perspective to the scene
	projection = glm::perspective(
		// FOV angle
		glm::radians(45.f),
		// aspect ratio
		(float) width / height,
		// near clip plane distance
		0.1f,
		// far clip plane distance
		100.f
	);

	// Update the camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// exports the camera matrix to a shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}
// handles camera inputs
void Camera::Inputs(GLFWwindow* window)
{
	// handles keyboard inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += speed * forward;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= speed * right;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= speed * forward;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += speed * right;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position -= speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		position += speed * up;
	}

	// handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (isFirstClick)
		{
			glfwSetCursorPos(window, width / 2, height / 2);
			isFirstClick = false;
		}

		// stores coordinates of the cursor
		double mouseX;
		double mouseY;

		// fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// normalizes and shifts the coordinates of the cursor such that they being in the middle of the screen
		// and the transforms them into degrees
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width/ 2)) / width;

		// calculates upcoming up and down rotation of the forward orientation
		glm::vec3 orientation = glm::rotate(forward, glm::radians(-rotX), glm::normalize(glm::cross(forward, up)));


		// decides wether or not the new vertical orientation is legal
		if (!(glm::angle(orientation, up) <= glm::radians(5.f)) or (glm::angle(orientation, -up) <= glm::radians(5.0f)))
		{
			forward = orientation;
		}

		// rotates the forward direction left and right
		forward = glm::rotate(forward, glm::radians(-rotY), up);
		right = glm::cross(forward, up);

		// sets mouse cursor to the middle of the screen, so it doesnt get lost
		glfwSetCursorPos(window, width / 2, height / 2);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// unhides cursor when mouse button is released
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// makes sure the next time the camera looks around it doesn't jump
		isFirstClick = true;
	}
}