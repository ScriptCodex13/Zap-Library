#include "enabler.h"
#ifdef SAMPLE_MAIN_SIMPLE_TRIANGLES_CLASS_CAMERA_CPP

// Just a example
// PR = Prototyping -> only for testing 
#include <Zap.h>
#include <iostream>
#include <vector>
#include <array>


#include <glm/glm.hpp>                  //basic glm math functions
#include <glm/gtc/matrix_transform.hpp> //matrix functions
#include <glm/gtc/type_ptr.hpp>         //convert glm types to opengl types


template <typename T> class window_camera_invoker : public zap::util::callback_invoker<T>
{
public:
	inline window_camera_invoker(T _callback) : callback_invoker(_callback) {}
	template<typename A1, typename A2> void operator () (A1& a1, A2& a2) { callback(a1, a2); }
};
window_camera_invoker  cbi([](zap::Window& window, zap::SceneCamera& camera)
{
	if (window.isKeyPressed(zap::Key::ESC))
	{
		window.Close();
	}

	if (window.GetInput(zap::Key::left_arrow, zap::State::PRESSED))
	{
		camera.Rotate(0.0f, 0.0f, -5.0f * window.GetDelta() * 20.0f);
	}
	if (window.GetInput(zap::Key::right_arrow, zap::State::PRESSED))
	{
		camera.Rotate(0.0f, 0.0f, 5.0f * window.GetDelta() * 20.0f);
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
	});

class TriangleColor : public zap::Mesh
{

	const char* vertexCameraShaderSource = R"glsl(#version 330 core
		layout(location = 0) in vec3 aPos;
		layout(location = 1) in vec3 aColor;

		uniform mat4 model;      //takes local coordinates for thing and moves it into world coordinates
		uniform mat4 view;       //moves world space objects around based on camera
		uniform mat4 projection; //converts values to normalised device coordinates (use sweet math for perspective)

		out vec3 ourColor;

		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
			ourColor = aColor;
		})glsl";

	const char* fragmentCameraShaderSource = R"glsl(#version 330
		out vec4 FragColor;
  
		in vec3 ourColor;

		void main()
		{
			FragColor = vec4(ourColor, 1);
		})glsl";


	std::vector<float> vertices =
	{
		// positions          // colors
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // top right 
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom right  This causes the bug
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   // top left		And also this

	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,
		1, 2, 3
	};

	//unsigned int texture0Id;
public:
	TriangleColor() //:zap::Mesh(vertices, indices)
	{
		PreSetIndices(indices);
		PreSetVertices(vertices);
		SetVertexShaderSource(vertexCameraShaderSource);
		SetFragmentShaderSource(fragmentCameraShaderSource);

		SetAttribPointer(0, 3, 6, 0);
		SetAttribPointer(1, 3, 6, 3);
		Finish();

	}
};

int main()
{
	zap::Init();

	zap::Window window(1920, 1080, "Hello Window");
	zap::util::scope_guard zapDeleter(zap::Delete);
	zap::InitGlad();

	//Mesh
	TriangleColor mesh;

	mesh.Finish();

	window.UpdateViewport(); //This is a set callback. Once set == forever set
	std::array<int, 2> size = window.GetSize(); // Not a Ref to the window size !

	zap::SceneCamera camera(size[0], size[1]);
	camera.SetRotationLimit(361.0f, 89.0f, 0.0f);
	camera.ActivateRotationLimit(true);


	float rotation = 0.0f, sensitivity = 0.1f;
	std::array<double, 2> oldPos = window.GetMousePosition();
	float lastFrame = 0.f, deltaTime = 0.f;

	while (window.Open())
	{
		std::array<double, 2> newpos = window.GetMousePosition();

		float xoffset = (newpos[0] - oldPos[0]) * sensitivity;
		float yoffset = (oldPos[1] - newpos[1]) * sensitivity;

		oldPos = newpos;

		camera.Rotate(xoffset, yoffset, 0.0f);

		deltaTime = (float)glfwGetTime() - lastFrame;

		lastFrame += deltaTime;
		cbi(window, camera);
		camera.UpdateRotation();

		//from here draw starts
		//there starts general draw
		glClear(GL_DEPTH_BUFFER_BIT); // PR
		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		zap::ClearBackground(zap::BackgroundColors::BLACK);


		//here starts current VAO for current program draw
		mesh.Bind(); //set current context before any draw routines, it prevents mess in more complex programs
		//update uniforms

		mesh.UpdateModel(glm::rotate(glm::mat4(1.0), (float)0, glm::vec3(0.0f, 1.0f, 0.0f)));
		mesh.SetView(camera.GetView());
		mesh.SetProjection(camera.GetProjection());

		mesh.Draw();
		//here draw ends

		window.SetTitle(std::to_string(window.GetDelta()));

		window.Update();
		window.Draw();

	}



}

#endif