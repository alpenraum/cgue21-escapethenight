#pragma once
#include "utils/Utils.h"
#include <vector>
#include "Particle.h"
#include "ParticleRenderer.h"

class ParticleMaster {

private:
	static std::vector<Particle*>* particles;
	static ParticleRenderer* renderer;

public:

	static void init();

	static void update(float dt);
	static void renderParticles(ICamera* camera, float dt);
	static void cleanup();

	static void addParticle(Particle* particle);
};