#pragma once
#include "Actor.h"
#include "utils/Utils.h"
#include "BasicCamera.h"
#include "Model.h"
class Player : 
    public Actor
{

private:
    ICamera* camera = nullptr;
    Model playerModel;
    //TODO FINISH PLAYER CLASS
};

