#pragma once
#include <glm/glm.hpp>
#include <PxPhysicsAPI.h>
#include <glm/gtc/quaternion.hpp>
#include <assimp/vector3.h>
#include <assimp/quaternion.h>
#include <assimp/matrix4x4.h>

inline physx::PxVec2 convert(const glm::vec2& vec2)
{
	return physx::PxVec2(vec2.x, vec2.y);
}

inline physx::PxVec3 convert(const glm::vec3& vec3)
{
	return physx::PxVec3(vec3.x, vec3.y, vec3.z);
}

inline physx::PxExtendedVec3 convert(const glm::dvec3& vec3)
{
	return physx::PxExtendedVec3(vec3.x, vec3.y, vec3.z);
}

inline physx::PxVec4 convert(const glm::vec4& vec4)
{
	return physx::PxVec4(vec4.x, vec4.y, vec4.z, vec4.w);
}

inline physx::PxQuat convert(const glm::quat& quat)
{
	return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
}

inline physx::PxMat33 convert(const glm::mat3& mat3)
{
	physx::PxMat33 pxMat33;
	pxMat33[0][0] = mat3[0][0];
	pxMat33[0][1] = mat3[0][1];
	pxMat33[0][2] = mat3[0][2];
	pxMat33[1][0] = mat3[1][0];
	pxMat33[1][1] = mat3[1][1];
	pxMat33[1][2] = mat3[1][2];
	pxMat33[2][0] = mat3[2][0];
	pxMat33[2][1] = mat3[2][1];
	pxMat33[2][2] = mat3[2][2];
	return pxMat33;
}

inline physx::PxMat44 convert(const glm::mat4& mat4)
{
	physx::PxMat44 pxMat44;
	pxMat44[0][0] = mat4[0][0];
	pxMat44[0][1] = mat4[0][1];
	pxMat44[0][2] = mat4[0][2];
	pxMat44[0][3] = mat4[0][3];
	pxMat44[1][0] = mat4[1][0];
	pxMat44[1][1] = mat4[1][1];
	pxMat44[1][2] = mat4[1][2];
	pxMat44[1][3] = mat4[1][3];
	pxMat44[2][0] = mat4[2][0];
	pxMat44[2][1] = mat4[2][1];
	pxMat44[2][2] = mat4[2][2];
	pxMat44[2][3] = mat4[2][3];
	pxMat44[3][0] = mat4[3][0];
	pxMat44[3][1] = mat4[3][1];
	pxMat44[3][2] = mat4[3][2];
	pxMat44[3][3] = mat4[3][3];
	return pxMat44;
}

inline glm::vec2 convert(const physx::PxVec2& pxVec2)
{
	return glm::vec2(pxVec2.x, pxVec2.y);
}

inline glm::vec3 convert(const physx::PxVec3& pxVec3)
{
	return glm::vec3(pxVec3.x, pxVec3.y, pxVec3.z);
}

inline glm::vec4 convert(const physx::PxVec4& pxVec4)
{
	return glm::vec4(pxVec4.x, pxVec4.y, pxVec4.z, pxVec4.w);
}

inline glm::dvec3 convert(const physx::PxExtendedVec3& pxEVec3)
{
	return glm::dvec3(pxEVec3.x, pxEVec3.y, pxEVec3.z);
}

inline glm::quat convert(const physx::PxQuat& pxQuat)
{
	return glm::quat(pxQuat.w, pxQuat.x, pxQuat.y, pxQuat.z);
}

inline glm::mat3 convert(const physx::PxMat33& pxMat33)
{
	glm::mat3 mat3;
	mat3[0][0] = pxMat33[0][0];
	mat3[0][1] = pxMat33[0][1];
	mat3[0][2] = pxMat33[0][2];
	mat3[1][0] = pxMat33[1][0];
	mat3[1][1] = pxMat33[1][1];
	mat3[1][2] = pxMat33[1][2];
	mat3[2][0] = pxMat33[2][0];
	mat3[2][1] = pxMat33[2][1];
	mat3[2][2] = pxMat33[2][2];
	return mat3;
}

inline glm::mat4 convert(const physx::PxMat44& pxMat44)
{
	glm::mat4 mat4;
	mat4[0][0] = pxMat44[0][0];
	mat4[0][1] = pxMat44[0][1];
	mat4[0][2] = pxMat44[0][2];
	mat4[0][3] = pxMat44[0][3];
	mat4[1][0] = pxMat44[1][0];
	mat4[1][1] = pxMat44[1][1];
	mat4[1][2] = pxMat44[1][2];
	mat4[1][3] = pxMat44[1][3];
	mat4[2][0] = pxMat44[2][0];
	mat4[2][1] = pxMat44[2][1];
	mat4[2][2] = pxMat44[2][2];
	mat4[2][3] = pxMat44[2][3];
	mat4[3][0] = pxMat44[3][0];
	mat4[3][1] = pxMat44[3][1];
	mat4[3][2] = pxMat44[3][2];
	mat4[3][3] = pxMat44[3][3];
	return mat4;
}

inline glm::vec3 convert(const aiVector3D aiVec3D) {
	return glm::vec3(aiVec3D.x, aiVec3D.y, aiVec3D.z);
}

inline glm::mat4 convert(const aiMatrix4x4 aiMat44) {
	return glm::mat4(aiMat44.a1, aiMat44.a2, aiMat44.a3, aiMat44.a4,
		aiMat44.b1, aiMat44.b2, aiMat44.b3, aiMat44.b4,
		aiMat44.c1, aiMat44.c2, aiMat44.c3, aiMat44.c4,
		aiMat44.d1, aiMat44.d2, aiMat44.d3, aiMat44.d4);
}

inline glm::quat convert(const aiQuaternion aiquat) {
	return glm::quat(aiquat.w, aiquat.x, aiquat.y, aiquat.z);
}