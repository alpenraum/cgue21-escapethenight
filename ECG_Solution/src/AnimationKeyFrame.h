#pragma once
#include "utils/Utils.h"
#include "JointTransform.h"
class AnimationKeyFrame
{
private:
	float timeStamp = 0;
	std::map<string, JointTransform> pose;

public:
	AnimationKeyFrame();
	AnimationKeyFrame(float time, std::map<string, JointTransform>transforms);

	float getTimeStamp();
	std::map<string, JointTransform> getJointKeyFrames();
};

