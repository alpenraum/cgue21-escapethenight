#pragma once
#include "utils/Utils.h"
#include "LevelRenderer.h"
#include "Skybox.h"
#include "OmniShadowRenderer.h"
#include "WaterFrameBuffer.h"
#include "Watertile.h"
#include "WaterRenderer.h"
class WorldRenderer
{
private:

	LevelRenderer levelRenderer;

	WaterRenderer waterRenderer;
	std::vector<Watertile*> watertiles;

	OmniShadowRenderer omniShadowRenderer;

	Skybox skybox;

	std::vector<DirectionalLight*>* dirLights;

	/*!
	* List of all PointLights in the scene
	* First one must be the sun/moon of the scene
	*/
	std::vector<PointLight*>* pointLights;




public:
	WorldRenderer();
	WorldRenderer(std::vector<Model*> models, std::vector<Watertile*> watertiles, std::vector<std::string> skyboxFaces, std::vector<DirectionalLight*>* dirLights, std::vector<PointLight*>* pointLights);

	void render(ICamera* camera, float deltaTime, bool lightMapping, bool normalMapping);

	void cleanUp();
};

