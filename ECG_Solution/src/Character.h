#pragma once
#include "Actor.h"
#include "PhysxMaster.h"
#include "CampFire.h"
using namespace physx;
class Character :
    public Actor
{

protected:
    PxController* controller = nullptr;
    PxControllerCollisionFlags collisionFlags;

	

	glm::vec3 updatePhysx(glm::vec3 movementVector, float delta);
	void initPhysx(PhysxMaster* physxMaster);
public:
	enum Character_Movement {
		FORWARD = 1 << 0,
		BACKWARD = 1 << 1,
		LEFT = 1 << 2,
		RIGHT = 1 << 3,
		NO_MOVEMENT = 0
	};


	PxController* getController() {
		return controller;
	}

	Character();
	Character(glm::vec3 position, PhysxMaster* physxMaster);

	

	
};

