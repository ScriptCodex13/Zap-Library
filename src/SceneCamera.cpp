#include "SceneCamera.h"

#include <iostream>

namespace zap
{
<<<<<<< HEAD
	Camera::Camera(unsigned int& window_width, unsigned int& window_height, const std::array<float, 3> position, std::array<float, 3> world_up)
=======
	SceneCamera::SceneCamera(unsigned int& window_width, unsigned int& window_height, const std::array<float, 3> position, std::array<float, 3> world_up)
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
		: i_screen_width(window_width), i_screen_height(window_height)
	{
		i_camera_position = glm::vec3(position[0], position[1], position[2]);
		i_world_up = glm::vec3(world_up[0], world_up[1], world_up[2]);

		projection = glm::perspective(glm::radians(i_fov), (float)i_screen_width / (float)i_screen_height, 0.1f, 100.0f);
		view = glm::lookAt(i_camera_position, i_camera_position + i_camera_front, i_camera_up);
	}

<<<<<<< HEAD
	Camera::~Camera()
=======
	SceneCamera::~SceneCamera()
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
	{

	}

<<<<<<< HEAD
	void Camera::SetPosition(float x, float y, float z)
=======
	void SceneCamera::SetPosition(float x, float y, float z)
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
	{
		i_camera_position = glm::vec3(x, y, z);
	}

<<<<<<< HEAD
	void Camera::Rotate(float yaw, float pitch, float roll /* <- Doesnt work*/)
	{

		std::cout << yaw << std::endl;

		i_yaw += yaw;

		if (i_yaw > 360.0f)
		{
			i_yaw = 0.0f;
		}
		if (i_yaw < -360.0f)
		{
			i_yaw = 0.0f;
		}

		i_pitch += pitch;

		if (i_pitch > 360.0f)
		{
			i_pitch = 0.0f;
		}
		if (i_pitch < -360.0f)
		{
			i_pitch = 0.0f;
		}

		std::cout << i_yaw << std::endl;
	}

	void Camera::SetFOV(float new_fov)
	{
		new_fov = std::clamp(new_fov, 10.0f, 120.0f);

		i_fov = new_fov;
	}

	void Camera::Move(float x, float y, float z)
=======
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

	void SceneCamera::RotateDelta(float yaw, float pitch, float roll /* <- Doesnt work*/)
	{
		this->RotateAbsolute(i_yaw + yaw, i_pitch + pitch, i_roll + roll);
	}

	void SceneCamera::RotateAbsolute(float yaw, float pitch, float roll /* <- Doesnt work*/)
	{
		const float yaw_clamp = 360.0f; //30 degrees should be enough
		const float pitch_clamp = 360.0f; //30 degrees should be enough

		i_yaw = std::clamp(yaw, -yaw_clamp, yaw_clamp);
		i_pitch = std::clamp (pitch, -pitch_clamp, pitch_clamp);

	}

	void SceneCamera::SetFOV(float new_fov)
	{
		i_fov = std::clamp(new_fov, 10.0f, 120.0f);
	}

	void SceneCamera::Move(float x, float y, float z)
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
	{
		glm::vec3 velocity = glm::vec3(x, y, z);
		i_camera_position += velocity * i_camera_front;
	}

<<<<<<< HEAD
	glm::mat4& Camera::GetModel()
=======
	glm::mat4& SceneCamera::GetModel()
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
	{
		return model;
	}

<<<<<<< HEAD
	glm::mat4& Camera::GetProjection()
=======
	glm::mat4& SceneCamera::GetProjection()
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
	{
		return projection;
	}

<<<<<<< HEAD
	glm::mat4& Camera::GetView()
=======
	glm::mat4& SceneCamera::GetView()
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
	{
		return view;
	}

<<<<<<< HEAD
	void Camera::UpdateModel(unsigned int shader_program, const std::string model_uniform_name)
=======
	void SceneCamera::UpdateModel(unsigned int shader_program, const std::string model_uniform_name)
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
	{
		glUniformMatrix4fv(glGetUniformLocation(shader_program, model_uniform_name.c_str()), 1, GL_FALSE, glm::value_ptr(model));
	}

<<<<<<< HEAD
	void Camera::UpdateProjection(unsigned int shader_program, const std::string projection_uniform_name) 
	{
		projection = glm::perspective(glm::radians(i_fov), (float)i_screen_width / (float)i_screen_height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, projection_uniform_name.c_str()), 1, GL_FALSE, glm::value_ptr(projection));
	}

	void Camera::UpdateView(unsigned int shader_program, const std::string view_uniform_name)
	{
		view = glm::lookAt(i_camera_position, i_camera_position + i_camera_front, i_camera_up);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, view_uniform_name.c_str()), 1, GL_FALSE, glm::value_ptr(view));
	}

	void Camera::UpdateRotation()
=======
	void SceneCamera::UpdateProjection(unsigned int shader_program, const std::string projection_uniform_name) 
	{
		glUniformMatrix4fv(glGetUniformLocation(shader_program, projection_uniform_name.c_str()), 1, GL_FALSE, glm::value_ptr(projection));
	}

	void SceneCamera::UpdateView(unsigned int shader_program, const std::string view_uniform_name)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader_program, view_uniform_name.c_str()), 1, GL_FALSE, glm::value_ptr(view));
	}

	void SceneCamera::UpdateRotation()
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
	{
		glm::vec3 front;
		front.x = cos(glm::radians(i_yaw)) * cos(glm::radians(i_pitch));
		front.y = sin(glm::radians(i_pitch));
		front.z = sin(glm::radians(i_yaw)) * cos(glm::radians(i_pitch));

		i_camera_front = glm::normalize(front);

		i_camera_right = glm::normalize(glm::cross(i_camera_front, i_world_up));
		i_camera_up = glm::normalize(glm::cross(i_camera_right, i_camera_front));
<<<<<<< HEAD
=======
		projection = glm::perspective(glm::radians(i_fov), (float)i_screen_width / (float)i_screen_height, 0.1f, 100.0f);
		view = glm::lookAt(i_camera_position, i_camera_position + i_camera_front, i_camera_up);
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844

	}
}
