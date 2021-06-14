#include "PostProcessingRenderer.h"

void PostProcessingRenderer::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

PostProcessingRenderer::PostProcessingRenderer()
{
	bloomFBO = BloomFrameBuffer();
	bloomShader = std::make_shared<AdvancedShader>("bloom.vert", "bloom.frag");

	hudShader = std::make_shared<AdvancedShader>("hud.vert", "hud.frag");


	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Settings::width, Settings::height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	// create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Settings::width, Settings::height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	hudEntities = std::vector<HudEntity>();

	std::vector<string> paths = std::vector<string>();
	paths.push_back("assets/hud/sanity_GUI_lowScale.png");
	paths.push_back("assets/hud/sanity_SCALE_lowScale.png");
	HudEntity sanity = HudEntity(paths, glm::vec2(-0.75f,-0.75f), glm::vec2(0.2f));
	hudEntities.push_back(sanity);

	paths.clear();
	paths.push_back("assets/hud/loose.png");
	paths.push_back("assets/hud/empty.png");

	loseScreen = HudEntity(paths, glm::vec2(0.0f), glm::vec2(1.0f));

	paths[0] = "assets/hud/win.png";

	winScreen = HudEntity(paths, glm::vec2(0.0f), glm::vec2(1.0f));



}

void PostProcessingRenderer::renderBloom()
{
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	bloomShader.get()->use();
	bloomShader.get()->setUniform("renderPass", 0);
	for (unsigned int i = 0; i < amount; i++)
	{
		bloomFBO.bindPingPongFBO(horizontal);
		bloomShader.get()->setUniform("horizontal", horizontal);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : bloomFBO.getPingPongColorBuffer(!horizontal));
		bloomShader->setUniform("blurTexture", 3);
		// bind texture of other framebuffer (or scene if first iteration)
		//renders a quad virtually in front of the camera which has the frame of the rendered scene as the texture
		renderQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bloomShader.get()->setUniform("renderPass", 1);
	bloomShader.get()->setUniform("gamma", Settings::gamma);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,colorBuffers[0]);
	bloomShader->setUniform("scene", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomFBO.getPingPongColorBuffer(!horizontal));
	bloomShader->setUniform("bloomBlur", 1);
	renderQuad();
}

void PostProcessingRenderer::renderHud(float normalizedSanity)
{
	hudShader->use();
	glDisable(GL_DEPTH_TEST);
	for each (HudEntity en in hudEntities)
	{
		hudShader->setUniform("sanity", normalizedSanity);
		hudShader->setUniform("transformationMatrix", en.getTransformationMatrix()); 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, en.getTextures()[0]);
		hudShader->setUniform("texGUI", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, en.getTextures()[1]);
		hudShader->setUniform("texSCALE", 1);

		renderQuad();
	}
	glEnable(GL_DEPTH_TEST);
	hudShader->unuse();
}



void PostProcessingRenderer::bindFBO()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void PostProcessingRenderer::addHudEntity(HudEntity en)
{
	hudEntities.push_back(en);
}

void PostProcessingRenderer::renderLoseScreen()
{
	hudShader->use();
	glDisable(GL_DEPTH_TEST);
	hudShader->setUniform("sanity", 0.0f);
	hudShader->setUniform("transformationMatrix", loseScreen.getTransformationMatrix());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, loseScreen.getTextures()[0]);
	hudShader->setUniform("texGUI", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, loseScreen.getTextures()[1]);
	hudShader->setUniform("texSCALE", 1);
	renderQuad();
	glEnable(GL_DEPTH_TEST);
	hudShader->unuse();

}

void PostProcessingRenderer::renderWinScreen()
{
	hudShader->use();
	glDisable(GL_DEPTH_TEST);
	hudShader->setUniform("sanity", 0.0f);
	hudShader->setUniform("transformationMatrix", winScreen.getTransformationMatrix());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, winScreen.getTextures()[0]);
	hudShader->setUniform("texGUI", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, winScreen.getTextures()[1]);
	hudShader->setUniform("texSCALE", 1);
	renderQuad();
	glEnable(GL_DEPTH_TEST);
	hudShader->unuse();
}

