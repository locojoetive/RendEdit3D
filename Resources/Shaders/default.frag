#version 330 core
out vec4 FragColor;

// Vertex Shader Imports
in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 currentPos;

// Gets Texture Unit from program
uniform sampler2D tex0;
// Gets Specular Map from program
uniform sampler2D tex1;

// Gets light features from program
uniform vec4 lightColor;
uniform vec3 lightPos;
// Gets camera position from program
uniform vec3 camPos;

void main()
{
	// ambient lighting
	float ambient = 0.2f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currentPos);

	// diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// outputs final color
	FragColor = (texture(tex0, texCoord) * (diffuse + ambient) + texture(tex1, texCoord).r * specular) * lightColor;
}
