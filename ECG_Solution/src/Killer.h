#pragma once
#include "Actor.h"
#include "Model.h"
#include "Player.h"
#include <ctime> 
#include "AnimatedModel.h"
#include "AnimatedModelRenderer.h"
class Killer : public Character
{
private:
	AnimatedModel model;
	AnimatedModelRenderer renderer;

	const float MOVEMENT_SPEED = 2.0f;
	const float ATTACK_SPEED = 3.8f;
	const float UPDATE_TICK = 3.0f;

	float timePassedSinceUpdate = 3.0f;

	bool playerInSightLastFrame = false;

	glm::vec3 movementGoal;

	const glm::vec3 normalizedForwardVector = glm::vec3(0.0f,0.0f,1.0f);


	


public:
	Killer();
	Killer(glm::vec3 position, PhysxMaster* physxMaster);

	void update(Player& player, bool playerNearLight, float dt);

	void draw(ICamera* camera, glm::vec4 clippingPlane, bool lightMapping, bool normalMapping, std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights);

	void drawShadows(AdvancedShader* shader);
	void resetKiller();
	bool showcaseMode = false;
	
};
