#include "BasicCamera.h"

BasicCamera::BasicCamera()
{
}

BasicCamera::BasicCamera(double fov, double aspect, double nearC, double farC, float mouseSensitivity)
{	
	_nearC = nearC;
	_farC = farC;
	_projMatrix = glm::perspective(glm::radians(fov), aspect, nearC, farC);
	_mouseSensitivity = mouseSensitivity;
	_yaw = 0.0f;
	_pitch = 0.0f;
	updateCameraVectors();
}

glm::vec2 BasicCamera::getNearFar() {
	return glm::vec2(_nearC, _farC);
}

glm::mat4 BasicCamera::getViewMatrix(){
	return this->_viewMatrix;
}
glm::mat4 BasicCamera::getViewProjMatrix() {
	return _projMatrix * _viewMatrix;
}
glm::mat4 BasicCamera::getProjMatrix() {
	return _projMatrix;
}

glm::vec3 BasicCamera::getPosition()
{
	return _position;
}

glm::vec3 BasicCamera::getForward()
{
	return _forward;
}

glm::vec3 BasicCamera::getRight()
{
	return _right;
}

glm::vec3 BasicCamera::getUp()
{
	return _up;
}

void BasicCamera::setPosition(glm::vec3 val)
{
	this->_position = val;
	updateCamera();
}

void BasicCamera::invertPitch()
{
	this->_pitch = -_pitch;
	updateCamera();
}

void BasicCamera::ProcessKeyboard(unsigned int direction, float deltaTime, bool shift)
{
	float velocity = 0.0f;
	if (shift) {
		velocity = _runningSpeed * deltaTime;
	}
	else {
		velocity = _movementSpeed * deltaTime;
	}
	//each bit corresponds to a direction. if 1, the camera moves in that direction
	if (CHECK_BIT(direction,0))
		_position += _forward * velocity;
	if (CHECK_BIT(direction, 1))
		_position -= _forward * velocity;
	if (CHECK_BIT(direction, 2))
		_position -= _right * velocity;
	if (CHECK_BIT(direction, 3))
		_position += _right * velocity;

	
}

void BasicCamera::ProcessMouseMovement(glm::vec2 mouseDelta, float deltaTime)
{
	mouseDelta *= _mouseSensitivity;
	
	_yaw += mouseDelta.x * deltaTime;
	_pitch += -mouseDelta.y *deltaTime;

	
	// make sure that when pitch is out of bounds, screen doesn't get flipped

	if (_pitch > (glm::radians(89.0f)))
		_pitch = glm::radians(89.0f);
	if (_pitch < -(glm::radians(89.0f)))
		_pitch = -(glm::radians(89.0f));

	
	//LOG_TO_CONSOLE("xPos", _pitch);
}

void BasicCamera::updateCamera()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(_yaw) * cos(_pitch);
	front.y = sin(_pitch);
	front.z = sin(_yaw) * cos(_pitch);
	_forward = glm::normalize(front);
	// also re-calculate the Right and Up vector
	_right = glm::normalize(glm::cross(_forward, glm::vec3(0.0f,1.0f,0.0f))); 
	_up = glm::normalize(glm::cross(_right,_forward));

	updateCameraVectors();
}

void BasicCamera::updateCamera(float yaw, float pitch, glm::vec3 position) {
	_yaw = yaw;
	_pitch = pitch;
	_position = position;
	updateCamera();
}

void BasicCamera::updateCameraVectors()
{
	/*glm::vec3 front;
	front.x = std::cos(_yaw) * std::cos(_pitch);
	front.y = std::sin(_pitch);
	front.z = std::sin(_yaw) * std::cos(_pitch);
	_front = glm::normalize(front);

	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));*/

	/*
	glm::mat4 matPitch = glm::mat4(1.0f);
	glm::mat4 matYaw = glm::mat4(1.0f);

	matPitch = glm::rotate(matPitch, _pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	matYaw = glm::rotate(matYaw, _yaw, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotate = matPitch * matYaw;

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -_position);*/

	_viewMatrix = glm::lookAt(_position, _position + _forward, _up);//rotate * translate;
}