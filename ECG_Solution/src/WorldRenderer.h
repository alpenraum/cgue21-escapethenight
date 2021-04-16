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

	CampFire campfire;

	std::vector<DirectionalLight*>* dirLights;

	/*!
	* List of all PointLights in the scene
	* First one must be the sun/moon of the scene
	*/
	std::vector<PointLight*>* pointLights;



public:
	WorldRenderer();
	WorldRenderer(std::vector<Model*> models, std::vector<Watertile*> watertiles, std::vector<std::string> skyboxFaces, std::vector<DirectionalLight*>* dirLights, std::vector<PointLight*>* pointLights);

	void render(ICamera* camera, float deltaTime, bool lightMapping, bool normalMapping, Player* player, bool renderPlayer, Killer* killer);

	void cleanUp();
};
