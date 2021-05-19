#pragma once
#include "utils/Utils.h"
#include "KeyFrame.h"
#include "JointAnimation.h"
#include "AnimationKeyFrame.h"
class Animation
{
private:
	string name;
	float length; //seconds
	std::vector<JointAnimation> jointAnimations;
	int numberOfKeyFrames;
public:
	enum ANIMATION {
		WALK,
		RUN
	};
	Animation();
	Animation(string name,float lengthInSeconds, std::vector<JointAnimation> jointAnimations, int numberOfKeyFrames);

	float getlength();
	std::vector<JointAnimation> getJointAnimations();

	AnimationKeyFrame getKeyFrame(int index);
	std::vector<AnimationKeyFrame> getKeyFrames();
	float getTimeStampOfKeyFrame(int index);
	string getName();
};

