#include "FireParticleSystem.h"

void FireParticleSystem::emitParticle(glm::vec3 origin)
{
	glm::vec3 direction = glm::vec3(0.0f);
	direction = generateRandomUnitVectorWithError(direction, directionDeviation);
	direction *= generateValue(averageSpeed, speedError);
	float scale = generateValue(averageScale, scaleError);
	float lifeLength = generateValue(averageLifeLength, lifeError);

	glm::vec3 gravity = glm::vec3(0.0f, 0.8f, 0.0f);

	new Particle(glm::vec3(origin), direction,gravity, particleWeight, lifeLength, scale);
}

float FireParticleSystem::generateValue(float average, float error)
{
	auto distribution = std::uniform_real_distribution<float>(-1.0f, 1.0f);
	float offset = (distribution(generator)) * 2.0f * error;
	return average + offset;
}

glm::vec3 FireParticleSystem::generateRandomUnitVectorWithError(glm::vec3 coneDirection, float angle)
{
	glm::vec3 result = glm::vec3(0);
	auto distribution = std::uniform_real_distribution<float>(-1.0f, 1.0f);
	result.x = /*coneDirection.x * angle + (1.0f - angle) * */(float(distribution(generator)) * 10.0f);
	result.y = /*coneDirection.y * angle + (1.0f - angle) * */(float(distribution(generator)+1.0f) * 10.0f);
	result.z = /*coneDirection.z * angle + (1.0f - angle) * */(float(distribution(generator)) * 10.0f);
	
	return glm::normalize(result);
}

FireParticleSystem::FireParticleSystem()
{

	//TODO CREATE PS WITH THE CODE FROM FIREPARTICLES DEMO
	pps = 100.0f;
	averageSpeed = 2.0f;
	particleWeight = 1.0f;
	averageLifeLength = 2.0f;
	averageScale = 0.2f;

	//setDirection(glm::vec3(0.0f,1.0f, 0.0f), 0.0f);
	setSpeedError(0.5f);
	setScaleError(0.5f);
	setLifeError(0.5f);

	
}

void FireParticleSystem::setDirection(glm::vec3 direction, float deviation)
{
	this->direction = glm::vec3(direction);
	this->directionDeviation = deviation * glm::pi<float>();
}

void FireParticleSystem::setSpeedError(float error)
{
	this->speedError = error * averageSpeed;
}

void FireParticleSystem::setLifeError(float error)
{
	this->lifeError = error * averageLifeLength;
}

void FireParticleSystem::setScaleError(float error)
{
	this->scaleError = error * averageScale;
}

void FireParticleSystem::generateParticles(glm::vec3 systemOrigin, float dt)
{
	float particlesToCreate = pps * dt;
	int particleCount = (int) std::floorf(particlesToCreate);
	float partialParticle =std::fmodf(particlesToCreate,1.0f);
	//std::srand(std::time(nullptr));
	for (int i = 0; i < particleCount; i++) {
		emitParticle(systemOrigin);
	}
	if (((float)std::rand()) / ((float)RAND_MAX) < partialParticle) {
		emitParticle(systemOrigin);
	}
}

