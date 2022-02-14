#version 330 core
out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the CPU
uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
}
