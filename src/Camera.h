<<<<<<< HEAD
#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GLFW/glfw3.h> //for GLtypes
#include <glm/glm.hpp>
namespace zap
{

class Camera
{

public:
	enum class Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	glm::vec3 position, front, up, right, worldUp;

	float yaw, pitch; // Euler Angles yaw/pitch/roll (down y/right x/ forward z axes respectively)
	float movementSpeed, mouseSensitivity;
	float zoom; // Camera options

	//Camera(pos {0.0f, 0.0f, 0.0f}, up{0.0f, 1.0f, 0.0f}, float yaw = -90.0f, float pitch = 0.0f);

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 view();
	glm::mat4 perspective(float znear = 0.1f, float zfar = 100.0f);
	glm::mat4 perspective(float windowWidth, float windowHeight, float znear, float zfar);
	void onKeyboard(Camera::Movement direction, float deltaTime);
	void onMouseMove(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void onMouseScroll(float yoffset);

	void updateVectors();

private:
	static const float YAW, PITCH, SPEED, SENSITIVITY, ZOOM;

};
namespace config
{
	inline Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));//make camera at xyz pos

	inline int windowWidth = 1920, windowHeight = 1080;
	inline float lastX = windowWidth / 2, lastY = windowWidth / 2; //center of the screen

	//inline int windowWidth = 1280, windowHeight = 720;
	//inline float lastX = windowWidth / 2, lastY = windowWidth / 2; //center of the screen
	//inline void updateConfig(int width, int  heigth)
	//{
	//	windowWidth = width;
	//	windowHeight = heigth;
	//	lastX = windowWidth / 2;
	//	lastY = windowWidth / 2;
	//}
}

}
=======
#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GLFW/glfw3.h> //for GLtypes
#include <glm/glm.hpp>
namespace zap
{

class Camera
{

public:
	enum class Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	glm::vec3 position, front, up, right, worldUp;

	float yaw, pitch; // Euler Angles yaw/pitch/roll (down y/right x/ forward z axes respectively)
	float movementSpeed, mouseSensitivity;
	float zoom; // Camera options

	//Camera(pos {0.0f, 0.0f, 0.0f}, up{0.0f, 1.0f, 0.0f}, float yaw = -90.0f, float pitch = 0.0f);

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 view();
	glm::mat4 perspective(float znear = 0.1f, float zfar = 100.0f);
	glm::mat4 perspective(float windowWidth, float windowHeight, float znear, float zfar);
	void onKeyboard(Camera::Movement direction, float deltaTime);
	void onMouseMove(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void onMouseScroll(float yoffset);

	void updateVectors();

private:
	static const float YAW, PITCH, SPEED, SENSITIVITY, ZOOM;

};
namespace config
{
	inline Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));//make camera at xyz pos

	inline int windowWidth = 1920, windowHeight = 1080;
	inline float lastX = windowWidth / 2, lastY = windowWidth / 2; //center of the screen

	//inline int windowWidth = 1280, windowHeight = 720;
	//inline float lastX = windowWidth / 2, lastY = windowWidth / 2; //center of the screen
	//inline void updateConfig(int width, int  heigth)
	//{
	//	windowWidth = width;
	//	windowHeight = heigth;
	//	lastX = windowWidth / 2;
	//	lastY = windowWidth / 2;
	//}
}

}
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
#endif