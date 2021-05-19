#include "JointTransform.h"

JointTransform::JointTransform()
{
}

JointTransform::JointTransform(glm::vec3 position, glm::quat rotation)
{
	this->position = position;
	this->rotation = rotation;
}

glm::mat4 JointTransform::getLocalTransform()
{
	glm::mat4 mat = glm::mat4();
	mat = glm::translate(mat, position);
	mat *= glm::toMat4(rotation);
	return mat;
}

JointTransform JointTransform::interpolate(JointTransform frameA, JointTransform frameB, float progression)
{
	glm::vec3 pos = interpolate(frameA.position, frameB.position, progression);
	glm::quat rot = interpolate(frameA.rotation, frameB.rotation, progression);
	return JointTransform(pos,rot);
}

glm::vec3 JointTransform::interpolate(glm::vec3 start, glm::vec3 end, float progression)
{
	float x = start.x + (end.x - start.x) * progression;
	float y = start.y + (end.y - start.y) * progression;
	float z = start.z + (end.z - start.z) * progression;

	return glm::vec3(x, y, z);
}

glm::quat JointTransform::interpolate(glm::quat start, glm::quat end, float progression)
{
	glm::quat result = glm::quat(0, 0, 0, 1);
	float dot = start.w * end.w + start.x * end.x + start.y * end.y + start.z * end.z;
	float blendI = 1.0f - progression;

	if (dot < 0) {
		result.w = blendI * start.w + progression * -end.w;
		result.x = blendI * start.x + progression * -end.x;
		result.y = blendI * start.y + progression * -end.y;
		result.z = blendI * start.z + progression * -end.z;
	}
	else {
		result.w = blendI * start.w + progression * end.w;
		result.x = blendI * start.x + progression * end.x;
		result.y = blendI * start.y + progression * end.y;
		result.z = blendI * start.z + progression * end.z;
	}

	result = glm::normalize(result);
	return result;
}
