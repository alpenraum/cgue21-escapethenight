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

bool Particle::update(float dt)
{
	velocity.y += GRAVITY * gravityEffect * dt;

	glm::vec3 movement = glm::vec3(velocity);
	movement *= dt;
	position += movement;

	elapsedTime += dt;
	return elapsedTime < lifeLength;

}
