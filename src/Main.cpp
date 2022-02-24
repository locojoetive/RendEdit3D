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
	Shader* frameBufferShader = new Shader(
		// Load Vertex Shader
		"Resources/Shaders/FrameBuffer/frameBuffer.vert",
		// Load Fragment Shader
		"Resources/Shaders/FrameBuffer/frameBuffer.frag"
	);

	// Define light features
	glm::vec4 lightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec3 lightPos = glm::vec3(5.f, 5.f, 5.f);

	// export values to object shader
	shader->Activate();
	shader->SetUniform4f("lightColor", lightColor);
	shader->SetUniform3f("lightPos", lightPos);
	
	frameBufferShader->Activate();
	frameBufferShader->SetUniform1i("screenTexture", 0);

	// Enables depth test to specify
	glEnable(GL_DEPTH_TEST);

	// Enable Backface Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	// Creates camera object
	Camera* camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.f, 0.f, 2.f));
	Model model("Resources/Models/crow/scene.gltf");

	uint rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));

	double previousTime = 0;
	double currentTime = 0;
	double timeDifference;
	uint counter = 0;

	// create framebuffer
	uint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// create texture for framebuffer
	uint framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	// create render buffer
	uint RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// check for errors in the frame buffer creation
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;



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

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// updates and exports the camera matrix to the vertex shader
		camera->updateMatrix(CAMERA_FOV_DEGREE, CAMERA_NEAR_CLIP_DISTANCE, CAMERA_FAR_CLIP_DISTANCE);
		shader->Activate();
		shader->SetUniform1f("near", CAMERA_NEAR_CLIP_DISTANCE);
		shader->SetUniform1f("far", CAMERA_FAR_CLIP_DISTANCE);
		
		model.Draw(*shader, *camera);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		frameBufferShader->Activate();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	shader->Delete();
	frameBufferShader->Delete();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
