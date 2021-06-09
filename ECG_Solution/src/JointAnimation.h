#pragma once
#include "utils/Utils.h"
#include "KeyFrame.h"
class JointAnimation
{
private:
	string name;
	std::vector<KeyFrame> frames;

public:

	JointAnimation();
	JointAnimation(string name, std::vector<KeyFrame> frames);

	std::vector<KeyFrame> getFrames();
	string getName();
};

