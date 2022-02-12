#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800


// Vertex Shader source code
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float size;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(size * aPos.x, size * aPos.y, size* aPos.z, 1.0);\n"
"}\0"
;

// Fragment Shader source code
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"	FragColor = color;\n"
"}\n\0"
;

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



	// Vertices coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.f,		// Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.f,		// Lower right corner
		0.f, 0.5f * float(sqrt(3)) * 2 / 3, 0.f		// 
	};


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

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Atach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program
	GLuint shaderProgram = glCreateProgram();
	// Attach Vertex and Fragment Shader to Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader Object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Create reference containers for the Vertex Array Object and the Vertex Buffer Object
	GLuint VAO, VBO;
	// Generate the Vertex Array Object
	glGenVertexArrays(1, &VAO);
	// Generate the Vertex Buffer Object
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it 
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


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
	float color[4] = { .8f, .3f, .02, 1.f };
	// Tell OpenGL which Shader Program we want to use
	glUseProgram(shaderProgram);


	// Main loop: keep window open until closed
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// declare new ImGui Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);

		// Only draw the triangle when the checkbox is ticked
		if (drawTriangle)
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		// ImGUI window creation
		ImGui::Begin("Modify Triangle");
		// Checkbox that appears in window
		ImGui::Checkbox("Draw Triangle", &drawTriangle);
		// Slider that appears in window
		ImGui::SliderFloat("Size", &size, .5f, 2.f);
		// Color Picker that appears in window
		ImGui::ColorEdit4("Color", color);
		// Closes/Deletes the window
		ImGui::End();


		// export variables to shader
		// glUseProgram(shaderProgram);
		glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
		glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);

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

	

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
