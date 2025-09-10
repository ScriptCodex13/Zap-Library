#pragma once

#ifndef SCENE_CAMERA_H
#define SCENE_CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Util.h"

#include <string>
#include <array>
#include <algorithm> // For std::clamp in SetFOV


namespace zap
{
	class SceneCamera // Dont know if it works
	{
	public:
		SceneCamera(unsigned int& window_width, unsigned int& window_height, const std::array<float, 3> position = { 0.0f, 0.0f, 3.0f }, std::array<float, 3> up = { 0.0f, 1.0f, 0.0f });
		~SceneCamera();

		void SetPosition(float x, float y, float z);
		void Rotate(float yaw, float pitch, float roll);
		void SetFOV(float new_fov);
		void SetRotationLimit(float yaw_limit, float pitch_limit, float roll_limit);
		void ActivateRotationLimit(bool state);

		void MoveForward(float speed_factor); // You need delta here
		void MoveBackward(float speed_factor);
		void MoveLeft(float speed_factor);
		void MoveRight(float speed_factor);

		glm::mat4& GetProjection();
		glm::mat4& GetView();

		glm::vec3& GetPosition();

		void UpdateRotation();

	private:
		float i_yaw = -90.0f;
		float i_pitch = 0.0f;
		float i_roll = 0.0f;
		float i_fov = 60;
		
		float i_yaw_clamp = 360.0f;  // keeps track on when to clamp
		float i_pitch_clamp = 360.0f;
		float i_roll_clamp = 360.0f;
		bool i_limit_rotation = false;

		glm::vec3 i_camera_position;
		glm::vec3 i_camera_front;
		glm::vec3 i_camera_up;
		glm::vec3 i_camera_right;

		glm::vec3 i_world_up;

		glm::mat4 projection;
		glm::mat4 view;

		unsigned int i_screen_width;
		unsigned int i_screen_height;

	};
}

#endif