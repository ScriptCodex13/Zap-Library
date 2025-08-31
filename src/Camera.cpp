#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp> //matrix functions
namespace zap
{

//#include <glm/glm.hpp>                  //basic glm math functions


// default camera
// Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));//make camera at xyz pos
//     position {0.0f,  0.0f,  3.0f}
//        front {0.0f,  0.0f, -1.0f}
//           up {0.0f,  1.0f,  0.0f}

//     position         {0.0f,  0.0f,  3.0f}   //eye
//     position + front {0.0f,  0.0f,  2.0f}   //center; center - eye = {0, 0, -1}
//                   up {0.0f,  1.0f,  0.0f}

// Default camera values
const float Camera::YAW = -90.0f;
const float Camera::PITCH = 0.0f;
const float Camera::SPEED = 2.5f;
const float Camera::SENSITIVITY = 0.1f;
const float Camera::ZOOM = 45.0f;


Camera::Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch) :
	position(_position), worldUp(_up), yaw(_yaw), pitch(_pitch),
	front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	updateVectors();
}
Camera::Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch) :
	Camera(glm::vec3(_posX, _posY, _posZ), glm::vec3(_upX, _upY, _upZ), _yaw, _pitch)
{
}

glm::mat4 Camera::view()
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::perspective(float znear, float zfar)
{
	return this->perspective(config::windowWidth, config::windowHeight, znear, zfar);
}
glm::mat4 Camera::perspective(float windowWidth, float windowHeight, float znear, float zfar)
{
	return glm::perspective(glm::radians(zoom), windowWidth / windowHeight, znear, zfar);
}
void Camera::onKeyboard(Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	switch (direction)
	{
	case Movement::FORWARD:   position += front * velocity; break;
	case Movement::RIGHT:     position += right * velocity; break;
	case Movement::BACKWARD:  position -= front * velocity; break;
	case Movement::LEFT:      position -= right * velocity; break;
	}
}

void Camera::onMouseMove(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch) //constraint to 90 degreees up/down
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// Update front, right and up Vectors using the updated Euler angles
	updateVectors();
}

void Camera::onMouseScroll(float yoffset)
{
	//If zoom in bounds then update zoom
	if (zoom >= 1.0f && zoom <= 45.0f) zoom -= yoffset;
	//If updated zoom out of bounds then get it back in bounds
	if (zoom <= 1.0f) zoom = 1.0f;
	if (zoom >= 45.0f) zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateVectors()
{
	// Calculate the new front vector
	glm::vec3 _front;
	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(_front);
	// Also re-calculate the right and up vector
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

}