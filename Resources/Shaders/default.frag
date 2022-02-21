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

uniform float a;
uniform float b;
uniform float specularLight;
uniform int specularIntensity;

vec4 pointLight()
{
	vec3 lightVector = lightPos - currentPos;
	float dist = length(lightVector);
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.2f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVector);

	// diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// calculate the camera view dirction
	vec3 viewDirection = normalize(camPos - currentPos);
	// calculate the light reflection on the surface
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	// calculate the intensity of the specular value
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), specularIntensity);
	float specular = specAmount * specularLight;

	// outputs final color
	return (texture(tex0, texCoord) * (diffuse * inten + ambient) + texture(tex1, texCoord).r * specular * inten) * lightColor;

}

void main()
{
	FragColor = pointLight();
}
