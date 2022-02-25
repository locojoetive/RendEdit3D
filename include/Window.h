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

	// test attributes
	bool drawTriangle;
	float size;
	float color[4] = { 0.8f, 0.3f, 0.02f, 1.0f };
	char pathInputText[128] = "C:\\_projects\\cpp\\3d-rasterizer\\Resources\\Models\\bunny\\scene.gltf";

	Scene* scene;
	Camera* sceneCamera;
	double previousTime = 0;
	double currentTime = 0;
	double timeDifference;
	unsigned int counter = 0;
	GLFWwindow* windowObject;

public:

	Window(int _width, int _height, std::string _name);
	~Window();

	bool shouldClose();
	void Update();
	void closeWindow();
	void InitGLFWwindow();
	void InitImGUI();
	void RenderImGUI();
};

