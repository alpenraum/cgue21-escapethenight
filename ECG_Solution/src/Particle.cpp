#include "Particle.h"
#include "ParticleMaster.h"

Particle::Particle()
{
}

Particle::Particle(glm::vec3 position, glm::vec3 velocity, glm::vec3 gravity, float gravityEffect, float lifeLength, float scale)
{
	this->origin = position;
	this->position = position;
	this->direction = velocity;
	this->gravityEffect = gravityEffect;
	this->lifeLength = lifeLength;
	this->gravity = gravity;
	this->scale = scale;

	ParticleMaster::addParticle(this);
}

glm::vec3 Particle::getPosition()
{
	return position;
}

float Particle::getRotation()
{
	return rotation;
}

float Particle::getScale()
{
	return scale;
}

float Particle::getAlpha()
{
	return alpha;
}

float Particle::getDistToCamera()
{
	return distSquaredToCamera;
}

glm::vec3 Particle::getHue() {
	return hue;
}

bool Particle::update(glm::vec3 cameraPos, float dt)
{
	glm::vec3 vecToCam = cameraPos - position;
	distSquaredToCamera = glm::dot(vecToCam, vecToCam);
	
	glm::vec3 movement = glm::vec3(direction);
	movement *= dt;
	position += movement;

	direction += gravity * gravityEffect * dt;

//pull particles towards the center, like a flame
	if ((position.x > origin.x) && (position.y > (0.1f + origin.y))) {
		gravity.x = -2.0f;
	}
	else if ((position.x < origin.x) && (position.y > (0.1f + origin.y))) {
		gravity.x = 2.0f;
	}
	else {
		gravity.x = 0.0f;
	}

	if ((position.z > origin.z) && (position.y > (0.1f + origin.y))) {
		gravity.z = -2.0f;
	}
	else if ((position.z < origin.z) && (position.y > (0.1f + origin.y))) {
		gravity.z = 2.0f;
	}
	else {
		gravity.z = 0.0f;
	}


	elapsedTime += dt;

	float lifeDelta = (elapsedTime / lifeLength);

	if (lifeDelta < 0.1f) {
		hue = glm::vec3(WHITE);
	}
	else if (lifeDelta >= 0.1f && lifeDelta < 0.25f) {
		hue = glm::vec3(BLUE);
	}
	else if (lifeDelta >= 0.25f && lifeDelta < 0.4f) {
		hue = glm::vec3(YELLOW);
	}
	else if (lifeDelta >= 0.4f && lifeDelta < 0.7f) {
		hue = glm::vec3(ORANGE);
	}
	else if (lifeDelta >= 0.7f) {
		hue = glm::vec3(RED);
	}




	alpha = 1 - lifeDelta ;

	return elapsedTime < lifeLength;

}
