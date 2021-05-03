#include "Player.h"

Player::Player(glm::vec3 position, PhysxMaster* physxMaster)
{
	camera = new BasicCamera(Settings::fov, ((double)Settings::width / (double)Settings::height), Settings::nearPlane, Settings::farPlane, Settings::mouseSens);

	this->setPosition(position);

	hand = PlayerHand(getPosition(), physxMaster);

	PxCapsuleControllerDesc desc;
	desc.position = PxExtendedVec3(position.x, position.y, position.z); //initial position
	desc.contactOffset = 0.1f; //controller skin within which contacts generated
	desc.stepOffset = 0.2f; //max obstacle height the character can climb
	desc.slopeLimit = cosf(glm::radians(45.0f)); // max slope the character can walk
	desc.radius = 1.0f; //radius of the capsule
	desc.height = 4; //height of the controller
	desc.upDirection = PxVec3(0, 1, 0); // Specifies the 'up' direction
	desc.material = physxMaster->getMaterial();

	controller = physxMaster->createCapsuleController(&desc);

}

void Player::update(unsigned int movementDirection, glm::vec2 mouseDelta, float delta, std::vector<CampFire*>* campfires)
{
	//PHYSX
	if (collisionFlags.isSet(PxControllerCollisionFlag::eCOLLISION_DOWN)) {
		onGround = true;

	}
	else {
		onGround = false;
	}


	glm::vec3 movement;


	float velocity = SPEED * delta;

	if (movementDirection != 0) {
		
		//TODO UNFUCK THIS
		if (CHECK_BIT(movementDirection, 0)) {
			movement.x += velocity;
			
		}
		
		if (CHECK_BIT(movementDirection, 1)) {
			movement.x -= velocity;
			
		}
		
		if (CHECK_BIT(movementDirection, 2)) {
			movement.z -= velocity;
			
		}
		
		if (CHECK_BIT(movementDirection, 3)) {
			movement.z += velocity;
	
		}
		
	}

	if (!onGround) {
		jumpVelocity -= 0.5f * (2 * delta);
		movement.y += jumpVelocity;
	}

	movement = glm::rotateY(movement, -camera->getYaw());

	collisionFlags = controller->move(convert(movement), 0.001f, delta, PxControllerFilters());
	PxExtendedVec3 physxCalculatedPosition = controller->getPosition();
	//ALL THIS NEEDS TO BE REWRITTEN WHEN PHYSX CONTROLS THE PLAYER POSITION

	physxCalculatedPosition.y += controller->getContactOffset();

	this->setPosition(convert(physxCalculatedPosition));
	PxQuat xQuat = PxQuat(camera->getYaw(), PxVec3(1.0f, 0.0f, 0.0f));
	PxQuat yQuat = PxQuat(camera->getPitch(), PxVec3(0.0f, 1.0f, 0.0f));
	this->transform.setRotation(xQuat * yQuat);


	mouseDelta *= Settings::mouseSens;

	float yaw = camera->getYaw();
	float pitch = camera->getPitch();
	yaw += mouseDelta.x * delta;
	pitch += -mouseDelta.y * delta;

	if (pitch > (glm::radians(89.0f)))
		pitch = glm::radians(89.0f);
	if (pitch < -(glm::radians(89.0f)))
		pitch = -(glm::radians(89.0f));

	camera->updateCamera(yaw, pitch, glm::vec3(physxCalculatedPosition.x, physxCalculatedPosition.y, physxCalculatedPosition.z));




	//sanity

	if (isNearLight(campfires) && movementDirection == Player::NO_MOVEMENT) { //if near light and standing still
		sanity += SANITY_CHANGE_PER_SECOND * delta;
	}
	else if ((isNearLight(campfires) && !movementDirection == Player::NO_MOVEMENT)) {
		//no change in sanity level
	}
	else {
		sanity -= SANITY_CHANGE_PER_SECOND * delta;
	}

	if (sanity > 100.0f) {
		sanity = 100.0f;
	}
	if (sanity < 0.0f) {
		sanity = 0.0f;
	}
}

BasicCamera* Player::getCamera()
{
	return camera;
}

void Player::draw(ICamera* camera, AdvancedShader* shader, float dt)
{
	if (isTorchLit) {

		hand.update(this->getPosition());

		shader->use();


		glm::mat4 view = camera->getViewMatrix();
		glm::mat3 newView = glm::mat3(view);
		newView *= glm::inverse(newView); // m * m^(-1) = I
		view = glm::mat4(newView);

		shader->setUniform("viewProjMatrix", camera->getProjMatrix() * view);
		hand.draw(shader, dt);


		shader->unuse();
	}



}

void Player::jump()
{

	if (onGround) {
		jumpVelocity = 0.5f;
		onGround = false;

	}
}

PointLight* Player::getLight()
{
	return hand.getLight();
}

void Player::toggleTorch()
{
	isTorchLit = !isTorchLit;
	hand.toggleLight();

}

bool Player::isNearLight(std::vector<CampFire*>* campfires)
{
	if (isTorchLit) {
		return true;
	}

	float dist = 0.0f;
	for each (CampFire * f in *campfires)
	{
		dist = glm::abs(glm::distance(this->getPosition(), f->getPosition()));

		if (dist <= LIGHT_RADIUS) {
			return true;
		}
	}

	return false;
}

float Player::getSanity()
{
	return sanity;
}

void Player::resetSanity()
{
	sanity = 100.0f;
}

PlayerHand::PlayerHand()
{
}

PlayerHand::PlayerHand(glm::vec3 playerPos)
{

	torchOffset = glm::vec3(0.2f, -0.10f, -0.2f); //modelspace
	modelHand = Model("assets/models/bullfinch_obj/bullfinch.obj", torchOffset, glm::vec3(0.02f));

	glm::vec3 lightPos = playerPos;
	lightPos.y += 1.5f;
	lightPos += torchOffset;
	lightsource = PointLight(glm::normalize(glm::vec3(1.0f, 0.4f, 0.1f)) * 2.0f, lightPos, glm::vec3(1.0f, 0.09f, 0.032f));
	lightsource.toggleShadows();
}

PlayerHand::PlayerHand(glm::vec3 playerPos, PhysxMaster* physxMaster) : PlayerHand(playerPos)
{

}

PointLight* PlayerHand::getLight()
{
	return &lightsource;
}

void PlayerHand::toggleLight()
{
	lightsource.enabled = !lightsource.enabled;
	lightsource.toggleShadows();
}

bool PlayerHand::isEnabled()
{
	return lightsource.enabled;
}

void PlayerHand::draw(AdvancedShader* shader, float dt)
{
	modelHand.draw(*shader);
}

void PlayerHand::update(glm::vec3 pos)
{
	this->setPosition(pos);
	//pos.y += 1.5f;
	pos.z += -0.5f;
	lightsource.position = pos;
}
