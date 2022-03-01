#pragma once

#include "Shader.h"

class FrameBuffer
{
private:
	Shader* frameBufferShader;
	unsigned int rectVAO, rectVBO;
	unsigned int FBO;
	unsigned int framebufferTexture;
	unsigned int RBO;
public:
	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();
	void Bind();
	void Draw();
};

