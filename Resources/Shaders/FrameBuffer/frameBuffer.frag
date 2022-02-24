#version 330 core

in vec2 texCoords;
out vec4 FragColor;;

uniform sampler2D screenTexture;

const float offset_x = 1.0f / 1920.f;
const float offset_y = 1.0f / 1080.f;

vec2 offset[9] = vec2[]
(
	vec2(-offset_x, offset_y),
	vec2(0, offset_y),
	vec2(offset_x, offset_y),

	vec2(-offset_x, 0),
	vec2(0, 0),
	vec2(offset_x, 0),

	vec2(-offset_x, -offset_y),
	vec2(0, -offset_y),
	vec2(offset_x, -offset_y)
);



float kernel0[] = float[]
(
	1,
	1,
	1,
	
	1,
	1,
	1,
	
	1,
	1,
	1
);

float kernel1[] = float[]
(
	-1,
	-1,
	-1,
	
	1,
	-8,
	1,
	
	2,
	2,
	2
);

float kernel2[] = float[]
(
	1.f/9.f,
	1.f/9.f,
	1.f/9.f,

	1.f/9.f,
	1.f/9.f,
	1.f/9.f,
	
	1.f/9.f,
	1.f/9.f,
	1.f/9.f
);

float kernel3[] = float[]
(
	-2,
	-1,
	0,

	-1,
	1,
	1,

	0,
	1,
	2
);


void main()
{
	vec3 color = vec3(0);
	for (int i = 0; i < 9; i++)
	{
		color += vec3(texture(screenTexture, texCoords.st + offset[i])) * kernel3[i];
	}
	FragColor = vec4(color, 1);
}