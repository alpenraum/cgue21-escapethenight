#pragma once
#include <physx/include/PxPhysicsAPI.h>
#include "utils/Utils.h"
using namespace physx;
class PhysxConfig
{
public:
	
	PhysxConfig() {
		mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

		mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(PxTolerancesScale()));
#if DEBUG
/* --------------------------------------------- */
		// Link to Physx Visual Debugger
		/* --------------------------------------------- */
		PxPvd* mPvd = PxCreatePvd(*mFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("Localhost", 5425, 10);
		mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), false, mPvd);
#else
		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), false);
#endif

		/* --------------------------------------------- */
		// Create Physx SDK & Scene & Controller Manager
		/* --------------------------------------------- */

		if (!mPhysics) {
			printError("PxCreatePhysics failed");
		}

		PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f); //GRAVITY
		sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		mScene = mPhysics->createScene(sceneDesc);

		manager = PxCreateControllerManager(*mScene);

		PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}


		material = mPhysics->createMaterial(1.0f, 1.0f, 0.5f);
	}

	void createPlane(PxPlane planeDef) {
		PxRigidStatic* plane = PxCreatePlane(*mPhysics, planeDef, *material);
		mScene->addActor(*plane);
	}


	bool advanceSimulation(PxReal dt) {
		
		M_ACCUMULATOR += dt;
		if (M_ACCUMULATOR < M_STEPSIZE)
			return false;
		M_ACCUMULATOR -= M_STEPSIZE;
		mScene->simulate(M_STEPSIZE);
		
		mScene->fetchResults(true);
		return true;
	}

	void shutdown() {
		manager->release();
		mPhysics->release();
		mCooking->release();
		mFoundation->release();
		
	}

protected:
private:

	int printError(string msg) {
		EXIT_WITH_ERROR(msg);
	}

	PxReal M_ACCUMULATOR = 0.0f;
	const PxReal M_STEPSIZE = 1.0f / 60.0f;
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxFoundation* mFoundation = nullptr;
	PxCooking* mCooking = nullptr;
	PxPhysics* mPhysics = nullptr;
	PxScene* mScene = nullptr;
	PxControllerManager* manager = nullptr;
	//MATERIALS (ONLY ONE FOR NOW)
	PxMaterial* material = nullptr;

};
