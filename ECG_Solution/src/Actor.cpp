#include "Actor.h"



Actor::Actor(): transform()
{
}

Actor::~Actor()
{
}

glm::vec3 Actor::getPosition()
{
	return transform.getPosition();
}
Transform Actor::getTransformation() {
	return this->transform;
}

glm::mat4 Actor::getTransform()
{
	return transform.getModelMatrix();
}

void Actor::update(float delta)
{
}
