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
#include "Camera/Camera.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main()
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

	/* OBJECT SHADER */
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
	VertexBufferObject* VBO1 = new VertexBufferObject(planeVerticies, sizeof(planeVerticies));
	// Generate Index Buffer Object and Bind it to the indices
	ElementBufferObject* EBO1 = new ElementBufferObject(planeIndices, sizeof(pyramidIndices));
	// Link VBO to VAO, Link shader attributes to VAO
	VAO1->LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*) 0);
	VAO1->LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1->LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1->LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// Unbind all Objects to prevent accidentally modifying them
	VAO1->Unbind();
	VBO1->Unbind();
	EBO1->Unbind();

	/* LIGHT SHADER */
	// Shader for light cube
	Shader* lightShader = new Shader("Resources/Shaders/light.vert", "Resources/Shaders/light.frag");
	// Generates Vertex Array Object and binds it
	VertexArrayObject* lightVAO = new VertexArrayObject;
	lightVAO->Bind();
	// Generates Vertex Buffer and links it to vertices
	VertexBufferObject* lightVBO = new VertexBufferObject(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer and links it to indices
	ElementBufferObject* lightEBO = new ElementBufferObject(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO->LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidental modification
	lightVAO->Unbind();
	lightVBO->Unbind();
	lightEBO->Unbind();

	// Define light features
	glm::vec4 lightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec3 lightPos = glm::vec3(.5f, .5f, .5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	// Define shape features
	glm::vec3 pyramidPos = glm::vec3(0.f, 0.f, 0.f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	// export values to light shader
	lightShader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	
	// export values to object shader
	shader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shader->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	/* TEXTURE */
	Texture* texture = new Texture("Resources/Textures/planks/planks.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture->texUnit(shader, "tex0", 0);

	/* ImGUI WINDOW*/
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	// Creates camera object
	Camera* camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.f, 0.f, 2.f));

	// Enables depth test to specify
	glEnable(GL_DEPTH_TEST);
	
	// Main loop: keep window open until closed
	bool drawMesh = true;
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Only draw the triangle when the checkbox is ticked
		if (drawMesh)
		{

			// handles camera inputs
			camera->Inputs(window);
			// updates and exports the camera matrix to the vertex shader
			camera->updateMatrix(45.f, 0.1f, 100.f);

			// Tell OpenGL which shader we want to use
			shader->Activate();
			// Export camera position to the fragment shader for specular lighting
			glUniform3f(glGetUniformLocation(shader->ID, "camPos"), camera->position.x, camera->position.y, camera->position.z);
			// Export camMatrix to the object vertex shader
			camera->Matrix(*shader, "camMatrix");
			// bind texture
			texture->Bind();
			// Bind the VAO so OpenGL knows to use it
			VAO1->Bind();
			// Draw primitives, number of indices, data type of indices, index of indices
			glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices)/sizeof(int), GL_UNSIGNED_INT, 0);

			// Tell OpenGL which shader we want to use
			lightShader->Activate();
			// Export camMatrix to the light's vertex shader
			camera->Matrix(*lightShader, "camMatrix");
			// Bind the VAO so OpenGL knows to use it
			lightVAO->Bind();
			// Draw primitives, number of indices, data type of indices, index of indices
			glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		// declare new ImGui Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// ImGUI window creation
		ImGui::Begin("Modify Triangle");
		// Checkbox that appears in window
		ImGui::Checkbox("Draw Mesh", &drawMesh);
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
	texture->Delete();
	shader->Delete();
	lightVAO->Delete();
	lightVBO->Delete();
	lightEBO->Delete();
	lightShader->Delete();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
