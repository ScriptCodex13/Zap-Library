#pragma once
// Just a sample
#include <Zap.h>
#include <Graphics.h>


class LightCube : public zap::Mesh
{
	const char* vertexShaderSource = R"glsl(#version 330 core
		layout (location = 0) in vec3 aPos;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
		})glsl";

	const char* fragmentShaderSource = R"glsl(#version 330 core
		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(1.0); // set all 4 vector values to 1.0
		})glsl";


public:
	LightCube();
	void MoveUpdate(const zap::SceneCamera& camera, glm::vec3 lightPos, double time);
};
