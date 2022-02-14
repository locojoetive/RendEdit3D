#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Shader/Shader.h"
#include "Shader/VBO/VBO.h"
#include "Shader/VAO/VAO.h"
#include "Shader/EBO/EBO.h"
#include "SimpleShapes.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800



int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// We are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW we are using the CORE profile
	// That means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object, called "Tutorial"
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tutorial", NULL, NULL);
	// Error check if window creation failed
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the view port
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create Shader Object using default Vertex and Fragment Shaders
	Shader* shader = new Shader("opengl_shaders/default.vert", "opengl_shaders/default.frag");
	
	// Generate and Bind Vertex Array Object
	VAO VAO1;
	VAO1.Bind();

	// Generate Vertex Buffer Object and Bind it to the vertices
	VBO VBO1(triforce, sizeof(triforce));
	// Generate Index Buffer Object and Bind it to the indices
	EBO EBO1(indices, sizeof(indices));

	// Link VBO to VAO, Link shader attributes to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*) 0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*) (3 * sizeof(float)));

	// Unbind all Objects to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	bool drawTriangle = true;
	float size = 1.f;
	/*
	float color[4] = { .8f, .3f, .02, 1.f };
	*/

	// Main loop: keep window open until closed
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// declare new ImGui Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Only draw the triangle when the checkbox is ticked
		if (drawTriangle)
		{
			shader->Activate();
			
			// export variables to shader
			shader->setFloatInShader("size", size);
			// shader->setColorInFragmentShader("color", color[0], color[1], color[2], color[3]);
			
			// Draw plain old triangle
			// glDrawArrays(GL_TRIANGLES, 0, 3);

			// Bind the VAO so OpenGL knows to use it
			VAO1.Bind();
			// Draw new cool Triforce!
			glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		}

		// ImGUI window creation
		ImGui::Begin("Modify Triangle");
		// Checkbox that appears in window
		ImGui::Checkbox("Draw Triangle", &drawTriangle);
		
		// Slider that appears in window
		ImGui::SliderFloat("Size", &size, .5f, 2.f);
		// Color Picker that appears in window
		// ImGui::ColorEdit4("Color", color);
		// Closes/Deletes the window
		ImGui::End();

		

		

		// Render ImGUI Elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shader->Delete();

	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
