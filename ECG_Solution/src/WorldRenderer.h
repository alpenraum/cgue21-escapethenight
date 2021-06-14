#pragma once
#include "utils/Utils.h"
#include "LevelRenderer.h"
#include "WaterRenderer.h"
#include "WaterFrameBuffer.h"
#include "Skybox.h"
#include "OmniShadowRenderer.h"
#include "Player.h"
#include "Killer.h"
#include "ParticleMaster.h"
#include "CampFire.h"
#include "PostProcessingRenderer.h"
/*!
* This manages the whole rendering process of all different renderers and entities
*/
class WorldRenderer
{
private:

	LevelRenderer levelRenderer;

	WaterRenderer waterRenderer;
	std::vector<Watertile*> watertiles;

	OmniShadowRenderer omniShadowRenderer;

	Skybox skybox;

	std::vector<CampFire*> campfires;

	std::vector<DirectionalLight*>* dirLights;

	/*!
	* List of all PointLights in the scene
	* First one must be the sun/moon of the scene
	*/
	std::vector<PointLight*>* pointLights;

	PostProcessingRenderer postProcessingRenderer;



public:
	WorldRenderer();
	WorldRenderer(std::vector<Model*> models, std::vector<Watertile*> watertiles, std::vector<std::string> skyboxFaces, std::vector<DirectionalLight*>* dirLights, std::vector<PointLight*>* pointLights);

	void render(ICamera* camera, float deltaTime, bool lightMapping, bool normalMapping, Player* player, bool renderPlayer, Killer* killer);


	void renderLoseScreen();
	void renderWinScreen();
	void cleanUp();

	std::vector<CampFire*>* getCampfires();
};