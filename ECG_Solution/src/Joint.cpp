#include "Joint.h"

Joint::Joint()
{
}

Joint::Joint(int index, string name, glm::mat4 bindLocalTransform)
{
	this->id = index;
	this->name = name;
	this->localBindTransform = bindLocalTransform;
}

Joint::Joint(int index, string name, glm::mat4 bindLocalTransform, glm::mat4 offsetMatrix):Joint(index, name, bindLocalTransform)
{
	this->offsetMatrix = offsetMatrix;
}

Joint::Joint(int index, string name, glm::mat4 bindLocalTransform, string parentName): Joint(index,name,bindLocalTransform)
{
	this->parentName = parentName;
}

void Joint::addChild(Joint child)
{
	this->children.push_back(new Joint(child));
}

glm::mat4 Joint::getAnimatedTransform()
{
	return animatedTransform;
}

void Joint::setAnimatedTransform(glm::mat4 animatedTransform)
{
	this->animatedTransform = animatedTransform;
}

glm::mat4 Joint::getLocalBindTransform()
{
	return localBindTransform;
}

glm::mat4 Joint::getOffsetTransform()
{
	return offsetMatrix;
}


