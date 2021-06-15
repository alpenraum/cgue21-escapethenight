#pragma once
#include "utils/Utils.h"
#include "utils/Time.h"
#include <PxPhysicsAPI.h>
using namespace physx;
class PhysxMaster
{
private:

	const PxReal M_STEPSIZE = 1.0f / 30.0f;
	PxReal M_ACCUMULATOR = 0.0f;
	// PhysX Variables
	PxDefaultAllocator      gAllocator;
	PxDefaultErrorCallback  gErrorCallback;

	PxControllerManager* gManager = NULL;
	PxFoundation* gFoundation = NULL;
	PxPvd* gPvd = NULL;
	PxPhysics* gPhysics = NULL;
	PxCooking* mCooking = NULL;

	PxDefaultCpuDispatcher* gDispatcher = NULL;
	PxScene* gScene = NULL;
	
	PxMaterial* defaultMaterial = NULL;

	


public:

	PhysxMaster();

	void update();

	void cleanupPhysics();


	void addActor(PxActor* actor);

	PxShape* createShapefromTriangleMesh(PxDefaultMemoryInputData input, PxVec3 scale, bool isExclusive);

	PxShape* createShapefromConvexMesh(PxDefaultMemoryInputData input, PxVec3 scale, bool isExclusive);


	PxRigidStatic* createRigidStatic(PxTransform pos);
	PxRigidDynamic* createRigidDynamic(PxTransform pos);

	PxController* createCapsuleController(PxCapsuleControllerDesc* desc);

	PxCooking* getCooking();
	PxMaterial* getMaterial();

	PxJoint* createRigidJoint(PxRigidActor* actor1, PxRigidActor* actor2, const PxTransform& localFrame0, const PxTransform& localFrame1);
};

