#include "Window.h"


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
	HandleImGUIInputs();

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

	// ImGUI window creation
	ImGui::Begin("Inspect Scene");
	ImGui::InputText("Path", pathInputText, IM_ARRAYSIZE(pathInputText));
	if (ImGui::Button("Load Model"))
	{
		scene->LoadModel(pathInputText);
	}
	// Slider that appears in the window
	ImGui::SliderFloat("PosX", &position.x, -10.f, 10.f);
	ImGui::SliderFloat("PosY", &position.y, -10.f, 10.f);
	ImGui::SliderFloat("PosZ", &position.z, -10.f, 10.f);

	ImGui::SliderFloat("RotX", &rotation.x, 0.f, 360.f);
	ImGui::SliderFloat("RotY", &rotation.y, 0.f, 360.f);
	ImGui::SliderFloat("RotZ", &rotation.z, 0.f, 360.f);

	ImGui::SliderFloat("ScaleX", &scale.x, 0.1f, 1.f);
	ImGui::SliderFloat("ScaleY", &scale.y, 0.1f, 1.f);
	ImGui::SliderFloat("ScaleZ", &scale.z, 0.1f, 1.f);

	ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.f, 4.f);
	// Fancy color editor that appears in the window
	ImGui::ColorEdit4("Light Color", lightColor);
	// Ends the window
	ImGui::End();

	// Renders the ImGUI elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	isKeyboardLockedByImGUI = ImGui::GetIO().WantCaptureKeyboard;
	isMouseLockedByImGUI = ImGui::GetIO().WantCaptureMouse;
}

void Window::HandleImGUIInputs()
{
	if (scene->isModelSelected())
	{
		scene->selectedModel->MoveTo(position);
		scene->selectedModel->RotateTo(rotation);
		scene->selectedModel->ScaleTo(scale);
	}
	sceneCamera->speed = cameraSpeed;
	scene->light.setColor(glm::vec4(lightColor[0], lightColor[1], lightColor[2], lightColor[3]));
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