#pragma once
#include "Model.h"
#include "FireParticleSystem.h"
#include "Light.h"
#include "utils/Utils.h"
#include "BasicCamera.h"
class CampFire :
    public Actor
{

private:

    FireParticleSystem ps;
    PointLight light;
    Model model;


public:
    CampFire();
    CampFire(string const& path, glm::vec3 position, glm::vec3 scale);

   // void draw(ICamera* camera,AdvancedShader* shader, float dt);


    //Has no own render method, the model needs to be given to the LevelRenderer, the light to the list of pointlights, all within the WorldRenderer
    void updateParticles(float dt);
    PointLight* getLight() { return &light; }
    Model* getModel() { return &model; }
};

