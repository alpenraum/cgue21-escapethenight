#include "PhysxMaster.h"



PhysxMaster::PhysxMaster()
{
    // Initialization of PhysX
    // The gFoundation instance initialize PhysX
    // This uses some callbacks for memory allocation and errors and also the ID version
    unsigned int x = 0x04010200;
    gFoundation = PxCreateFoundation(x, gAllocator, gErrorCallback);

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
    gPhysics = PxCreatePhysics(x, *gFoundation, PxTolerancesScale(), true, gPvd);
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); //define default gravity

    //  cooking transforms the mesh data into a form which allows the SDK to perform efficient collision detection
    mCooking = PxCreateCooking(x, *gFoundation, PxCookingParams(scale));

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


    defaultMaterial = gPhysics->createMaterial(1.0f, 1.0f, 0.5f);

    gManager =PxCreateControllerManager(*gScene);
}

void PhysxMaster::update()
{
    M_ACCUMULATOR += Time::getDeltaTime();
    if (M_ACCUMULATOR < M_STEPSIZE)
        return;
    M_ACCUMULATOR -= M_STEPSIZE;
    gScene->simulate(M_STEPSIZE);

    gScene->fetchResults(true);
    
}

void PhysxMaster::cleanupPhysics()
{
    gPhysics->release();
    gFoundation->release();
    mCooking->release();
    gManager->release();
    printf("PhysX shutdown\n");
}

void PhysxMaster::addActor(PxActor* actor)
{
    gScene->addActor(*actor);
}

PxShape* PhysxMaster::createShapefromTriangleMesh(PxDefaultMemoryInputData input, PxVec3 scale, bool isExclusive)
{
    PxTriangleMesh* triangleMesh = gPhysics->createTriangleMesh(input);
    return gPhysics->createShape(PxTriangleMeshGeometry(triangleMesh,scale), *defaultMaterial, isExclusive);
}
PxShape* PhysxMaster::createShapefromConvexMesh(PxDefaultMemoryInputData input, PxVec3 scale, bool isExclusive)
{
    PxConvexMesh* convexMesh = gPhysics->createConvexMesh(input);
    return gPhysics->createShape(PxConvexMeshGeometry(convexMesh,scale), *defaultMaterial, isExclusive);
}

PxRigidStatic* PhysxMaster::createRigidStatic(PxTransform pos)
{
    return gPhysics->createRigidStatic(pos);
}

PxRigidDynamic* PhysxMaster::createRigidDynamic(PxTransform pos)
{
    return gPhysics->createRigidDynamic(pos);
}

PxController* PhysxMaster::createCapsuleController(PxCapsuleControllerDesc* desc)
{
    return gManager->createController(*desc);
}

PxCooking* PhysxMaster::getCooking()
{
    return mCooking;
}

PxMaterial* PhysxMaster::getMaterial()
{
    return defaultMaterial;
}

PxJoint* PhysxMaster::createRigidJoint(PxRigidActor* actor1, PxRigidActor* actor2, const PxTransform& localFrame0, const PxTransform& localFrame1)
{
    return PxFixedJointCreate(*gPhysics, actor1, localFrame0, actor2, localFrame1);
}
