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
px::PxRigidDynamic* createDynamic(const px::PxTransform& t, const px::PxGeometry& geometry, const px::PxVec3& velocity = px::PxVec3(0));
px::PxRigidStatic* createStatic(const px::PxTransform& t, const px::PxVec3& velocity, px::PxTriangleMeshGeometry* geometry);

