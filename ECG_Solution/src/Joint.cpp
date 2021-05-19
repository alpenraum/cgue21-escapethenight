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

glm::mat4 Joint::getInverseBindTransform()
{
	return inverseBindTransform;
}

void Joint::calcInverseBindTransform(glm::mat4 parentBindTransform)
{
	glm::mat4 bindTransform = parentBindTransform * localBindTransform;
	inverseBindTransform = glm::inverse(bindTransform); //==ERROR HERE==
	for each (Joint* child in children)
	{
		child->calcInverseBindTransform(bindTransform);
	}
}
