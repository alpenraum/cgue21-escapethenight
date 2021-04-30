#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <PxPhysicsAPI.h>
#include "Converter.cpp"
#include "Utils.h"

using namespace physx;

/*!
* saves the position, scale, and rotation of an entity and can calculate the neccessary matrices
*/
class Transform
{
public:

	Transform();
	~Transform();

	/** The local position */
	glm::vec3 position;
	/** The local scale */
	glm::vec3 scale;
	/** The local rotation */
	glm::quat rotation;

	/** Set the local position

	 @param[in] position The position

	 */

	void setPosition(glm::vec3 position);
	void setPosition(PxVec3 position);
	/** Set the local rotation

	 @param[in] rotation The rotation

	 */

	void setRotation(glm::quat rotation);
	void setRotation(PxQuat rotation);
	/** Set the local scale

	 @param[in] scale The scale

	 */

	void setScale(glm::vec3 scale);

	/** Get the local position

	 @return The local position

	 */

	glm::vec3 getPosition();

	/** Get the local scale

	 @return The local scale

	 */

	glm::vec3 getScale();

	/** Get the local rotation

	 @return The local rotation

	 */

	glm::quat getRotation();

	/** Get the full model matrix

	 @return The model matrix

	 */

	glm::mat4 getModelMatrix();

	/** Get the Up vector for this Transform

	 @return The Up vector */

	glm::vec3 getUp();

	/** Get the Forward vector for this Transform

	 @return The Forward vector */

	glm::vec3 getForward();

	/** Get the Right vector for this Transform

	 @return The Right vector */

	glm::vec3 getRight();
};