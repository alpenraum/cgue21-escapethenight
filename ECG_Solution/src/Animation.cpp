#include "Animation.h"

Animation::Animation()
{
	name = "";
	length = 0;
	jointAnimations = std::vector<JointAnimation>();
}

Animation::Animation(string name,float lengthInSeconds, std::vector<JointAnimation> jointAnimations, int numberOfKeyFrames)
{
	this->name = name;
	this->length = lengthInSeconds;
	this->jointAnimations = jointAnimations;
	this->numberOfKeyFrames = numberOfKeyFrames;
}

float Animation::getlength()
{
	return length;
}

std::vector<JointAnimation> Animation::getJointAnimations()
{
	return jointAnimations;
}

AnimationKeyFrame Animation::getKeyFrame(int index)
{
	std::map<string, JointTransform> keyFrame = std::map<string, JointTransform>();
	float timeStamp = 0;
	for each (JointAnimation jT in jointAnimations) {
		keyFrame.insert(std::make_pair(jT.getName(), jT.getFrames().at(index).getTransform()));
		
	}
	timeStamp = jointAnimations.at(0).getFrames().at(index).getTimeStamp();

	return AnimationKeyFrame(timeStamp, keyFrame);;
}

std::vector<AnimationKeyFrame> Animation::getKeyFrames()
{
	std::vector<AnimationKeyFrame> keyFrames = std::vector<AnimationKeyFrame>();

	for (int i = 0; i < numberOfKeyFrames; i++) {
		keyFrames.push_back(getKeyFrame(i));
	}

	return keyFrames;
}

float Animation::getTimeStampOfKeyFrame(int index)
{
	return jointAnimations[0].getFrames().at(index).getTimeStamp();
}

string Animation::getName()
{
	return name;
}
