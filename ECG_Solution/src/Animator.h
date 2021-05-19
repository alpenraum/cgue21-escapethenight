#pragma once
#include "utils/Utils.h"
#include "AnimatedModel.h"
class AnimatedModel;
class Animator
{
private:
	AnimatedModel* entity;

	Animation* currentAnimation;
	float animationTime = 0;

	std::vector<AnimationKeyFrame> getPreviousAndNextFrame();
	float calculateProgression(AnimationKeyFrame frameA, AnimationKeyFrame frameB);
	void increaseAnimationTime(float dt);
	std::map<string, glm::mat4> interpolatePoses(AnimationKeyFrame previousFrame, AnimationKeyFrame nextFrame, float progression);
	std::map<string, glm::mat4> calculateCurrentAnimationPose();
	void applyPoseToJoints(std::map<string, glm::mat4>currentPose, Joint* joint, glm::mat4 parentTransform);
	
public:
	Animator();
	Animator(AnimatedModel* entity);

	void doAnimation(Animation animation);
	void update(float dt, Joint* rootJoint);

};

