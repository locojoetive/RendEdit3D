#pragma once

#include <glad/glad.h>
#include <math.h>

// Vertices coordinates
GLfloat triforce[] =
{
	//				Coordinates				//		COLORS			//

	// Lower left corner
	-0.5f,	-0.5f * float(sqrt(3)) / 3,		0.f,	0.47f, 0.1f, 0.08f,
	// Lower right corner
	0.5f,	-0.5f * float(sqrt(3)) / 3,		0.f,	0.97f, 0.71f, 0.22f,
	// Upper Corner
	0.f,	float(sqrt(3)) / 3,				0.f,	0.85f, 0.49f, 0.15f,
	// Inner Left
	-0.25f, 0.5f * float(sqrt(3)) / 6,		0.f,	0.85f, 0.34f, 0.16f,
	// Inner Right
	0.25f,	0.5f * float(sqrt(3)) / 6,		0.f,	0.76f, 0.18f, 0.15f,
	// Inner Down
	0.f,	-0.5f * float(sqrt(3)) / 3,		0.f,	0.63f, 0.53f, 0.62f
};


// Order of indices
GLuint triforceIndices[] =
{
	0, 3, 5,	// Lower Left Triangle
	3, 2, 4,	// Lower Right Triangle
	5, 4, 1		// Upper Triangle
};
