#include "SceneCamera.h"

#include <iostream>

namespace zap
{

	inline float FastClamp(float var, float min, float max)
	{
		if (var < min)
		{
			return var = min;
		}

		if (var > max)
		{
			return var = max;
		}

		return var;
	}

	SceneCamera::SceneCamera(unsigned int& window_width, unsigned int& window_height, const std::array<float, 3> position, std::array<float, 3> world_up)
		: i_screen_width(window_width), i_screen_height(window_height)
	{
		i_camera_position = glm::vec3(position[0], position[1], position[2]);
		i_world_up = glm::vec3(world_up[0], world_up[1], world_up[2]);

		projection = glm::perspective(glm::radians(i_fov), (float)i_screen_width / (float)i_screen_height, 0.1f, 100.0f);
		view = glm::lookAt(i_camera_position, i_camera_position + i_camera_front, i_camera_up);
	}

	SceneCamera::~SceneCamera()
	{

	}

	void SceneCamera::SetPosition(float x, float y, float z)
	{
		i_camera_position = glm::vec3(x, y, z);
	}

	//void Camera::Rotate(float yaw, float pitch, float roll /* <- Doesnt work*/)
	//{
	//
	//	std::cout << yaw << std::endl;
	//
	//	i_yaw += yaw;
	//
	//	if (i_yaw > 360.0f)
	//	{
	//		i_yaw = 0.0f;
	//	}
	//	if (i_yaw < -360.0f)
	//	{
	//		i_yaw = 0.0f;
	//	}
	//
	//	i_pitch += pitch;
	//
	//	if (i_pitch > 360.0f)
	//	{
	//		i_pitch = 0.0f;
	//	}
	//	if (i_pitch < -360.0f)
	//	{
	//		i_pitch = 0.0f;
	//	}
	//
	//	std::cout << i_yaw << std::endl;
	//}

	void SceneCamera::Rotate(float yaw, float pitch, float roll /* <- Doesnt work*/)
	{
		i_yaw += yaw;

		/*if (i_yaw > 360.0f)
		{
			i_yaw = 0.0f;
		}
		if (i_yaw < -360.0f)
		{
			i_yaw = 0.0f;
		}*/
		
			i_pitch += pitch;
		
		/*if (i_pitch > 360.0f)
		{
			i_pitch = 0.0f;
		}
		if (i_pitch < -360.0f)
		{
			i_pitch = 0.0f;
		}*/
	}

	void SceneCamera::SetFOV(float new_fov)
	{
		i_fov = std::clamp(new_fov, 10.0f, 120.0f);
	}

	void SceneCamera::SetRotationLimit(float yaw_limit, float pitch_limit, float roll_limit) // You only need to give in positive numbers
	{
		i_yaw_clamp = yaw_limit;
		i_pitch_clamp = pitch_limit;
		i_roll_clamp = roll_limit;
	}

	void SceneCamera::ActivateRotationLimit(bool state)
	{
		i_limit_rotation = state;
	}

	void SceneCamera::MoveForward(float speed_factor)
	{
		i_camera_position += glm::vec3(i_camera_front * speed_factor);
	}

	void SceneCamera::MoveBackward(float speed_factor)
	{
		i_camera_position -= glm::vec3(i_camera_front * speed_factor);
	}

	void SceneCamera::MoveLeft(float speed_factor)
	{
		i_camera_position -= glm::normalize(glm::cross(i_camera_front, i_camera_up)) * speed_factor;
	}

	void SceneCamera::MoveRight(float speed_factor)
	{
		i_camera_position += glm::normalize(glm::cross(i_camera_front, i_camera_up)) * speed_factor;
	}

	
	glm::mat4& SceneCamera::GetProjection()
	{
		return projection;
	}

	glm::mat4& SceneCamera::GetView()
	{
		return view;
	}

	glm::vec3& SceneCamera::GetPosition()
	{
		return i_camera_position;
	}

	void SceneCamera::UpdateRotation()
	{
		if (i_limit_rotation)
		{
			i_yaw = FastClamp(i_yaw, -i_yaw_clamp, i_yaw_clamp);
			i_pitch = FastClamp(i_pitch, -i_pitch_clamp, i_pitch_clamp);
			i_roll = FastClamp(i_roll, -i_roll_clamp, i_roll_clamp);
		}

		if (i_yaw > 359.9f) // Prevents clipping or teleporting of objects in the scene
		{
			i_yaw = 0.0f;
		}
		if (i_yaw < -359.9f)
		{
			i_yaw = 0.0f;
		}

		if (i_pitch > 359.9f)
		{
			i_pitch = 0.0f;
		}
		if (i_pitch < -359.9f)
		{
			i_pitch = 0.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(i_yaw)) * cos(glm::radians(i_pitch));
		front.y = sin(glm::radians(i_pitch));
		front.z = sin(glm::radians(i_yaw)) * cos(glm::radians(i_pitch));

		i_camera_front = glm::normalize(front);

		i_camera_right = glm::normalize(glm::cross(i_camera_front, i_world_up));
		i_camera_up = glm::normalize(glm::cross(i_camera_right, i_camera_front));
		projection = glm::perspective(glm::radians(i_fov), (float)i_screen_width / (float)i_screen_height, 0.1f, 100.0f);
		view = glm::lookAt(i_camera_position, i_camera_position + i_camera_front, i_camera_up);

	}
}
