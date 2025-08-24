// Just a example

//this is separate application and should treat the Zap library as external
//everyone using this library will do, this is a standard practice
#include "Zap.h" // Please use intern Header with ""
#include "Mesh.h"
#include <iostream>
#include <vector>


const char* vertexShaderSource = R"glsl(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
} )glsl";

const char* fragmentShaderSource = R"glsl(#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
})glsl";


int main()
{
	system("cd");
	zap::Init();

	zap::Window window(1920, 1080, "Hello Window");

	zap::InitGlad();

	//window.SetFullscreen(true);
	//window.UpdateViewport();

	std::vector<float> vertices =
	{
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right 
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right  This causes the bug
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left		And also this

	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,
		1, 2, 3
	};


	zap::Device controller = zap::AssignController();

	//Mesh

	zap::Mesh2D mesh(&vertices, &indices);

	mesh.SetVertexShaderSource(vertexShaderSource);
	mesh.SetFragmentShaderSource(fragmentShaderSource);

	mesh.SetAttribPointer(0, 3, 8, 0);
	mesh.SetAttribPointer(1, 3, 8, 3);
	mesh.SetAttribPointer(2, 2, 8, 6);

	//Remove the absolute path pointing to file location on your computer
	system("cd");
	unsigned int texture0Id = mesh.InitTexture(0, "textures\\texture.png", 0, zap::TextureFilters::LINEAR, zap::MipmapSettings::LINEAR_MIPMAP_LINEAR, zap::TextureWrapping::CLAMP_TO_EDGE).i_id;

	mesh.Finish();


	window.UpdateViewport(); //This is a set callback. Once set == forever set

	while (window.Open())
	{
		//window.StartTime();

		if (controller.IsConnected())
		{
			if (controller.GetButton(zap::ControllerButtonsPS::CROSS, zap::State::EZ_PRESSED))
			{
				std::cout << "X Pressed" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::RECTANGLE, zap::State::EZ_PRESSED))
			{
				std::cout << "Rectangle pressed" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::TRIANGLE, zap::State::EZ_PRESSED))
			{
				std::cout << "Triangle pressed" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::CIRCLE, zap::State::EZ_PRESSED))
			{
				std::cout << "Circle pressed" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::DPAD_DOWN, zap::State::EZ_PRESSED))
			{
				std::cout << "DPad down" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::DPAD_UP, zap::State::EZ_PRESSED))
			{
				std::cout << "DPad up" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::DPAD_LEFT, zap::State::EZ_PRESSED))
			{
				std::cout << "DPad left" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::DPAD_RIGHT, zap::State::EZ_PRESSED))
			{
				std::cout << "DPad right" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::GUIDE_BUTTON, zap::State::EZ_PRESSED))
			{
				std::cout << "Guide" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::L1, zap::State::EZ_PRESSED))
			{
				std::cout << "L1" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::R1, zap::State::EZ_PRESSED))
			{
				std::cout << "R1" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::BACK, zap::State::EZ_PRESSED))
			{
				std::cout << "Back" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::MENU, zap::State::EZ_PRESSED))
			{
				std::cout << "menu" << std::endl;
			}
			if (controller.GetButton(zap::ControllerButtonsPS::LEFT_STICK_PRESS, zap::State::EZ_PRESSED))
			{
				std::cout << "Left Stick" << std::endl;
			}

			/*int count;
			const float* axis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

			//std::cout << count << std::endl;

			std::cout << axis[5] << std::endl;*/

			std::cout << controller.GetTrigger(zap::ControllerTriggersPS::L2) << std::endl;

		}


		/*int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

		if (present)
		{
			int count;
			const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

			if (buttons[19] == GLFW_PRESS)
			{
				std::cout << "true" << std::endl;
			}
		}*/


		if (window.isKeyPressed(zap::Key::ESC))
		{
			window.Close();
		}

		//from here draw starts
		//there starts general draw
		window.ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		window.ClearBackground(zap::BackgroundColors::BLACK);
		//here starts current VAO for current program draw
		mesh.bind(); //set current context before any draw routines, it prevents mess in more complex programs
		mesh.SetTexture(texture0Id); //return false if texture not found
		mesh.Write();
		//here draw ends
		
		window.Update();
		window.Draw();

		//std::cout << window.GetFPS() << std::endl;
	}

	zap::Delete();

}



