#pragma once
#include "utils/Utils.h"
#include "LevelRenderer.h"
#include "WaterRenderer.h"
#include "WaterFrameBuffer.h"
#include "Skybox.h"
class WorldRenderer
{
private:

	LevelRenderer levelRenderer;

	WaterRenderer waterRenderer;
	//WaterFrameBuffer waterFBO;
	std::vector<Watertile*> watertiles;
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

