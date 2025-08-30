// Just a example


// PR = Prototyping -> only for testing 

//this is separate application and should treat the Zap library as external
//everyone using this library will do, this is a standard practice
#include "Zap.h" //TODO: This is not the correct way to use libraries
#include "Mesh.h"
#include <iostream>
#include <vector>

const char* vertexShaderSource = R"glsl(#version 330 core
layout(location = 0) in vec3 aPos;
// layout(location = 1) in vec3 aColor;
// layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;      //takes local coordinates for thing and moves it into world coordinates
uniform mat4 view;       //moves world space objects around based on camera
uniform mat4 projection; //converts values to normalised device coordinates (use sweet math for perspective)

out vec3 ourColor;
// out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// ourColor = aColor;
	// TexCoord = vec2(aTexCoord.x, aTexCoord.y);

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

// Du musst auch das mesh transformieren ! Du musst uniform model auc verändern !

int main()
{
	zap::Init();

	zap::Window window(1920, 1080, "Hello Window");

	zap::InitGlad();

	glEnable(GL_DEPTH_TEST); // PR

	//window.SetFullscreen(true);
	//window.UpdateViewport();

	std::vector<float> vertices =
	{
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right 
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left

	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,
		1, 2, 3
	};

	//Controller 

	zap::Device controller = zap::AssignController();

	//


	//Camera

	std::array<unsigned int, 2> size = window.GetSize(); // Funktioniert noch nicht ganz gibt keine refernz zurück !

	zap::Camera camera(size[0], size[1]);

	//


	//Mesh

	zap::Mesh2D mesh(&vertices, &indices);

	mesh.SetVertexShaderSource(vertexShaderSource);
	mesh.SetFragmentShaderSource(fragmentShaderSource);

	mesh.SetAttribPointer(0, 3, 8, 0);
	//mesh.SetAttribPointer(1, 3, 8, 3);
	//mesh.SetAttribPointer(2, 2, 8, 6);

	//Remove the absolute path pointing to file location on your computer
	//TODO: Texture must be added into the project. The project must be self sufficient.
	//auto texture = mesh.AddTexture(0, "C:/Dev/Test/New_v13/Zap-Library/Zap3DDemo/Zap3DTrianglesBasic/textures/texture.png", zap::TextureFilters::LINEAR, zap::MipmapSettings::LINEAR_MIPMAP_LINEAR, zap::TextureWrapping::CLAMP_TO_EDGE);

	mesh.Finish();

	//

	
	// Window settings

	window.UpdateViewport(true);

	window.SetFPSLimit(60);

	//


	while (window.Open())
	{
		window.ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);

		glClear(GL_DEPTH_BUFFER_BIT); // PR

		camera.Rotate(0.0f, 0.0f, 0.0f); // Doesn't work
		
		camera.UpdateProjection(mesh.GetProgram(), "projection"); // Maybe also 
		camera.UpdateView(mesh.GetProgram(), "view");
		camera.UpdateRotation();

		if (window.isKeyPressed(zap::Key::ESC))
		{
			window.Close();
		}

		//from here draw starts
		//there starts general draw
		window.ShowWireFrame(window.isKeyPressed(zap::Key::F10));


		//here starts current VAO for current program draw
		mesh.bind(); //set current context before any draw routines, it prevents mess in more complex programs
		//mesh.UseTexture(texture.i_id); //return false if texture not found
		mesh.Write();
		//here draw ends
		
		window.Update();
		window.Draw();

		//std::cout << window.GetFPS() << std::endl;
	}

	zap::Delete();

}



