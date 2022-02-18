#version 330 core

// Position
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture
layout (location = 2) in vec2 aTex;

// Outputs the vertex color
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// world to screen matrix
uniform mat4 camMatrix;

void main()
{
	// outputs the position of a vertex
	gl_Position = camMatrix * vec4(aPos, 1.0);
	
	// outputs the color of a vertex
	color = aColor;
	
	// assigns the texture coordinates from the vertex data
	texCoord = aTex;
}
