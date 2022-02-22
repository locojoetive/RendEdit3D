#version 330 core

// Imports from Vertex Buffer Object
// Position
layout (location = 0) in vec3 aPos;
// Normal (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture
layout (location = 3) in vec2 aTex;

// Outputs for the Fragment Shader
// Current Position
out vec3 currentPos;
// Normal
out vec3 Normal;
// Vertex Color
out vec3 color;
// Texture Coordinates
out vec2 texCoord;

// Imports from program
// world to screen matrix
uniform mat4 camMatrix;
// model to world matrix
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 model;

void main()
{
	/* MAGIC NUMBER	
	 * negative rotation necessary
	 * might be due to different rotation standards in glm and gltf
	 */
	// 
	currentPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));
	Normal = aNormal;
	color = aColor;
	/* MAGIC NUMBER
	 *  rotate the texture coordinates by 90 degrees
	 *  this also might be due to different rotation standards in glm and gltf
	 */
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;

	// outputs the position of a vertex
	gl_Position = camMatrix * vec4(currentPos , 1.0);
}
