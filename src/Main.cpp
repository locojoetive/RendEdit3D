#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader/Shader.h"
#include "Shader/VBO/VertexBufferObject.h"
#include "Shader/VAO/VertexArrayObject.h"
#include "Shader/EBO/ElementBufferObject.h"
#include "SimpleShapes.h"
#include "Texture/Texture.h"

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
	Shader* shader = new Shader(
		// Load Vertex Shader
		"Resources/Shaders/default.vert",
		// Load Fragment Shader
		"Resources/Shaders/default.frag"
	);
	
	// Generate and Bind Vertex Array Object
	VertexArrayObject* VAO1 = new VertexArrayObject;
	VAO1->Bind();

	// Generate Vertex Buffer Object and Bind it to the vertices
	VertexBufferObject* VBO1 = new VertexBufferObject(pyramid, sizeof(pyramid));
	// Generate Index Buffer Object and Bind it to the indices
	ElementBufferObject* EBO1 = new ElementBufferObject(pyramidIndices, sizeof(pyramidIndices));

	// Link VBO to VAO, Link shader attributes to VAO
	VAO1->LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*) 0);
	VAO1->LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1->LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind all Objects to prevent accidentally modifying them
	VAO1->Unbind();
	VBO1->Unbind();
	EBO1->Unbind();

	// Texture
	Texture* choppa = new Texture("Resources/Textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	choppa->texUnit(shader, "tex0", 0);

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
	
	bool drawMesh = true;
	float size = 1.f;
	/*
	float color[4] = { .8f, .3f, .02, 1.f };
	*/


	glEnable(GL_DEPTH_TEST);
	// Main loop: keep window open until closed
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// declare new ImGui Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Only draw the triangle when the checkbox is ticked
		if (drawMesh)
		{
			shader->Activate();

			glm::mat4 modelMatrix = glm::mat4(1.f);
			glm::mat4 viewMatrix = glm::mat4(1.f);
			glm::mat4 projectionMatrix = glm::mat4(1.f);

			viewMatrix = glm::translate(viewMatrix, glm::vec3(0.f, -0.5f, -2.f));
			projectionMatrix = glm::perspective(
				// FOV angle
				glm::radians(45.f),
				// aspect ratio
				(float) WINDOW_WIDTH/WINDOW_HEIGHT,
				// near clip plane
				0.1f,
				// far clip plane
				100.f
			);

			int modelLocation = glGetUniformLocation(shader->ID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			int viewLocation = glGetUniformLocation(shader->ID, "view");
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

			int projectionLocation = glGetUniformLocation(shader->ID, "projection");
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


			// export variables to shader
			shader->setFloatInShader("size", size);
			choppa->Bind();
			// shader->setColorInFragmentShader("color", color[0], color[1], color[2], color[3]);
			
			// Draw plain old triangle
			// glDrawArrays(GL_TRIANGLES, 0, 3);

			// Bind the VAO so OpenGL knows to use it
			VAO1->Bind();
			// Draw new cool Triforce!
			glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices)/sizeof(int), GL_UNSIGNED_INT, 0);
		}

		// ImGUI window creation
		ImGui::Begin("Modify Triangle");
		// Checkbox that appears in window
		ImGui::Checkbox("Draw Mesh", &drawMesh);
		
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

	VAO1->Delete();
	VBO1->Delete();
	EBO1->Delete();
	choppa->Delete();
	shader->Delete();

	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
