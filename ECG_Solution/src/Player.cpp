#include "Player.h"

Player::Player()
{
	camera = new BasicCamera(Settings::fov, ((double)Settings::width / (double)Settings::height), Settings::nearPlane, Settings::farPlane, Settings::mouseSens);
	
	this->setPosition(glm::vec3(0.0f, 3.0f, 10.0f));

	hand = PlayerHand(getPosition());
}

void Player::update(unsigned int movementDirection, glm::vec2 mouseDelta, float delta, std::vector<CampFire*>* campfires)
{

	float velocity = SPEED * delta;
	glm::vec3 pos = camera->getPosition();

	if (movementDirection != 0) {
		
		glm::vec3 forward = camera->getForward();
		glm::vec3 right = camera->getRight();
		if (CHECK_BIT(movementDirection, 0))
			pos += forward * velocity;
		if (CHECK_BIT(movementDirection, 1))
			pos -= forward * velocity;
		if (CHECK_BIT(movementDirection, 2))
			pos -= right * velocity;
		if (CHECK_BIT(movementDirection, 3))
			pos += right * velocity;
	}
	//ALL THIS NEEDS TO BE REWRITTEN WHEN PHYSX CONTROLS THE PLAYER POSITION

	this->setPosition(pos);
	camera->setPosition(pos);

	mouseDelta *= Settings::mouseSens;

	float yaw = camera->getYaw();
	float pitch = camera->getPitch();
	yaw += mouseDelta.x * delta;
	pitch += -mouseDelta.y * delta;

	if (pitch > (glm::radians(89.0f)))
		pitch = glm::radians(89.0f);
	if (pitch < -(glm::radians(89.0f)))
		pitch = -(glm::radians(89.0f));

	camera->updateCamera(yaw, pitch, pos);




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
	for each (CampFire *f in *campfires)
	{
		dist = glm::abs(glm::distance(this->getPosition(),f->getPosition()));

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
	lightsource = PointLight(glm::normalize(glm::vec3(1.0f,0.4f,0.1f)) * 2.0f, lightPos,glm::vec3(1.0f,0.09f,0.032f));
	lightsource.toggleShadows();
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
