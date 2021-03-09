#pragma once
#include "utils/Utils.h"
#include "utils/Transform.h"
/*!
* The interface of cameras
*/
class ICamera {
protected:
	//contains the position and viewing direction of the camera
	Transform spatialData;
	//near plane
	float _nearC;
	//far plane
	float _farC;
public:
	enum Camera_Movement {
		FORWARD = 1 << 0,
		BACKWARD = 1 << 1,
		LEFT = 1 << 2,
		RIGHT = 1 << 3,
		NO_MOVEMENT = 0
	};

	virtual glm::mat4 getViewMatrix() = 0;
	virtual glm::mat4 getProjMatrix() = 0;
	virtual glm::mat4 getViewProjMatrix() = 0;
	virtual glm::vec3 getPosition() = 0;
	virtual glm::vec2 getNearFar() = 0;
	
	virtual float getYaw() = 0;
	virtual float getPitch() = 0;

	virtual void setPosition(glm::vec3 val) = 0;
	virtual void invertPitch()= 0;
};