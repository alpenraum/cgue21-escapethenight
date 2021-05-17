#pragma once
#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "utils/Transform.h"

/*!
 *An Actor something in the game that has a position and a direction/rotation in the 3D world.
 */
class Actor
{
protected:
	//The current Transformation of this Actor
	Transform transform;

public:
	/*!
	 * Base constructor. Initialises with zero rotation and translation.
	 */
	Actor();

	/*!
	 * Creates new Actor with the given position and zero rotation.
	 * @param position the initial position of the new Actor
	 */
	Actor(glm::vec3 position);

	~Actor();

	/*!
	 * Returns the current position of this Actor.
	 * @return the current position of this Actor
	 */
	glm::vec3 getPosition();
	void setPosition(glm::vec3 pos);

	/*!
	 * Returns the current transformation matrix for this Actor.
	 * @return the current transformation matrix for this Actor
	 */
	glm::mat4 getTransMatrix();
	Transform* getTransformation();
	/*!
	 * Updates the state of the Actor. Gets called (in)directly in the gameloop. This implementation is empty. Subclasses may override this function to define thier update behaviour.
	 * @param delta the time that has passed since the last update.
	 */
	virtual void update(float delta);

	glm::quat rotateBetweenVectors(glm::vec3 start, glm::vec3 dest);
	glm::quat rotateTowards(glm::quat q1, glm::quat q2, float maxAngle);
};
