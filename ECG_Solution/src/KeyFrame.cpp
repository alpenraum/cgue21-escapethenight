#include "KeyFrame.h"

KeyFrame::KeyFrame()
{
	timeStamp = 0;
	transform = JointTransform();
}

KeyFrame::KeyFrame(float timestamp, JointTransform transform)
{
	this->timeStamp = timestamp;
	this->transform = transform;
}

float KeyFrame::getTimeStamp()
{
	return timeStamp;
}

JointTransform KeyFrame::getTransform()
{
	return transform;
}

