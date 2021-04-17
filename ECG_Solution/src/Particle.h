#pragma once
#include "utils/Utils.h"
//SOURCE: https://www.youtube.com/watch?v=6PkjU9LaDTQ
class Particle {
private:
	const glm::vec3 WHITE = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 BLUE = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 YELLOW = glm::vec3(1.0f, 1.0f, 0.0f);
	const glm::vec3 ORANGE = glm::vec3(1.0f, 0.5f, 0.0f);
	const glm::vec3 RED = glm::vec3(1.0f, 0.0f, 0.0f);
	

	const float GRAVITY = -9.8f;

	glm::vec3 origin;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 gravity;

	float distSquaredToCamera;
	
	float gravityEffect;
	float lifeLength;
	float rotation;
	float scale;

	float elapsedTime = 0.0f;

	float alpha = 1.0f;

	glm::vec3 hue;

	glm::vec3 mixColours(glm::vec3 c1, glm::vec3 c2, float t);

public:
	Particle();
	Particle(glm::vec3 position, glm::vec3 direction,glm::vec3 gravity, float gravityEffect, float lifeLength, float scale);
	
	glm::vec3 getPosition();
	float getRotation();
	float getScale();
	float getAlpha();
	float getDistToCamera();

	glm::vec3 getHue();

	//MAKE COLOR HUE GO FROM BLUE OVER YELLOW TO ORANGE TO RED

	/*
	returns false if particle has exceeded lifeLength
	*/
	bool update(glm::vec3 cameraPos, float dt);
};