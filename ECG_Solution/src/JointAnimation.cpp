#include "JointAnimation.h"

JointAnimation::JointAnimation()
{
	name = "empty";
}

JointAnimation::JointAnimation(string name,std::vector<KeyFrame> frames)
{
	this->name = name;
	this->frames = frames;
}

std::vector<KeyFrame> JointAnimation::getFrames()
{
	return frames;
}

string JointAnimation::getName()
{
	return name;
}
