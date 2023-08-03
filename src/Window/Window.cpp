#include "Window.h"
#include <vector>


/*
* override ImGui Combo and ListBox to support std::vector without copying the values
* https://eliasdaler.github.io/using-imgui-with-sfml-pt2/#combobox-listbox
*/
namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter, static_cast<void*>(&values), static_cast<int>(values.size()));
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter, static_cast<void*>(&values), static_cast<int>(values.size()));
	}

}

Window::Window(int _width, int _height, std::string _name)
	: width(_width), height(_height), name(_name)
{
	InitGLFWwindow();
	
	sceneCamera = new Camera(
		width,
		height,
		glm::vec3(0, 0, 2)
	);

	scene = new Scene(*sceneCamera);

	InitImGUI();
}

Window::~Window() { closeWindow(); }

bool Window::shouldClose() { return glfwWindowShouldClose(windowObject); }

void Window::Update()
{
	// Calculate frame time and FPS
	currentTime = glfwGetTime();
	timeDifference = currentTime - previousTime;
	counter++;
	if (timeDifference >= 1.0 / 60.0)
	{
		std::string FPS = std::to_string((1.0 / timeDifference) * counter);
		std::string ms = std::to_string((timeDifference / counter) * 1000);
		std::string newTitle = "3D-Rasterizer - (" + FPS + "FPS / " + ms + "ms";
		glfwSetWindowTitle(windowObject, newTitle.c_str());
		previousTime = currentTime;
		// handles camera inputs
		HandleInputs();
	}
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	scene->UpdateScene();

	RenderImGUI();

	// Swap front with back buffer
	glfwSwapBuffers(windowObject);
	// Take care of all GLFW events
	glfwPollEvents();
}

void Window::closeWindow()
{
	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(windowObject);
	glfwTerminate();
}

void Window::InitGLFWwindow()
{
	// Initialize GLFW
	glfwInit();
	/* WINDOW */
	// Tell GLFW what version of OpenGL we are using
	// We are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// That means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a GLFWwindow object
	windowObject = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (windowObject == NULL)
	{
		closeWindow();
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	// Introduce the window
	glfwMakeContextCurrent(windowObject);
	// Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the view port
	glViewport(0, 0, width, height);

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
}

void Window::InitImGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(windowObject, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void Window::RenderImGUI()
{
	// Tell OpenGL a new frame is about to begin
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Model View
	{
		ImGui::Begin("Inspect Model");

		ImGui::InputText("Path", pathInputText, IM_ARRAYSIZE(pathInputText));
		if (ImGui::Button("Load Model"))
		{
			scene->LoadModel(pathInputText);
		}

		ImGui::Separator();

		std::vector<std::string> modelNames = scene->getModelNames();
		// reset transform values in GUI when another model is selected
		if (ImGui::Combo("Selected Model " + selectedItemIndex, &selectedItemIndex, modelNames))
		{
			scene->selectModel(selectedItemIndex);
			glm::vec3 _position = scene->selectedModel->getPosition();
			position[0] = _position.x;
			position[1] = _position.y;
			position[2] = _position.z;

			glm::vec3 _rotation = scene->selectedModel->getRotation();
			rotation[0] = _rotation.x;
			rotation[1] = _rotation.y;
			rotation[2] = _rotation.z;

			glm::vec3 _scale = scene->selectedModel->getScale();
			scale[0] = _scale.x;
			scale[1] = _scale.y;
			scale[2] = _scale.z;
		}

		if (modelNames.size() > 0)
		{
			if (ImGui::Button("Delete Model"))
			{
				scene->DeleteSelectedModel();
			}
			else
			{
				ImGui::SliderFloat3("Position XYZ", &position[0], -100.f, 100.f);
				scene->selectedModel->setPosition(position);
				scene->arrow->setPosition(position);

				ImGui::SliderFloat3("Rotation XYZ", &rotation[0], 0.f, 360.f);
				scene->selectedModel->setRotation(rotation);
				scene->arrow->setRotation(rotation);

				ImGui::SliderFloat3("Scale XYZ", &scale[0], 0.001f, 10.f);
				scene->selectedModel->setScale(scale);
				scene->arrow->setScale(scale);
			}
		}

		// Ends the window
		ImGui::End();
	}

	// Camera View
	{
		// ImGUI window creation
		ImGui::Begin("Camera");
		ImGui::SliderFloat("Camera Speed", &sceneCamera->speed, 0.f, 4.f);

		ImGui::SliderFloat("Near Clip Plane", &sceneCamera->nearClipDistance, 0.001f, 1000.f);
		ImGui::SliderFloat("Near Clip Plane", &sceneCamera->farClipDistance, 0.001f, 1000.f);
		ImGui::SliderFloat("Field Of View Angle", &sceneCamera->fovAngle, -179.f, 179.f);
		// Ends the window
		ImGui::End();
	}

	// Lights View
	{
		// ImGUI window creation
		ImGui::Begin("Lights");
		ImGui::ColorEdit4("Light Color", lightColor);
		scene->light.setColor(glm::vec4(lightColor[0], lightColor[1], lightColor[2], lightColor[3]));
		ImGui::End();

	}

	// Renders the ImGUI elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	isKeyboardLockedByImGUI = ImGui::GetIO().WantCaptureKeyboard;
	isMouseLockedByImGUI = ImGui::GetIO().WantCaptureMouse;
}

void Window::HandleInputs()
{
	// stores coordinates of the cursor
	double mouseX;
	double mouseY;

	// fetches the coordinates of the cursor
	glfwGetCursorPos(windowObject, &mouseX, &mouseY);

	if (!isKeyboardLockedByImGUI)
	{
		sceneCamera->KeyboardInputs(windowObject);
	}
	if (!isMouseLockedByImGUI)
	{
		sceneCamera->MouseInputs(windowObject, mouseX, mouseY);
	}
}