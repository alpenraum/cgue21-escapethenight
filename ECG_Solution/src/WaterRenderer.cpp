#include "WaterRenderer.h"
#include "Watertile.h"
#include "Light.h"

WaterRenderer::WaterRenderer()
{
	shader = std::make_shared<Shader>("water.vert", "water.frag");

	reflectionTextureLocation = shader.get()->getUniformLocationPublic("reflectionTexture");
	refractionTextureLocation = shader.get()->getUniformLocationPublic("refractionTexture");
	dudvLocation = shader.get()->getUniformLocationPublic("dudvmap");
	normalMapLocation = shader.get()->getUniformLocationPublic("normalMap");
	dudvTexture = TextureLoader::loadTexture("assets/models/water/water_dudv.png");
	normalMap = TextureLoader::loadTexture("assets/models/water/water_nm.png");
	refractionDepthTextureLocation = shader.get()->getUniformLocationPublic("refractionDepthTexture");

	glGenVertexArrays(1, &waterVAO);
	glGenBuffers(1, &waterVBO);
	glBindVertexArray(waterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

WaterRenderer::~WaterRenderer()
{
}

void WaterRenderer::draw(ICamera* camera, Watertile* tile, WaterFrameBuffer waterFBO, float deltaTime, std::vector<PointLight*> pointLights, bool normalMapping) {
	shader->use();
	

	//MUST BE DONE FOR EVERY TILE
	shader.get()->setUniform("modelMatrix", tile->getModelMatrix());
	float scaleFactor = glm::length(tile->getScale());
	shader->setUniform("tiling", scaleFactor / 4.0f);

	shader.get()->setUniform("viewProjMatrix", camera->getViewProjMatrix());
	shader->setUniform("moveFactor", tile->getMoveFactor(deltaTime));
	shader->setUniform("cameraPos", camera->getPosition());
	shader->setUniform("normalMapping", normalMapping);
	shader->setUniform("gamma", Settings::gamma);

	for (unsigned int i = 0; i < pointLights.size(); i++) {
		string number = std::to_string(i);

		shader->setUniform(("lights[" + number + "].position").c_str(), pointLights.at(i)->position);
		shader->setUniform(("lights[" + number + "].color").c_str(), pointLights.at(i)->color);
	}

	shader->setUniform("near", camera->getNearFar().x);
	shader->setUniform("far", camera->getNearFar().y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterFBO.getReflectionTexture());
	glUniform1i(reflectionTextureLocation, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterFBO.getRefractionTexture());
	glUniform1i(refractionTextureLocation, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);
	glUniform1i(dudvLocation, 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glUniform1i(normalMapLocation, 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, waterFBO.getRefractionDepthTexture());
	glUniform1i(refractionDepthTextureLocation, 4);

	
	glBindVertexArray(waterVAO);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);


	shader->unuse();
}

void WaterRenderer::cleanup()
{
	glDeleteProgram(shader->getHandle());
}