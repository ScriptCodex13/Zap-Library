#include "enabler.h"

#ifdef MAIN_ASSIMP_MODEL_LOADING_CPP

#include <Zap.h>
#include <Graphics.h>

std::string vertex_shader_source =
R"glsl(
			#version 330 core

			layout(location = 0) in vec3 aPos;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			void main()
			{
				gl_Position = projection * model * view * vec4(aPos, 1.0);
			}
	)glsl";

int main()
{
	zap::Init(4, 6);

	zap::Window window(1920, 1080, "Hello Model");

	zap::InitGlad();

	zap::ModelData modeldata = zap::LoadModel("models/TestCube.obj");

	window.Maximize();
	window.UpdateViewport(true);
	window.SetCursorinCameraMode(true);

	zap::Mesh mesh(modeldata.meshes[0].vertices, modeldata.meshes[0].indices);

	mesh.SetAttribPointer(0, 3, 3, 0);

	mesh.SetVertexShaderSource(vertex_shader_source);

	mesh.Finish();

	unsigned int model_uniform_location = glGetUniformLocation(mesh.GetProgram(), "model");
	unsigned int view_uniform_location = glGetUniformLocation(mesh.GetProgram(), "view");
	unsigned int projection_uniform_location = glGetUniformLocation(mesh.GetProgram(), "projection");

	zap::SceneCamera camera(window.GetSize()[0], window.GetSize()[1]);

	camera.SetRotationLimit(361.0f, 89.0f, 361.0f);
	camera.ActivateRotationLimit(true);

	glm::mat4 model = glm::mat4(1.0f);

	std::array<double, 2> oldPos = window.GetMousePosition();

	float sensitivity = 0.1f;

	glEnable(GL_CULL_FACE); // Test if it works

	while (window.Open())
	{
		std::array<double, 2> newpos = window.GetMousePosition();

		float xoffset = (newpos[0] - oldPos[0]) * sensitivity;
		float yoffset = (oldPos[1] - newpos[1]) * sensitivity;

		oldPos = newpos;

		camera.Rotate(xoffset, yoffset, 0.0f);

		if (window.GetInput(zap::Key::left_arrow, zap::State::PRESSED))
		{
			camera.Rotate(-5.0f * window.GetDelta() * 20.0f, 0.0f, 0.0f);
		}
		if (window.GetInput(zap::Key::right_arrow, zap::State::PRESSED))
		{
			camera.Rotate(5.0f * window.GetDelta() * 20.0f, 0.0f, 0.0f);
		}
		if (window.GetInput(zap::Key::W, zap::State::PRESSED))
		{
			camera.MoveForward(0.5f * window.GetDelta() * 20.0f);
		}
		if (window.GetInput(zap::Key::S, zap::State::PRESSED))
		{
			camera.MoveBackward(0.5f * window.GetDelta() * 20.0f);
		}
		if (window.GetInput(zap::Key::A, zap::State::PRESSED))
		{
			camera.MoveLeft(0.5f * window.GetDelta() * 20.0f);
		}
		if (window.GetInput(zap::Key::D, zap::State::PRESSED))
		{
			camera.MoveRight(0.5f * window.GetDelta() * 20.0f);
		}

		if(window.isKeyPressed(zap::Key::ESC))
		{
			window.Close();
		}

		camera.UpdateRotation();

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));

		mesh.UseProgram();

		glUniformMatrix4fv(model_uniform_location, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(view_uniform_location, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
		glUniformMatrix4fv(projection_uniform_location, 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));

		mesh.Bind();
		mesh.Draw();

		window.Update();
		window.Draw();
	}

	zap::Delete();
}

#endif