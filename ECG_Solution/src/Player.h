#pragma once
#include "Actor.h"
#include "utils/Utils.h"
#include "BasicCamera.h"
#include "Model.h"
#include "Light.h"
#include <cmath>
#include "CampFire.h"

class PlayerHand : public Actor {
private:
	Model modelHand;
	PointLight lightsource;
	glm::vec3 torchOffset;

	
public:
	PlayerHand();

	PlayerHand(glm::vec3 playerPos);

	PointLight* getLight();

	void toggleLight();

	bool isEnabled();

	void draw(AdvancedShader* shader, float dt);

	void update(glm::vec3 pos);

};


class Player : 
    public Actor
{

private:
    BasicCamera* camera = nullptr;
    //model of the leg of the player
    //Model modelLeg; --------------- DONT KNOW HOW TO LIMIT ITS ROTATION ONLY TO CAMERAS YAW AND NOT PITCH
    //model of the torch and arm of the player
    PlayerHand hand;
	
	bool isTorchLit = true;
	
	//radius of when the player is 'near a light'
	const float LIGHT_RADIUS = 3.0f;
    //The base speed of the player
    const float SPEED = 4.0f;
	//the current Speed of the player
	float currSpeed = 0.0f;

	bool onGround = true;

	//Current W, A, S, D input
	bool w = false;
	bool a = false;
	bool s = false;
	bool d = false;

	//the velocity of the vertical movement
	float jumpVelocity = 0.0f;

	float yaw = 0.0f;
	float pitch = 0.0f;

	float sanity = 100.0f;
	const float SANITY_CHANGE_PER_SECOND = 1.0f;
	


	

public:
	enum Player_Movement {
		FORWARD = 1 << 0,
		BACKWARD = 1 << 1,
		LEFT = 1 << 2,
		RIGHT = 1 << 3,
		NO_MOVEMENT = 0
	};

	Player();

	/**
	* Update the Player state.
	* @param delta the ammount of time that has passed since the last update.
	*/
	void  update(unsigned int movementDirection, glm::vec2 mouseDelta,float delta, std::vector<CampFire*>* campfires);

	BasicCamera* getCamera();
	void draw(ICamera* camera, AdvancedShader* shader, float dt);
	//Informes the Player to jump in the next tick.
	//TODO: WHEN PHYSX IMPLEMENTED
	void jump();

	PointLight* getLight();
	
	void toggleTorch();

	bool isNearLight(std::vector<CampFire*>* campfires); //TODO: Change to Campfires

	float getSanity();
	void resetSanity();
};


