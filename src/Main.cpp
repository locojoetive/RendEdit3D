#include "Mesh/Mesh.h"
#include "SimpleShapes.h"

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


	/* TEXTURE */
	Texture rawTextures[] =
	{
		Texture("Resources/Textures/planks/planks.png", "diffuse", 0),
		Texture("Resources/Textures/planks/planksSpec.png", "specular", 1)
	};

	/* OBJECT SHADER */
	// Create Shader Object using default Vertex and Fragment Shaders
	Shader* shader = new Shader(
		// Load Vertex Shader
		"Resources/Shaders/default.vert",
		// Load Fragment Shader
		"Resources/Shaders/default.frag"
	);


	// Store Mesh data in vectors for the mesh
	std::vector<Vertex> vertices(planeVerticies, planeVerticies + sizeof(planeVerticies) / sizeof(Vertex));
	std::vector<GLuint> indices(planeIndices, planeIndices + sizeof(planeIndices) / sizeof(GLuint));
	std::vector<Texture> textures(rawTextures, rawTextures + sizeof(rawTextures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(vertices, indices, textures);

	/* LIGHT SHADER */
	// Shader for light cube 
	Shader* lightShader = new Shader("Resources/Shaders/light.vert", "Resources/Shaders/light.frag");
	// Store Mesh data in vectors for the mesh
	std::vector<Vertex> lightVertices(rawLightVertices, rawLightVertices + sizeof(rawLightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightIndices(rawLightIndices, rawLightIndices + sizeof(rawLightIndices) / sizeof(GLuint));
	//  Create light cube
	Mesh light(lightVertices, lightIndices, textures);

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
	
	// Creates camera object
	Camera* camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.f, 0.f, 2.f));

	// Enables depth test to specify
	glEnable(GL_DEPTH_TEST);
	
	// Main loop: keep window open until closed
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// updates and exports the camera matrix to the vertex shader
		camera->updateMatrix(45.f, 0.1f, 100.f);
		// handles camera inputs
		camera->Inputs(window);

		// draw floor
		floor.Draw(*shader, *camera);
		// draw light
		light.Draw(*lightShader, *camera);

		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	shader->Delete();
	lightShader->Delete();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
