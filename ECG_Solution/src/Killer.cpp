#include "Killer.h"
#include "SoundManager.h"

Killer::Killer()
{
	this->setPosition(glm::vec3(30.0f, 4.0f, 10.0f));
	movementGoal = this->getPosition();
}
Killer::Killer(glm::vec3 position, PhysxMaster* physxMaster) : Character(position,physxMaster) {
	
	model = AnimatedModel("assets/models/killer/killer_classic_noroot.dae", this->getPosition(), glm::vec3(1.0f));
	//this->transform.setRotation(glm::quat(glm::vec3(glm::pi<float>(), 0, 0)));
	movementGoal = this->getPosition();
	model.doAnimation(Animation::WALK);
	//SoundManager::playEvent("event:/Killer Audio");
}

void Killer::update(Player& player, bool playerNearLight, float dt)
{
	timePassedSinceUpdate += dt;
	bool playerInSight = true;
	std::srand(std::time(nullptr));
	float distToPlayer = glm::abs(glm::distance(this->getPosition(), player.getPosition()));
	float speed = playerNearLight? ATTACK_SPEED : MOVEMENT_SPEED;
	if (this->timePassedSinceUpdate >= UPDATE_TICK || playerNearLight || distToPlayer<=15.0f) { //only calculates the goal position once the old one has been reach or the player is visible)
		timePassedSinceUpdate = 0.0f;
		movementGoal = player.getPosition();
		movementGoal.y = 0.0f;
		
		if (!playerNearLight && distToPlayer>5.0f ) { //killer walks around randomly when player not visible
			playerInSight = false;
			movementGoal.x = movementGoal.x * 1.5f * (std::sin(std::rand() % 100));
			movementGoal.z = movementGoal.z * 1.5f * (std::sin(std::rand() % 100));
		}
		
	}


	glm::vec3 movementVectorNormalized = glm::normalize(movementGoal - this->getPosition());
	glm::vec3 movementVectorSpeed = movementVectorNormalized * speed * dt;

	
	
	this->setPosition(this->getPosition() + movementVectorSpeed);


	updatePhysx(movementVectorSpeed, dt);
	movementVectorNormalized.y = 0.0f;
	movementVectorNormalized = glm::normalize(movementVectorNormalized);
	glm::quat quat = this->rotateBetweenVectors(normalizedForwardVector, movementVectorNormalized);
	
	quat = this->rotateTowards(this->transform.getRotation(), quat, glm::pi<float>()/2.0f * dt);
	this->transform.setRotation(quat);


	model.update(dt);

	playerInSightLastFrame = playerInSight;

	//SoundManager::setEvent3dPosition("event:/Killer Audio", this->getPosition());
}

void Killer::draw(ICamera* camera, glm::vec4 clippingPlane, bool lightMapping, bool normalMapping, std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights)
{
	glm::vec3 pos = this->getPosition();
	model.setPosition(pos);
	model.setRotation(this->transform.getRotation() * glm::quat(glm::vec3(-glm::pi<float>()/2.0f, 0, 0)));
	

	renderer.prepareRender(camera, clippingPlane, lightMapping, normalMapping, dirLights, pointLights);
	renderer.render(&model, camera);
	renderer.finish();
}

void Killer::drawShadows(AdvancedShader* shader)
{
	/*
	glm::vec3 pos = this->getPosition();
	model.setPosition(pos);
	model.setRotation(this->transform.getRotation()* glm::quat(glm::vec3(-glm::pi<float>() / 2.0f, 0, 0)));
	model.draw(shader);
	*/
}

void Killer::resetKiller() {
	Character::setPosition(glm::vec3(30.0f, 4.0f, 10.0f));
	movementGoal = this->getPosition();
}

