#pragma once

#ifndef SCENE_CAMERA_H
#define SCENE_CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <array>
#include <algorithm> // For std::clamp in SetFOV


namespace zap
{
	class Camera // Dont know if it works
	{
	public:
		Camera(unsigned int& window_width, unsigned int& window_height, const std::array<float, 3> position = { 0.0f, 0.0f, 3.0f }, std::array<float, 3> up = { 0.0f, 1.0f, 0.0f });
		~Camera();

		void SetPosition(float x, float y, float z);
		void Rotate(float yaw, float pitch, float roll);
		void SetFOV(float new_fov);

		glm::mat4 GetProjection();
		glm::mat4 GetView();

		void UpdateProjection(unsigned int shader_program, const std::string projection_uniform_name);
		void UpdateView(unsigned int shader_program, const std::string view_uniform_name);
		void UpdateRotation();

	private:
		float i_yaw = -90.0f;
		float i_pitch = 0.0f;
		float i_roll = 0.0f;
		float i_fov = 60;
		
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