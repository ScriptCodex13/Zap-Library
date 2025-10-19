#include "enabler.h"
#ifdef SAMPLE_MAIN_GL_CUBES_LIGHT_CPP

// Just a example
// PR = Prototyping -> only for testing 
#include <Zap.h>
#include <iostream>
#include <vector>
#include <array>
#include "Cube.h"
#include "LightCube.h"

template <typename T> class window_camera_invoker : public zap::util::callback_invoker<T>
{
public:
	inline window_camera_invoker(T _callback) : callback_invoker(_callback) {}
	template<typename A1, typename A2> void operator () (A1& a1, A2& a2) { callback(a1, a2); }
};
window_camera_invoker  cbi([](zap::Window& window, zap::SceneCamera& camera) {
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

int main()
{
	//ZAP_DISABLE_OUTPUTS(true);

	zap::Init(4, 6);

	zap::Window window(1920, 1080, "Hello Window");
	zap::util::scope_guard zapDeleter (zap::Delete);

	zap::InitGlad();


	// Window settings
	window.UpdateViewport(true);
	window.SetVSync(true);
	//window.Maximize();



	std::array<int, 2> size = window.GetSize();

	//Camera
	zap::SceneCamera camera(size[0], size[1]);
	camera.SetRotationLimit(361.0f, 89.0f, 0.0f);
	camera.ActivateRotationLimit(true);


	//window.Maximize();
	window.SetCursorinCameraMode(true);
	//


	glm::vec3 lightPos(1.0f, 0.0f, 2.0f);

	float rotation = 0.0f, sensitivity = 0.1f;

	std::array<double, 2> oldPos = window.GetMousePosition();

	//////Mesh
	Cube cube;
	LightCube lightCube;


	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	zap::Enable(zap::Instructions::DEPTH);
	zap::Enable(zap::Instructions::ANTIALIASING);
	glEnable(GL_DEPTH_TEST);
	while (window.Open())
	{
		glClear(GL_DEPTH_BUFFER_BIT); // PR
		zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);
		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));

		// Mouse Input
		std::array<double, 2> newpos = window.GetMousePosition();
		float xoffset = (newpos[0] - oldPos[0]) * sensitivity;
		float yoffset = (oldPos[1] - newpos[1]) * sensitivity;
		oldPos = newpos; 
		// End mouse input

		camera.Rotate(xoffset, yoffset, 0.0f);

		//Input
		//All the window/camera magic happens here
		cbi(window, camera);
		//

		//Draw cubes here
		//Cube

		camera.UpdateRotation(); //Make camera up to most recent maths

		//Start rendering here


		cube.Bind();
		cube.setColorUniform3f(1.0f, 0.5f, 0.31f);
		cube.setLightColorUniform3f(1.0f, 1.0f, 1.0f);
		cube.MoveUpdate(camera, lightPos, glfwGetTime());
		cube.Draw(36);

		//Light cube

		lightCube.Bind();
		lightCube.MoveUpdate(camera, lightPos, glfwGetTime());
		lightCube.Draw(36);

		//*/

		window.Update();
		window.Draw();

	}

	return 0;

}


#endif