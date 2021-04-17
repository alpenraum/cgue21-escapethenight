#pragma once
#include "Actor.h"
#include "Model.h"
#include "Player.h"
#include <ctime> 
class Killer : public Actor
{
private:
	Model model;

	const float MOVEMENT_SPEED = 2.0f;
	const float ATTACK_SPEED = 3.5f;

	//glm::vec3 movementVector;
	glm::vec3 movementGoal;
public:
	Killer();

	void update(Player& player, bool playerNearLight, float dt);

	void draw(AdvancedShader* shader);

	void resetKiller();
};
