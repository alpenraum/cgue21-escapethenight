#pragma once
#include "utils/Utils.h"
//SOURCE: https://www.youtube.com/watch?v=6PkjU9LaDTQ
class Particle {
private:

	const float GRAVITY = -9.8f;

	glm::vec3 position;
	glm::vec3 velocity;
	
	float gravityEffect;
	float lifeLength;
	float rotation;
	float scale;

	float elapsedTime = 0.0f;

public:
	Particle();
	Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale);
	
	glm::vec3 getPosition();
	float getRotation();
	float getScale();

	/*
	returns false if particle has exceeded lifeLength
	*/
	bool update(float dt);
};