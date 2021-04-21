#pragma once
#include "Utils/Utils.h"
#include "BasicCamera.h"
#include "Model.h"
#include "PhysXScene.h"
using namespace physx;

void initPhysics();
void update();
void render();
void cleanupPhysics();
void stepPhysics();
void setActiveCamera(BasicCamera* cam);
void updateKinematics(PxReal timeStep);
BasicCamera* getActiveCamera();
physx::PxRigidDynamic* createDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry, const physx::PxVec3& velocity = physx::PxVec3(0));
physx::PxRigidStatic* createStatic(const physx::PxTransform& t, const physx::PxVec3& velocity, physx::PxTriangleMeshGeometry* geometry);

