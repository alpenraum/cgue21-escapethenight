#pragma once
#include "utils/Utils.h"
#include <ctime> 
#include "Particle.h"

class FireParticleSystem {
private:
	float pps, averageSpeed, particleWeight, averageLifeLength, averageScale;
	float speedError, lifeError, scaleError;
	glm::vec3 direction;
	float directionDeviation;

	void emitParticle(glm::vec3 origin);

	float generateValue(float average, float error);
	static glm::vec3 generateRandomUnitVectorWithError(glm::vec3 direction, float angle);
public:
	FireParticleSystem();

	void setDirection(glm::vec3 direction, float deviation);
	void setSpeedError(float error);
	void setLifeError(float error);
	void setScaleError(float error);

	void generateParticles(glm::vec3 systemOrigin, float dt);


};