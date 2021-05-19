#pragma once
#include "utils/Utils.h"
#include "JointTransform.h"
class KeyFrame
{
private:
	float timeStamp;
	JointTransform transform;

public:
	KeyFrame();
	KeyFrame(float timestamp, JointTransform transform);

	float getTimeStamp();
	JointTransform getTransform();
};

