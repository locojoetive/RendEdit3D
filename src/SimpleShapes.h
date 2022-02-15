#pragma once

#include <glad/glad.h>
#include <math.h>

// Vertices coordinates
GLfloat triangle[] =
{
	//				Coordinates				//		COLORS			//

	// Lower left corner
	-0.5f,	-0.5f * float(sqrt(3)) / 3,		0.f,	0.47f, 0.1f, 0.08f,
	// Lower right corner
	0.5f,	-0.5f * float(sqrt(3)) / 3,		0.f,	0.97f, 0.71f, 0.22f,
	// Upper Corner
	0.f,	float(sqrt(3)) / 3,				0.f,	0.85f, 0.49f, 0.15f
};

// Order of indices
GLuint triangleIndices[] =
{
	0, 1, 2
};


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

GLfloat square[] =
{
	//		Coordinates		//		COLORS		//

	// Lower left corner
	-0.5f,	-0.5f,	0.f,	0.47f,	0.1f,	0.08f,	0.f,	0.f,
	// Lower right corner
	0.5f,	-0.5f,	0.f,	0.85f,	0.49f,	0.15f,	1.f,	0.f,
	// Upper right corner
	0.5f,	0.5f,	0.f,	0.97f,	0.71f,	0.22f,	1.f,	1.f,
	// Upper left corner
	-0.5f,	0.5f,	0.f,	0.85f,	0.34f,	0.16f,	0.f,	1.f
};


// Order of indices
GLuint squareIndices[] =
{
	0, 1, 3,	// Lower Left Triangle
	1, 2, 3		// Upper Right Triangle
};


GLfloat pyramid[] =
{
	//		Coordinates		//		COLORS		//

	// front left
	-0.5f,	0.0f,	0.5f,	0.47f,	0.1f,	0.08f,	0.f,	0.f,
	// back left
	-0.5f,	0.0f,	-0.5f,	0.85f,	0.49f,	0.15f,	1.f,	0.f,
	// back right
	0.5f,	0.f,	-0.5f,	0.97f,	0.71f,	0.22f,	1.f,	1.f,
	// front right
	0.5f,	0.0f,	0.5f,	0.85f,	0.34f,	0.16f,	0.f,	1.f,
	// tip of the pyramid
	0.f,	0.8f,	0.f,	0.85f,	0.34f,	0.16f,	0.f,	1.f
};


// Order of indices
GLuint pyramidIndices[] =
{
	0, 1, 2,
	2, 3, 0,
	0, 3, 4,
	3, 2, 4,
	2, 1, 4,
	1, 0, 4
};