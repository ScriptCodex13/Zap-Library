// Just a example

#include "Zap.h"
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


	//Mesh

	zap::Mesh2D mesh(&vertices, &indices);

	mesh.UseEBO(true);

	mesh.SetVertexShaderSource(vertexShaderSource);
	mesh.SetFragmentShaderSource(fragmentShaderSource);

	mesh.SetAttribPointer(0, 3, 8, 0);
	mesh.SetAttribPointer(1, 3, 8, 3);
	mesh.SetAttribPointer(2, 2, 8, 6);

	mesh.UseTexture(true);
	mesh.InitTexture(0, "C:/Dev/Zap/Rec/Textures/texture.jpg" /*Use your own path here*/, 0, zap::TextureFilters::NEAREST, zap::MipmapSettings::NEAREST_MIPMAP_NEAREST, zap::TextureWrapping::CLAMP_TO_EDGE);
	//mesh.UsePNG(0);

	mesh.Finish();

	//



	//window.Maximize();

	//window.SetFPSLimit(60);

	while (window.Open())
	{
		window.StartTime();

		window.ClearBackground(zap::BackgroundColors::BLACK);

		if (window.GetInput(zap::Key::ESC, zap::State::EZ_PRESSED))
		{
			window.Close();
		}
		if (window.GetInput(zap::Key::F10, zap::State::EZ_PRESSED))
		{
			window.ShowWireFrame(true);
		}
		if (window.GetInput(zap::Key::F10, zap::State::EZ_RELEASED))
		{
			window.ShowWireFrame(false);
		}

		mesh.SetTexture(0);
		mesh.Write(6);

		window.SetTitle(std::to_string(window.GetDelta()));

		window.UpdateViewport();
		window.Update();
		window.Draw();

		//std::cout << window.GetFPS() << std::endl;
	}

	zap::Delete();

}


