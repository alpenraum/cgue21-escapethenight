#pragma once
#include "ICamera.h"
class BasicCamera : public ICamera
{
public:
	

	BasicCamera();
	BasicCamera(double fov, double aspect, double nearC, double farC, float mouseSensitivity);
	glm::vec2 getNearFar();
	// constructor with scalar values
	
	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix();

	glm::mat4 getViewProjMatrix();
	glm::mat4 getProjMatrix();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 val);
	void invertPitch();

	float getYaw() { return _yaw; };
	float getPitch() { return _pitch; }
	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(unsigned int direction, float deltaTime, bool shift);

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(glm::vec2,float deltaTime);

	void updateCamera();
private:
	void updateCameraVectors();

	glm::mat4 _viewMatrix = glm::mat4(1);
	glm::mat4 _projMatrix = glm::mat4(1);
	int _mouseX=0, _mouseY=0;

	glm::vec3 _position = glm::vec3(0.0f,10.0f,10.0f);
	glm::vec3 _forward = glm::vec3(0);
	glm::vec3 _right = glm::vec3(0);
	glm::vec3 _up = glm::vec3(0);
	float _yaw = 0.0f;
	float _pitch = 0.0f;

	float _movementSpeed = 5.0f;
	float _runningSpeed = 15.0f;
	float _mouseSensitivity = 0.2f;
	
};

