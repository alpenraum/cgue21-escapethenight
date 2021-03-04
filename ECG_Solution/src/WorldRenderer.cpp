#include "WorldRenderer.h"

WorldRenderer::WorldRenderer()
{
}

WorldRenderer::WorldRenderer(std::vector<Model*> models, std::vector<Watertile*> watertiles, std::vector<std::string> skyboxFaces, std::vector<DirectionalLight*>* dirLights, std::vector<PointLight*>* pointLights)
{
	levelRenderer = LevelRenderer();
	levelRenderer.addModels(models);


	//waterFBO = WaterFrameBuffer(Settings::width, Settings::height);
	waterRenderer = WaterRenderer();

	this->watertiles = watertiles;

	this->skybox = Skybox(skyboxFaces);

	this->dirLights = dirLights;
	this->pointLights = pointLights;
}

void WorldRenderer::render(ICamera* camera, float deltaTime, bool lightMapping, bool normalMapping)
{

	//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	WaterFrameBuffer waterFBO;
	glm::vec3 cameraPos = camera->getPosition();

	//in case water textures are smaller than screen resolution
	if (Settings::waterTextureScale != 1.0f) {
		glViewport(0, 0, Settings::width * Settings::waterTextureScale, Settings::height * Settings::waterTextureScale);
	}

	for each (Watertile* tile in watertiles) {
		//RENDER REFLECTION
		waterFBO = tile->getWaterFBO();
		//set position to under the water plane
		
		float distance = 2 * (cameraPos.y - tile->getPosition().y);
		camera->setPosition(cameraPos - glm::vec3(0, distance, 0));
		camera->invertPitch();


		waterFBO.bindReflectionFBO();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		levelRenderer.setUniforms(camera, glm::vec4(0, 1.0f, 0, -tile->getPosition().y + 1.0f), lightMapping, normalMapping, *dirLights, *pointLights); 
		levelRenderer.render();
		skybox.draw(camera);


		//RENDER REFRACTION

		//return camera position
		camera->setPosition(cameraPos);
		camera->invertPitch();



		waterFBO.bindRefractionFBO();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		levelRenderer.setUniforms(camera, glm::vec4(0, -1.0f, 0, tile->getPosition().y), lightMapping, normalMapping, *dirLights, *pointLights);
		levelRenderer.render();
		waterFBO.unbindFBO();

	}
	



	//RENDER SCENE
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, Settings::width, Settings::height);
	// Set per-frame uniforms
	levelRenderer.setUniforms(camera, glm::vec4(0, 1.0f, 0, 1000000.0f), lightMapping, normalMapping, *dirLights, *pointLights);
	levelRenderer.render();
	for each (Watertile * tile in watertiles)
	{
		waterRenderer.draw(camera, tile, tile->getWaterFBO(), deltaTime, pointLights->at(0));
	}
	skybox.draw(camera);
	// draw skybox as last

}

void WorldRenderer::cleanUp()
{
	for each (Watertile * tile in watertiles) {
		tile->getWaterFBO().shutdown();
	}
}
