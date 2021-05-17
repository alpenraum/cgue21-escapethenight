#pragma once
#include "Model.h"
#include <PxPhysicsAPI.h>
#include "PhysxMaster.h"
class StaticModel :
    public Model
{
private:
    physx::PxRigidStatic* collisionModel;

public:
    StaticModel();
    StaticModel(string const& path, glm::vec3 position, PhysxMaster* physxMaster, bool shapeIsExclusive);
    StaticModel(string const& path, glm::vec3 position, glm::vec3 scale, PhysxMaster* physxMaster, bool shapeIsExclusive);
};

