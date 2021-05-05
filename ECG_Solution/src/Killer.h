#pragma once
#include "Actor.h"
#include "Model.h"
#include "Player.h"
#include <ctime> 
class Killer : public Character
{
private:
	Model model;

	const float MOVEMENT_SPEED = 2.0f;
	const float ATTACK_SPEED = 3.5f;

	glm::vec3 movementGoal;

	const glm::vec3 normalizedForwardVector = glm::vec3(0.0f,0.0f,1.0f);
public:
	Killer();
	Killer(glm::vec3 position, PhysxMaster* physxMaster);

	void update(Player& player, bool playerNearLight, float dt);

	void draw(AdvancedShader* shader);

	void resetKiller();

	
};
