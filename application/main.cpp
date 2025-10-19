#include "enabler.h"
#ifdef SAMPLE_MAIN_CPP

// Just a example
// PR = Prototyping -> only for testing 
#include <Zap.h>
#include <iostream>
#include <vector>
#include <array>
#include "Cube.h"
#include "LightCube.h"
#include <Window/Window.h>

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

	//Controller

	// zap::Device controller = zap::AssignController(); For later prototyping with the controller

	//


	//Camera

	std::array<int, 2> size = window.GetSize(); // Not a Ref to the window size !

	zap::SceneCamera camera(size[0], size[1]);

	camera.SetRotationLimit(361.0f, 89.0f, 0.0f);
	camera.ActivateRotationLimit(true);

	//



	// Window settings

	window.UpdateViewport(true);
	window.SetVSync(true);
	//window.Maximize();

	zap::Enable(zap::Instructions::DEPTH);
	zap::Enable(zap::Instructions::ANTIALIASING);

	//window.SetCursorinCameraMode(false);

	zap::Text text("C:/Windows/Fonts/arial.ttf", "Text", window.GetSize()); // It's better to use GetSize here

	//text.SetCharacterSize(48);
	text.SetColor(0.0f, 1.0f, 0.0f);
	text.SetPosition(500.0f, 500.0f); // ToDo: Maybe use gl_coords here

	//text.SetTextureFilter(zap::TextureFilter::LINEAR);
	//text.SetMipmapSettings(zap::MipmapSetting::LINEAR_MIPMAP_LINEAR);
	//text.GenerateCharacters();

	text.SetScale(2.0f, 2.0f);


	glm::vec3 lightPos(1.0f, 0.0f, 2.0f);

	float rotation = 0.0f, sensitivity = 0.1f;

	std::array<double, 2> oldPos = window.GetMousePosition();

	auto coord = zap::util::pixel_to_gl_coords(window.GetSize(), 1.0f, 1.0f);
	//std::cout << "x: " << coord[0] << ",y: " << coord[1] << std::endl;
	auto coord_2 = zap::util::gl_coords_to_pixel(window.GetSize(), 0.9f, 0.0f);
	//std::cout << "x: " << coord_2[0] << ",y: " << coord[1] << std::endl;

	//////Mesh
	Cube cube;
	LightCube lightCube;

	zap::Button button(std::array<float, 4> {-0.5, 0.6, 0.2, 0.8 }, window.GetSize(), window.GetOriginalSize(), "Button", "C:/Windows/Fonts/arial.ttf");
	//button.SetGlPosition(std::array<float, 2> {- 0.9f, 0.7f});
	//button.SetGlPosition(- 0.9f, 0.7f, 0.2, 0.8 );
	//button.SetGlSize(std::array<float, 2> {0.1f, 0.1f});
	//button.SetGlWidth(0.7);
	//button.SetGlHeight(0.28);
	button.FinishMesh();

	button.SetTextOffset(0.31f, 0.075f);
	//button.UseText(false);
	button.SetTextColor(zap::TextColors::PURPLE);


	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (window.Open())
	{
		// Mouse Input -> Commented out temporary
		/*
		std::array<double, 2> newpos = window.GetMousePosition();

		float xoffset = (newpos[0] - oldPos[0]) * sensitivity;
		float yoffset = (oldPos[1] - newpos[1]) * sensitivity;

		oldPos = newpos; 

		camera.Rotate(xoffset, yoffset, 0.0f); //*/ 

		//

		//Input
		//All the window/camera magic happens here
		cbi(window, camera);

		//


		glClear(GL_DEPTH_BUFFER_BIT); // PR
		zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);

		//Cube

		camera.UpdateRotation();
	
		cube.Bind();
		cube.setColorUniform3f(1.0f, 0.5f, 0.31f);
		cube.setLightColorUniform3f(1.0f, 1.0f, 1.0f);
		cube.MoveUpdate(camera, lightPos, glfwGetTime());
		cube.Draw(36);

		//

		//Light cube

		lightCube.Bind();
		lightCube.MoveUpdate(camera, lightPos, glfwGetTime());
		lightCube.Draw(36);

		//*/

		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		//zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);
		//here starts current VAO for current program draw
		//here draw ends

		glDisable(GL_DEPTH_TEST);
		//button.Update(); //TODO: Do this only when needed, not on each while loop rendering iteration
		button.Draw();

		text.SetContent(std::to_string(std::round(window.GetFPS())));
		text.Draw();

		//if (button.Pressed(zap::Key::LEFT_MOUSE))
		//{	
		//	std::cerr << "Pressed" << std::endl;
		//
		//	//window.Close();
		//}
		glEnable(GL_DEPTH_TEST);

		window.Update();
		window.Draw();

		zap::ClearBuffers();

		/*float width, height;

		glfwGetWindowContentScale(window.GetNativeWindow(), &width, &height);

		std::cout << "width: " << width << "height: " << height << std::endl;*/

		rotation += 1.0f;
	}

	return 0;

}


#endif