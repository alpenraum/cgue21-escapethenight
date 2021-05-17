#pragma once
#include "Model.h"
#include <PxPhysicsAPI.h>
#include "PhysxMaster.h"

using namespace physx;
class DynamicModel :
    public Model
{
private:
    PxRigidDynamic* collisionModel;

public:

    DynamicModel();
    DynamicModel(string const& path, glm::vec3 position, PhysxMaster* physxMaster, bool shapeIsExclusive);
	DynamicModel(string const& path, glm::vec3 position,glm::vec3 scale, PhysxMaster* physxMaster, bool shapeIsExclusive);


	PxRigidDynamic* getCollisionModel() {
		return collisionModel;
	}

    void draw(AdvancedShader& shader) override;

	void addTorque(PxVec3 torque) {
		collisionModel->addTorque(torque);
	}
	void addForce(PxVec3 force) {
		collisionModel->addForce(force);
	}
	void setMass(float mass) {
		collisionModel->setMass(mass);
	}
};

