#include "Animator.h"

std::vector<AnimationKeyFrame> Animator::getPreviousAndNextFrame()
{
	std::vector<AnimationKeyFrame> allFrames = currentAnimation->getKeyFrames();
	AnimationKeyFrame prevFrame = allFrames[0];
	AnimationKeyFrame nextFrame = allFrames[0];
	for (int i = 1; i < allFrames.size(); i++) {
		nextFrame = allFrames[i];
		if (nextFrame.getTimeStamp() > animationTime) {
			break;
		}
		prevFrame = allFrames[i];
	}
	return std::vector<AnimationKeyFrame>{prevFrame, nextFrame};
}

float Animator::calculateProgression(AnimationKeyFrame frameA, AnimationKeyFrame frameB)
{
	float totalTime = frameB.getTimeStamp() - frameA.getTimeStamp();
	float currentTime = animationTime - frameA.getTimeStamp();
	return currentTime / totalTime;
}

void Animator::increaseAnimationTime(float dt)
{
	animationTime += dt;
	if (animationTime > currentAnimation->getlength()) {
		animationTime = std::fmod(animationTime, currentAnimation->getlength());
	}
}

std::map<string, glm::mat4> Animator::interpolatePoses(AnimationKeyFrame previousFrame, AnimationKeyFrame nextFrame, float progression)
{
	std::map<string, glm::mat4> currentPose = std::map<string, glm::mat4>();
	
	std::map<string, JointTransform> previousMap = previousFrame.getJointKeyFrames();
	
	std::map<string, JointTransform> nextMap = nextFrame.getJointKeyFrames();
	std::map<string, JointTransform>::iterator nextIT;


	for (auto const& element : previousMap) {
		string name = element.first.c_str();
		JointTransform previousTransform = element.second;
		JointTransform nextTransform;

		nextIT = nextMap.find(name);
		if (nextIT == nextMap.end()) {
			//notFOunde
		}
		nextTransform = nextIT->second;

		JointTransform currentTransform = JointTransform::interpolate(previousTransform, nextTransform, progression);
		currentPose.insert(std::make_pair( name, currentTransform.getLocalTransform() ) );
	}
	return currentPose;
}

std::map<string, glm::mat4> Animator::calculateCurrentAnimationPose()
{
	std::vector<AnimationKeyFrame> frames = getPreviousAndNextFrame();
	float progression = calculateProgression(frames[0], frames[1]);
	return interpolatePoses(frames[0], frames[1], progression);
}

void Animator::applyPoseToJoints(std::map<string, glm::mat4> currentPose, Joint* joint, glm::mat4 parentTransform)
{

	glm::mat4 localBoneTransform = joint->getLocalBindTransform();
	if (currentPose.find(joint->name) != currentPose.end()) {
			localBoneTransform = currentPose[joint->name];
		
	}

	glm::mat4 globalTransform = ((parentTransform) * (localBoneTransform));
	

	joint->setAnimatedTransform(globalTransform * joint->getOffsetTransform());

	for each (Joint * childJoint in joint->children) {
		applyPoseToJoints(currentPose, childJoint, globalTransform);
	}
	
}

Animator::Animator()
{
	this->entity = nullptr;
	currentAnimation = nullptr;
}

Animator::Animator(AnimatedModel* entity)
{
	currentAnimation = nullptr;
	this->entity = entity;
}

void Animator::doAnimation(Animation animation)
{
	this->animationTime = 0;
	this->currentAnimation = new Animation(animation);
}

void Animator::update(float dt, Joint* rootJoint)
{
	if (!currentAnimation) {
		return;
	}
	increaseAnimationTime(dt);
	std::map<string, glm::mat4> currentPose = calculateCurrentAnimationPose();
	applyPoseToJoints(currentPose, rootJoint, glm::mat4());
}
