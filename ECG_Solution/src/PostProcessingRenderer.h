#pragma once
#include "BloomFrameBuffer.h"
#include "AdvancedShader.h"
#include "utils/TextureLoader.h"
#include "HudEntity.h"
class PostProcessingRenderer
{
private:
	BloomFrameBuffer bloomFBO;

	std::shared_ptr<AdvancedShader> bloomShader;
	std::shared_ptr<AdvancedShader> hudShader;

	void renderQuad();
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	GLuint fbo;
	GLuint colorBuffers[2]; //normal render and bright pixels only
	GLuint depthRenderBuffer;

	std::vector<HudEntity> hudEntities;

	HudEntity loseScreen;
	HudEntity winScreen;


public:
	PostProcessingRenderer();

	void renderBloom();

	void renderHud(float normalizedSanity);

	void bindFBO();

	void addHudEntity(HudEntity en);

	void renderLoseScreen();
	void renderWinScreen();
};

