#include "Character.h"


glm::vec3 Character::updatePhysx(glm::vec3 movementVector, float delta)
{
	

	collisionFlags = controller->move(convert(movementVector), 0.001f, delta, PxControllerFilters());
	PxExtendedVec3 physxCalculatedPosition = controller->getPosition();

	physxCalculatedPosition.y += controller->getContactOffset();
	this->setPosition(convert(physxCalculatedPosition));
	return glm::vec3(physxCalculatedPosition.x, physxCalculatedPosition.y, physxCalculatedPosition.z);
}

void Character::initPhysx(PhysxMaster* physxMaster)
{
	PxCapsuleControllerDesc desc;
	desc.position = PxExtendedVec3(getPosition().x, getPosition().y, getPosition().z); //initial position
	desc.contactOffset = 0.1f; //controller skin within which contacts generated
	desc.stepOffset = 0.2f; //max obstacle height the character can climb
	desc.slopeLimit = cosf(glm::radians(45.0f)); // max slope the character can walk
	desc.radius = 1.0f; //radius of the capsule
	desc.height = 4; //height of the controller
	desc.upDirection = PxVec3(0, 1, 0); // Specifies the 'up' direction
	desc.material = physxMaster->getMaterial();

	controller = physxMaster->createCapsuleController(&desc);
}

Character::Character()
{
}

Character::Character(glm::vec3 position, PhysxMaster* physxMaster)
{
	this->setPosition(position);

	initPhysx(physxMaster);
}
