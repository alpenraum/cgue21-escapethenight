#include "WorldRenderer.h"

WorldRenderer::WorldRenderer()
{
}

WorldRenderer::WorldRenderer(std::vector<Model*> models, std::vector<Watertile*> watertiles, std::vector<std::string> skyboxFaces, std::vector<DirectionalLight*>* dirLights, std::vector<PointLight*>* pointLights)
{
	levelRenderer = LevelRenderer();
	levelRenderer.addModels(models);

	omniShadowRenderer = OmniShadowRenderer();

	waterRenderer = WaterRenderer();

	this->watertiles = watertiles;

	this->skybox = Skybox(skyboxFaces);

	this->dirLights = dirLights;
	this->pointLights = pointLights; //DONT FORGET TO CHANGE SIZE IN SHADER WHEN ALTERING SIZE. CURRENTLY 3

	this->campfires = std::vector<CampFire*>();

	CampFire* c = new CampFire("assets/models/campfire/campfire.obj", glm::vec3(5.0f, 3.0f, 10.0f), glm::vec3(1.0f));
	levelRenderer.addModel(c->getModel());
	pointLights->push_back(c->getLight());

	campfires.push_back(c);

	CampFire* c1 = new CampFire("assets/models/campfire/campfire.obj", glm::vec3(31, 4.0f, -19.0f), glm::vec3(1.0f));
	levelRenderer.addModel(c1->getModel());
	pointLights->push_back(c1->getLight());

	campfires.push_back(c1);

	
	postProcessingRenderer = PostProcessingRenderer();



}

void WorldRenderer::render(ICamera* camera, float deltaTime, bool lightMapping, bool normalMapping, Player* player, bool renderPlayer, Killer* killer)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	WaterFrameBuffer waterFBO;
	glm::vec3 cameraPos = camera->getPosition();
	
	ParticleMaster::update(camera->getPosition(), deltaTime);

	//render shadowmaps
	std::vector<Model*>* modelList = levelRenderer.getModels();
	for each (PointLight * light in *pointLights) {
	
		if (light->castsShadows()) {
			omniShadowRenderer.prepareRender(light);
			for each (Model * m in *modelList) {
				if (m->renderShadows) {
					m->draw(*omniShadowRenderer.getShader());
				}
			}
			killer->drawShadows(omniShadowRenderer.getShader().get());
			if (renderPlayer) {
				player->draw(camera, omniShadowRenderer.getShader().get(), deltaTime);
			}
			omniShadowRenderer.cleanUpAfterRender(light);
		}
	}
	

	//in case water textures have a different size to Screen
	glViewport(0, 0, Settings::waterTextureDimension, Settings::waterTextureDimension);
	
	for each (Watertile * tile in watertiles) {
		//RENDER REFLECTION
		waterFBO = tile->getWaterFBO();
		//set position to under the water plane

		float distance = 2 * (cameraPos.y - tile->getPosition().y);
		camera->setPosition(cameraPos - glm::vec3(0, distance, 0));
		camera->invertPitch();

		waterFBO.bindReflectionFBO();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		levelRenderer.setUniforms(true, camera, glm::vec4(0, 1.0f, 0, -tile->getPosition().y + 0.5f), lightMapping, normalMapping, *dirLights, *pointLights, deltaTime);
		levelRenderer.render();

		//killer->draw(camera, glm::vec4(0, 1.0f, 0, -tile->getPosition().y + 0.5f), lightMapping, normalMapping, *dirLights, *pointLights);

		skybox.draw(camera);

		//RENDER REFRACTION

		//return camera position
		camera->setPosition(cameraPos);
		camera->invertPitch();

		waterFBO.bindRefractionFBO();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		levelRenderer.setUniforms(true, camera, glm::vec4(0, -1.0f, 0, tile->getPosition().y), lightMapping, normalMapping, *dirLights, *pointLights, deltaTime);
		levelRenderer.render();
		//killer->draw(camera, glm::vec4(0, 1.0f, 0, -tile->getPosition().y + 0.5f), lightMapping, normalMapping, *dirLights, *pointLights);

		waterFBO.unbindFBO();
		
	}
	
	//FINAL RENDER SCENE TO SCREEN
	postProcessingRenderer.bindFBO();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, Settings::width, Settings::height);
	// Set per-frame uniforms
	levelRenderer.setUniforms(true, camera, glm::vec4(0, 1.0f, 0, 1000000.0f), lightMapping, normalMapping, *dirLights, *pointLights, deltaTime);

	levelRenderer.render();

	
	killer->draw(camera,glm::vec4(0,1.0f,0,1000000.0f),lightMapping,normalMapping,*dirLights,*pointLights);

	if (renderPlayer) {
		player->draw(camera, levelRenderer.getShader(), deltaTime);
	}



	for each (Watertile * tile in watertiles)
	{
		waterRenderer.draw(camera, tile, tile->getWaterFBO(), deltaTime, *pointLights, normalMapping);
	}




	// draw skybox as last
	skybox.draw(camera);

	for each (CampFire * campFire in campfires) {
		campFire->updateParticles(deltaTime);
	}

	ParticleMaster::renderParticles(camera, deltaTime);

	postProcessingRenderer.renderBloom();


	postProcessingRenderer.renderHud(player->getSanity()/100.0f);

}

void WorldRenderer::renderLoseScreen()
{
	postProcessingRenderer.renderLoseScreen();
}

void WorldRenderer::renderWinScreen()
{
	postProcessingRenderer.renderWinScreen();
}

void WorldRenderer::cleanUp()
{
	for each (Watertile * tile in watertiles) {
		tile->getWaterFBO().shutdown();
	}
	for each (PointLight * l in *pointLights) {
		l->omniShadowFBO.shutdown();
	}
	levelRenderer.cleanup();
	waterRenderer.cleanup();
	omniShadowRenderer.cleanup();
	ParticleMaster::cleanup();
}

std::vector<CampFire*>* WorldRenderer::getCampfires()
{
	return &campfires;
}