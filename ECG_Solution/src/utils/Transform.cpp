#include "Transform.h"
#include "Utils.h"




Transform::Transform() : position(0.0f), rotation(), scale(1.0f)
{}



Transform::~Transform()
{}


void Transform::setPosition(glm::vec3 position)
{

	this->position = position;
}
void Transform::setPosition(PxVec3 position) {
	setPosition(PhysXVec3ToglmVec3(position));
}


void Transform::setScale(glm::vec3 scale)

{

	this->scale = scale;
}




glm::vec3 Transform::getPosition()

{

	return position;

}




glm::quat Transform::getRotation()

{

	return rotation;

}

void Transform::setRotation(glm::quat rotation)

{

	this->rotation = rotation;

}
void Transform::setRotation(PxQuat rotation) {
	this->setRotation(PhysXQuatToglmQuat(rotation));
}


glm::vec3 Transform::getScale()

{

	return scale;

}





glm::mat4 Transform::getModelMatrix()

{
	return glm::translate(position) * (glm::mat4)(rotation) * glm::scale(scale);
}





/** Get the Up vector for this Transform

 @return The Up vector */

glm::vec3 Transform::getUp()

{

	glm::vec4 v = getModelMatrix() * glm::vec4(0, 1, 0, 0);

	return glm::normalize(glm::vec3(v.x, v.y, v.z));



}

/** Get the Forward vector for this Transform

 @return The Forward vector */

glm::vec3 Transform::getForward()

{

	glm::vec4 v = getModelMatrix() * glm::vec4(0, 0, 1, 0);

	return glm::normalize(glm::vec3(v.x, v.y, v.z));

}

/** Get the Right vector for this Transform

 @return The Right vector */

glm::vec3 Transform::getRight()

{

	glm::vec4 v = getModelMatrix() * glm::vec4(1, 0, 0, 0);

	return glm::normalize(glm::vec3(v.x, v.y, v.z));

}