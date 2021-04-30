#pragma once
#include "utils/Utils.h"
#include <ctime> 
#include <functional>
#include <random>
#include "Particle.h"

class FireParticleSystem {
private:
	std::default_random_engine generator;
	
	float pps, averageSpeed, particleWeight, averageLifeLength, averageScale;
	float speedError, lifeError, scaleError;
	glm::vec3 direction;
	float directionDeviation;

	void emitParticle(glm::vec3 origin);

	float generateValue(float average, float error);
	glm::vec3 generateRandomUnitVectorWithError(glm::vec3 direction, float angle);
public:
	FireParticleSystem();

	FireParticleSystem(float pps, float averageSpeed, float particleWeight, float averageLifeLength, float averageScale);

	void setDirection(glm::vec3 direction, float deviation);
	void setSpeedError(float error);
	void setLifeError(float error);
	void setScaleError(float error);

	void generateParticles(glm::vec3 systemOrigin, float dt);


};