// Just a example


// PR = Prototyping -> only for testing 

//this is separate application and should treat the Zap library as external
//everyone using this library will do, this is a standard practice
#include <Zap.h> //TODO: This is not the correct way to use libraries
#include "SceneCamera.h"
#include <iostream>
#include <vector>

const char* vertexShaderSource = R"glsl(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;      //takes local coordinates for thing and moves it into world coordinates
uniform mat4 view;       //moves world space objects around based on camera
uniform mat4 projection; //converts values to normalised device coordinates (use sweet math for perspective)

out vec3 ourColor;

out vec2 TexCoord;
void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor;
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
	mesh.SetAttribPointer(1, 3, 8, 3);
	mesh.SetAttribPointer(2, 2, 8, 6);

	auto texture = mesh.AddTexture(0, "textures/texture.png", zap::TextureFilters::LINEAR, zap::MipmapSettings::LINEAR_MIPMAP_LINEAR, zap::TextureWrapping::CLAMP_TO_EDGE);

	mesh.Finish();
	
	//



	// Window settings

	window.UpdateViewport(true);

	window.SetFPSLimit(120);

	//
	mesh.bind();
	unsigned int modelLocationId      = mesh.GetUniformLocation("model");
	unsigned int viewLocationId       = mesh.GetUniformLocation("view");
	unsigned int projectionLocationId = mesh.GetUniformLocation("projection");
	const double currentTime = glfwGetTime();
	glm::mat4 model      = glm::translate(glm::mat4(1.0), glm::vec3(0.1f, -0.1f, 0.0f));
	glm::mat4 view       = camera.GetView();
	glm::mat4 projection = camera.GetProjection();
	//Initialize uniforms. Later change any of them only when really needed:
	glUniformMatrix4fv(modelLocationId,      1, GL_FALSE, glm::value_ptr(model));        //As of now doesn't change during execution
	glUniformMatrix4fv(projectionLocationId, 1, GL_FALSE, glm::value_ptr(projection));   //As of now doesn't change during execution
	glUniformMatrix4fv(viewLocationId,       1, GL_FALSE, glm::value_ptr(view));         //Changes every frame

	std::cout << "Menu (key press): " << std::endl;
	std::cout << "    <1>   Enable FPS Limit" << std::endl;
	std::cout << "    <2>   Disable FPS Limit" << std::endl;
	std::cout << "    <Q>   Move camera LEFT/RIGHT with Array keys LEFT/RIGHT" << std::endl;
	std::cout << "    <W>   Move camera UP/DOWN with Time Bouncing" << std::endl;
	std::cout << "    <A>   Keep button pressed for 120 FOW and released for 60 FOW" << std::endl;
	std::cout << "    <F10> Keep button pressed for Wireframe mode and released for Surface mode" << std::endl;
	std::cout << "    <ESC> Quit Application" << std::endl;
	double currentTime2 = glfwGetTime();
	int pathway = 1;
	const int keyboardArray = 1, bounceTimed = 2;
	int useDrawing = keyboardArray;
	while (window.Open())
	{

		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_1) == GLFW_PRESS) pathway = 1;
		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_2) == GLFW_PRESS) pathway = 2;
		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_Q) == GLFW_PRESS) useDrawing = keyboardArray;
		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_W) == GLFW_PRESS) useDrawing = bounceTimed;
		if (window.isKeyPressed(zap::Key::ESC))
		{
			window.Close();
		}

		if (window.GetInput(zap::Key::A, zap::State::EZ_PRESSED))
		{
			camera.SetFOV(120);
		}
		if (window.GetInput(zap::Key::A, zap::State::EZ_RELEASED))
		{
			camera.SetFOV(60);
		}

		//TODO: Keep the workflow. Here starts general draw
		glEnable(GL_DEPTH_TEST); // Move it here. Any rendering function must be invoked here.
		glClear(GL_DEPTH_BUFFER_BIT); // PR
		window.ClearDepthBuffer();
		window.ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		window.ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);


		//here starts transformation calculations for current VAO
		double deltaTime2 = glfwGetTime() - currentTime2, deltaTime = glfwGetTime() - currentTime;
		currentTime2 += deltaTime2;
		switch (useDrawing)
		{
		case keyboardArray:
			if (window.isKeyPressed(zap::Key::left_arrow))
			{
				if (pathway == 1) camera.RotateDelta(0.1f * window.GetDelta() * 1.0f/*<- base speed: 20 is too fast*/, 0.00f, 0.0f);
				else if (pathway == 2) camera.RotateDelta(deltaTime2 * 50.f, 0.00f, 0.0f);
			}
			else if (window.isKeyPressed(zap::Key::right_arrow))
			{
				if (pathway == 1) camera.RotateDelta(-0.1f * window.GetDelta() * 1.0f /*<- base speed: 20 is too fast*/, 0.00f, 0.0f);
				else if (pathway == 2) camera.RotateDelta(-deltaTime2 * 50.f, 0.00f, 0.0f);
			}
			break;
		case bounceTimed:
			camera.RotateAbsolute(-90.0f, cos(deltaTime) * 30.f, 0.0f);
			break;
		}

		camera.UpdateRotation(); // Update camera vectors based on changed yaw and pitch
		view = camera.GetView();

		//here starts current VAO for current program draw
		mesh.bind(); //set current context before any draw routines, it prevents mess in more complex workflow
		glUniformMatrix4fv(viewLocationId, 1, GL_FALSE, glm::value_ptr(view));
		mesh.UseTexture(texture.i_id); //return false if texture not found
		mesh.Draw();
		//here draw ends
		
		window.Update();
		if (pathway == 1) window.Draw();
		else if (pathway == 2) glfwSwapBuffers(window.GetNativeWindow());
	}

	zap::Delete();

}





