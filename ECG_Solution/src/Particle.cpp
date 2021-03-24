#include "Particle.h"
#include "ParticleMaster.h"

Particle::Particle()
{
}

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale)
{
	this->position = position;
	this->velocity = velocity;
	this->gravityEffect = gravityEffect;
	this->lifeLength = lifeLength;
	this->rotation = rotation;
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

glm::vec3 Particle::getHue() {
	return hue;
}

bool Particle::update(float dt)
{
	velocity.y += GRAVITY * gravityEffect * dt;

	glm::vec3 movement = glm::vec3(velocity);
	movement *= dt;
	position += movement;

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
