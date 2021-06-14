#pragma once
#include "Character.h"
#include "utils/Utils.h"
#include "BasicCamera.h"
#include "Model.h"
#include "Light.h"
#include <cmath>
#include "CampFire.h"
#include <PxPhysicsAPI.h>
#include "PhysxMaster.h"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/constants.hpp>

using namespace physx;

class PlayerHand : public Actor{
private:
	Model modelHand;
	PointLight lightsource;
	glm::vec3 torchOffset;

	

public:
	PlayerHand();

	PlayerHand(glm::vec3 playerPos);
	PlayerHand(glm::vec3 playerPos, PhysxMaster* physxMaster);

	PointLight* getLight();

	void toggleLight();

	bool isEnabled();

	void draw(AdvancedShader* shader, float dt);

	void update(glm::vec3 pos, glm::quat Rotation, float dt);


};

class Player :
    public Character
{
private:

    BasicCamera* camera = nullptr;
    
    PlayerHand hand;

	bool isTorchLit = true;

	//radius of when the player is 'near a light'
	const float LIGHT_RADIUS = 6.0f;
    //The base speed of the player
    const float SPEED = 4.0f;
	//the current Speed of the player
	float currSpeed = 0.0f;

	bool onGround = true;

	//the velocity of the vertical movement
	float jumpVelocity = 0.0f;

	

	float sanity = 100.0f;
	const float SANITY_CHANGE_PER_SECOND = 1.5f;

public:
	

	PxController* getController() {
		return controller;
	}
	Player(glm::vec3 position, PhysxMaster* physxMaster);

	/**
	* Update the Player state.
	* @param delta the ammount of time that has passed since the last update.
	*/
	void update(unsigned int movementDirection, glm::vec2 mouseDelta, float delta, std::vector<CampFire*>* campfires);

	BasicCamera* getCamera();
	void draw(ICamera* camera, AdvancedShader* shader, float dt);
	//Informes the Player to jump in the next tick.
	void jump();

	PointLight* getLight();

	void toggleTorch();

	bool isNearLight(std::vector<CampFire*>* campfires);

	float getSanity();
	void resetSanity();

	
};
