#include "Model/Model.h"
#include "SimpleShapes.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define CAMERA_FOV_DEGREE 45.f
#define CAMERA_NEAR_CLIP_DISTANCE 0.1f
#define CAMERA_FAR_CLIP_DISTANCE 1000.f

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

	// Create Shader Object using default Vertex and Fragment Shaders
	Shader* grassShader = new Shader(
		// Load Vertex Shader
		"Resources/Shaders/default.vert",
		// Load Fragment Shader
		"Resources/Shaders/grass.frag"
	);

	// Define light features
	glm::vec4 lightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec3 lightPos = glm::vec3(5.f, 5.f, 5.f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	// export values to object shader
	shader->Activate();
	shader->SetUniform4f("lightColor", lightColor);
	shader->SetUniform3f("lightPos", lightPos);
	
	grassShader->Activate();
	grassShader->SetUniform4f("lightColor", lightColor);
	grassShader->SetUniform3f("lightPos", lightPos);

	// Enables depth test to specify
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Enable Backface Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Creates camera object
	Camera* camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.f, 0.f, 2.f));

	Model grass("Resources/Models/grass/scene.gltf");
	Model ground("Resources/Models/ground/scene.gltf");

	double previousTime = 0;
	double currentTime = 0;
	double timeDifference;
	uint counter = 0;

	// Main loop: keep window open until closed
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		timeDifference = currentTime - previousTime;
		counter++;
		if (timeDifference >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDifference) * counter);
			std::string ms = std::to_string((timeDifference / counter) * 1000);
			std::string newTitle = "3D-Rasterizer - (" + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			previousTime = currentTime;

			// handles camera inputs
			camera->Inputs(window);
		}

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// updates and exports the camera matrix to the vertex shader
		camera->updateMatrix(CAMERA_FOV_DEGREE, CAMERA_NEAR_CLIP_DISTANCE, CAMERA_FAR_CLIP_DISTANCE);
		shader->Activate();
		shader->SetUniform1f("near", CAMERA_NEAR_CLIP_DISTANCE);
		shader->SetUniform1f("far", CAMERA_FAR_CLIP_DISTANCE);
		
		ground.Draw(*shader, *camera);
		glDisable(GL_CULL_FACE);
		grass.Draw(*grassShader, *camera);


		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	shader->Delete();
	grassShader->Delete();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
