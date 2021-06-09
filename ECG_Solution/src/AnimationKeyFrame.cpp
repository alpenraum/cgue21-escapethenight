#include "AnimationKeyFrame.h"

AnimationKeyFrame::AnimationKeyFrame()
{
}

AnimationKeyFrame::AnimationKeyFrame(float time, std::map<string, JointTransform> transforms)
{
	timeStamp = time;
	pose = transforms;
}

float AnimationKeyFrame::getTimeStamp()
{
	return timeStamp;
}

std::map<string, JointTransform> AnimationKeyFrame::getJointKeyFrames()
{
	return pose;
}
