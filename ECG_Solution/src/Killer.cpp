#include "Killer.h"

Killer::Killer()
{
	this->setPosition(glm::vec3(20.0f, 30.0f, 12.0f));
	model = Model("assets/models/bullfinch_obj/bullfinch_inv.obj", this->getPosition(), glm::vec3(0.5f));
	movementGoal = this->getPosition();
}

void Killer::update(Player& player, bool playerNearLight, float dt)
{
	std::srand(std::time(nullptr));
	float distToPlayer = glm::abs(glm::distance(this->getPosition(), player.getPosition()));
	float speed = playerNearLight? ATTACK_SPEED : MOVEMENT_SPEED;
	if (glm::abs(glm::distance(this->getPosition(), movementGoal)) <= 2.0f || playerNearLight || distToPlayer<=5.0f) { //only calculates the goal position once the old one has been reach or the player is visible)

		movementGoal = player.getPosition();
		
		if (!playerNearLight && distToPlayer>5.0f ) { //killer walks around randomly when player not visible
			movementGoal.x = movementGoal.x * 1.5f * (std::sin(std::rand() % 100));
			movementGoal.z = movementGoal.z * 1.5f * (std::sin(std::rand() % 100));
		}
	}
	glm::vec3 movementVector = glm::normalize(movementGoal - this->getPosition());

	this->setPosition(this->getPosition() + movementVector * speed * dt);
	
	
}

void Killer::draw(AdvancedShader* shader)
{
	model.setPosition(this->getPosition());

	shader->use();
	model.draw(*shader);
	shader->unuse();
}
