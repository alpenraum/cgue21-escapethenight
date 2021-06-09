#pragma once
#include "utils/Utils.h"
#include "PxPhysicsAPI.h"
#include <glm/gtx/quaternion.hpp>
class JointTransform
{
private:
	glm::vec3 position;
	glm::quat rotation;

public:
	JointTransform();
	JointTransform(glm::vec3 position, glm::quat rotation);

	glm::mat4 getLocalTransform();
	
	static JointTransform interpolate(JointTransform frameA, JointTransform frameB, float progression);
	static glm::vec3 interpolate(glm::vec3 start, glm::vec3 end, float progression);
	static glm::quat interpolate(glm::quat start, glm::quat end, float progression);
};

