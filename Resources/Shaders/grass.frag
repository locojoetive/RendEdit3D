#version 330 core
out vec4 FragColor;

// Vertex Shader Imports
in vec3 currentPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

// Gets Texture Unit from program
uniform sampler2D diffuse0;
// Gets Specular Map from program
uniform sampler2D specular0;

// Gets light features from program
uniform vec4 lightColor;
uniform vec3 lightPos;
// Gets camera position from program
uniform vec3 camPos;

// Gets camera near and far clip plane distance
uniform float near = 0.1f;
uniform float far = 100.f;

vec4 pointLight()
{
	float a = 0.1f;
	float b = 0.1f;
	vec3 lightVector = lightPos - currentPos;
	float dist = length(lightVector);
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.5f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVector);

	// diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// calculate the camera view dirction
	vec3 viewDirection = normalize(camPos - currentPos);
	// calculate the light reflection on the surface
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	
	// calculate the intensity of the specular value
	float specularLight = 0.5f;
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// outputs final color
	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;

}

vec4 directionalLight()
{
	vec3 lightVector = vec3(1.f, 1.f, 0.f);

	// ambient lighting
	float ambient = 0.2f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVector);

	// diffuse lighting
	float diffuse = max(abs(dot(normal, lightDirection)), 0.f);

	// calculate the camera view dirction
	vec3 viewDirection = normalize(camPos - currentPos);
	// calculate the light reflection on the surface
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	
	// calculate the intensity of the specular value
	float specularLight = 0.5f;
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	if (texture(diffuse0, texCoord).a < 0.1)
		discard;

	// outputs final color
	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}


vec4 spotLight()
{
	// angles between light direction and inner cone in cosine
	float innerCone = 0.95f;
	// angles between light direction and outer cone in cosine
	float outerCone = 0.9f;

	// ambient lighting
	float ambient = 0.2f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currentPos);

	// diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.f);

	// calculate the camera view dirction
	vec3 viewDirection = normalize(camPos - currentPos);
	// calculate the light reflection on the surface
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	
	// calculate the intensity of the specular value
	float specularLight = 0.5f;
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	float angle = dot(vec3(1.0f, -1.0f, 0.0f), -lightDirection);
	float intensity = clamp((angle-outerCone) / (innerCone-outerCone), 0.0f, 1.0f);
	// outputs final color
	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness, float offset)
{
	float zValue = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zValue - offset))));
}

void main()
{
	FragColor = directionalLight();
}
