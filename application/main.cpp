// Just a example


// PR = Prototyping -> only for testing 

//this is separate application and should treat the Zap library as external
//everyone using this library will do, this is a standard practice
#include <Zap.h> //TODO: This is not the correct way to use libraries
#include <iostream>
#include <vector>

const char* vertexShaderSource = R"glsl(#version 330 core
layout(location = 0) in vec3 aPos;
// layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;      //takes local coordinates for thing and moves it into world coordinates
uniform mat4 view;       //moves world space objects around based on camera
uniform mat4 projection; //converts values to normalised device coordinates (use sweet math for perspective)

// out vec3 ourColor;

out vec2 TexCoord;
void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// ourColor = aColor;
})glsl";

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

	std::vector<float> vertices =
	{
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 2
		//1, 2, 3
	};


	//Controller 

	// zap::Device controller = zap::AssignController(); For later prototyping with the controller

	//


	//Camera

	std::array<unsigned int, 2> size = window.GetSize(); // Not a Ref to the window size !

	zap::SceneCamera camera(size[0], size[1]);

	//


	//Mesh

	zap::Mesh2D mesh(&vertices, &indices);

	mesh.SetVertexShaderSource(vertexShaderSource);
	mesh.SetFragmentShaderSource(fragmentShaderSource);

	

	mesh.SetAttribPointer(0, 3, 8, 0);
	//mesh.SetAttribPointer(1, 3, 8, 3);
	mesh.SetAttribPointer(2, 2, 8, 6);

	auto texture = mesh.AddTexture(0, "textures/texture.png", zap::TextureFilters::LINEAR, zap::MipmapSettings::LINEAR_MIPMAP_LINEAR, zap::TextureWrapping::CLAMP_TO_EDGE);

	mesh.Finish();
	
	//

	window.SetVSync(true);

	// Window settings

	window.UpdateViewport(true);

	//
	mesh.Bind();
	
	while (window.Open())
	{
		if (window.isKeyPressed(zap::Key::ESC))
		{
			window.Close();
		}

		if (window.GetInput(zap::Key::left_arrow, zap::State::PRESSED))
		{
			camera.Rotate(5.0f * window.GetDelta() * 20.0f, 0.0f, 0.0f);
		}
		if (window.GetInput(zap::Key::right_arrow, zap::State::PRESSED))
		{
			camera.Rotate(-5.0f * window.GetDelta() * 20.0f, 0.0f, 0.0f);
		}

		mesh.GetModel() = glm::translate(glm::mat4(1.0), glm::vec3(0.1f, -0.1f, 0.0f));

		mesh.UpdateModel("model");
		camera.UpdateProjection(mesh.GetProgram(), "projection");
		camera.UpdateView(mesh.GetProgram(), "view");
		camera.UpdateRotation();

		//here starts current VAO for current program draw
		mesh.Bind(); //set current context before any draw routines, it prevents mess in more complex workflow

		glEnable(GL_DEPTH_TEST); // Move it here. Any rendering function must be invoked here.
		glClear(GL_DEPTH_BUFFER_BIT); // PR

		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);


		//glUniformMatrix4fv(viewLocationId, 1, GL_FALSE, glm::value_ptr(view));
		mesh.UseTexture(texture.i_id); //return false if texture not found
		mesh.Draw();
		//here draw ends
		
		window.Update();
		window.Draw();

		//std::cout << window.GetFPS() << std::endl;
	}

	zap::Delete();

}





