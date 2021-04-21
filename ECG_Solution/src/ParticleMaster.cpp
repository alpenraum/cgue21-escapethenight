#include "ParticleMaster.h"

std::vector<Particle*>* ParticleMaster::particles = nullptr;
ParticleRenderer* ParticleMaster::renderer = nullptr;

void ParticleMaster::init()
{
	ParticleMaster::particles = new std::vector<Particle*>();
	ParticleMaster::renderer = new ParticleRenderer();
}

void ParticleMaster::update(glm::vec3 cameraPos, float dt)
{
	auto it = ParticleMaster::particles->begin();
	while (it != ParticleMaster::particles->end()){
		Particle* p = *it;
		bool isAlive = p->update(cameraPos, dt);

		if (!isAlive) {
			it = ParticleMaster::particles->erase(it);
		}
		else {
			++it;
		}
	}
}

void ParticleMaster::renderParticles(ICamera* camera,float dt)
{
	ParticleMaster::renderer->draw(camera, dt, *particles);
}

void ParticleMaster::cleanup()
{
	ParticleMaster::renderer->cleanup();
}

void ParticleMaster::addParticle(Particle* particle)
{
	ParticleMaster::particles->push_back(particle);
	
}
