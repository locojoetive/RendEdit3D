#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene.h"

class Window
{
private:
	int width;
	int height;
	std::string name;
	bool isMouseLockedByImGUI;
	bool isKeyboardLockedByImGUI;

	// test attributes
	float position[3] = { 0.f, 0.f, 0.f };
	float rotation[3] = { 0.f, 0.f, 0.f };
	float scale[3] = { 1.f, 1.f, 1.f };
	float cameraSpeed = 1.f;
	float lightColor[4] = { 1.f, 1.f, 1.f, 1.f };
	// TODO: replace with relative path
	char pathInputText[128] = "Resources\\Models\\crow\\scene.gltf";
	int selectedItemIndex;

	Scene* scene;
	Camera* sceneCamera;
	double previousTime = 0;
	double currentTime = 0;
	double timeDifference;
	unsigned int counter = 0;
	GLFWwindow* windowObject;

	void HandleInputs();
public:

	Window(int _width, int _height, std::string _name);
	~Window();

	bool shouldClose();
	void Update();
	void closeWindow();
	void InitGLFWwindow();
	void InitImGUI();
	void RenderImGUI();
	void HandleImGUIInputs();
	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values);
};

