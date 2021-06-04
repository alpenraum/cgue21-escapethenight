#pragma once
#include "BloomFrameBuffer.h"
#include "AdvancedShader.h"
class PostProcessingRenderer
{
private:
	BloomFrameBuffer bloomFBO;

	std::shared_ptr<AdvancedShader> bloomShader;

	void renderQuad();
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	GLuint fbo;
	GLuint colorBuffers[2]; //normal render and bright pixels only
	GLuint depthRenderBuffer;


public:
	PostProcessingRenderer();

	void renderBloom();


	void bindFBO();
};

