#version 330 core

// Imports from Vertex Buffer Object
// Position
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture
layout (location = 2) in vec2 aTex;
// Normal (not necessarily normalized)
layout (location = 3) in vec3 aNormal;

// Outputs for the Fragment Shader
// Vertex Color
out vec3 color;
// Texture Coordinates
out vec2 texCoord;
// Normal
out vec3 Normal;
// Current Position
out vec3 currentPos;

// Imports from program
// world to screen matrix
uniform mat4 camMatrix;
// model to world matrix
uniform mat4 model;

void main()
{
	currentPos = vec3(model * vec4(aPos, 1.0f));
	color = aColor;
	texCoord = aTex;
	Normal = aNormal;
	// outputs the position of a vertex
	gl_Position = camMatrix * vec4(currentPos , 1.0);
}
