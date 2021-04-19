#pragma once
#include "PhysXScene.h"
using namespace physx;

// PhysX Variables
PxDefaultAllocator      gAllocator;
PxDefaultErrorCallback  gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPvd* gPvd = NULL;
PxPhysics* gPhysics = NULL;
PxPhysics* mPhysics = NULL;
PxCooking* mCooking = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
PxMaterial* gMaterialTree = NULL;
PxMaterial* gMaterialGround = NULL;
PxRigidDynamic* Player = NULL;
PxRigidDynamic* Killer = NULL;

std::vector<PxRigidActor*> trees;
std::vector<PxRigidActor*> terrain;
PxRigidDynamic* gKinematics;

void initPhysics()
{

    // Initialization of PhysX
    // The gFoundation instance initialize PhysX
    // This uses some callbacks for memory allocation and errors and also the ID version
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

    // PVD is used for visual debugging in PhysX
    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

   // COOKING INIT
   // This is helpful to work with ConvexHull geometries

   // 1. Define the scale
    PxTolerancesScale scale;
    scale.length = 100;
    scale.speed = 981;         // typical speed of an object, gravity*1s

    // Here the library initialize the Physics with some tolerance (this can be updated)
    // ? The tolerance is in animation, maybe gravity, etc
    // This PxCreatePhysics does not have Articulations nor Height fields
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, -3.0f); //define default gravity

    //  cooking transforms the mesh data into a form which allows the SDK to perform efficient collision detection
    mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(scale));

    if (!mCooking)
    {
        printf("PxCreateCooking failed!");
    }

    // For handling the Threads, PhysX uses a gDispatcher
    // GPU Optimization
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    // PhysX Works using Tasks, specially for simulation
    // Creating the scene with the init parameters
    // Every scene uses a Thread Local Storage slot.
    gScene = gPhysics->createScene(sceneDesc);

    //A scene is a collection of bodies and constraints which can interact
    PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();

    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }


    //Default settings from the PhysX Documentation
    gScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
    gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
    gScene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.0f);
    gScene->setVisualizationParameter(PxVisualizationParameter::eBODY_AXES, 1.0f);
    gScene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
    gScene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

    // The material allows us to define the friction for the objects
    // We define the friction for the two elements and the restitution
    // The friction for dynamic elements should be around .1f and the restitution 1.2f
    // Atm i dont know if we need friction or not (maybe for going down a hill or if something is wet idk)
    gMaterialTree = gPhysics->createMaterial(0.0f, 0.1f, 1.2f);
    gMaterialGround = gPhysics->createMaterial(0.0f, 0.5f, 1.4f);

    // BASE -> Actor -> RigidBody
    // PxRigidStatic simulates a rigid body object
    // PxCreatePlane is method to create planes of equation a.x + b = 0
    //when the 3D terrain object is finished this groundPlane will be replaced by it
    PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0.0f, 1.0f, 0.0f, 0.0f), *gMaterialGround);
    gScene->addActor(*groundPlane);
    terrain.push_back(groundPlane);

}

void updateKinematics(PxReal timeStep)
{
    PxTransform motion;
    motion.q = PxQuat(PxIdentity);

    static float gTime = 0.0f;
    gTime += timeStep;

    //Motion on X axis
    //sinf(Period = velocity) * Amplitude;
    const float xf = sinf(gTime * 0.5f) * 9.0f;
    motion.p = PxVec3(xf, 2.0f, -30.0f);

    PxRigidDynamic* kine = gKinematics;
    kine->setKinematicTarget(motion);

}

void update()
{
    float fixedUpdateRate = Time::getFixedDeltaTime();
    float deltaTime = Time::getDeltaTime();
    while (deltaTime > 0)
    {
        deltaTime -= fixedUpdateRate;
        gScene->simulate(fixedUpdateRate);
        gScene->fetchResults(true);
    }

    px::PxU32 nbActiveActors;

    px::PxActor** activeActors = gScene->getActiveActors(nbActiveActors);

    // update each render object with the new transform
}

class PxAllocatorCallback
{
public:
    virtual ~PxAllocatorCallback() {}
    virtual void* allocate(size_t size, const char* typeName, const char* filename,
        int line) = 0;
    virtual void deallocate(void* ptr) = 0;
};

class UserErrorCallback : public PxErrorCallback
{
public:
    virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file,
        int line)
    {
        // error processing implementation
    }
};

void stepPhysics()
{
    const PxReal timeStep = 1.0f / 60.0f;

    updateKinematics(timeStep);

    for (int i = 0; i < 10; i++)
    {
        gScene->simulate(1.0f / 200.0f);
        gScene->fetchResults(true);
    }
}

void cleanupPhysics()
{
    gPhysics->release();
    gFoundation->release();
    printf("Game Over.\n");
}