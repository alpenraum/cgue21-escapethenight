#include "CampFire.h"



CampFire::CampFire()
{
}

CampFire::CampFire(string const& path, glm::vec3 position, glm::vec3 scale) {

    this->setPosition(position);
    ps = FireParticleSystem();
    model = Model(path, position, scale);

    glm::vec3 lightPos = glm::vec3(position);
    lightPos.y += 0.5f;
    light = PointLight(glm::normalize(glm::vec3(255, 128, 0)*4.0f), this->getPosition(), glm::vec3(1.0f, 0.14f, 0.07f));
    light.castShadow = true;

}



void CampFire::updateParticles(float dt)
{
    //std::cout << "LIGHT: ENABLED: " << light.enabled << std::endl;

    this->ps.generateParticles(getPosition(), dt);
}
