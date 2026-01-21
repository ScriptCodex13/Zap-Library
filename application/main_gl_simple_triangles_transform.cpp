#include "enabler.h"

// Just a example
// PR = Prototyping -> only for testing 
#include <Zap.h>
#include <Graphics.h>
#include <iostream>
#include <vector>
#include <array>


#include <glm/glm.hpp>                  //basic glm math functions
#include <glm/gtc/matrix_transform.hpp> //matrix functions
#include <glm/gtc/type_ptr.hpp>         //convert glm types to opengl types
#include <Window/Window.h>
#include <os/system_font.h>

template <typename T> class window_invoker : public zap::util::callback_invoker<T>
{
public:
	inline window_invoker(T _callback) : callback_invoker(_callback) {}
	template<typename A1> void operator () (A1& a1) { callback(a1); }
};
window_invoker cbi([](zap::Window& window)
	{
		if (window.isKeyPressed(zap::Key::ESC))
		{
			window.Close();
		}
	});

static const char* vertexCameraShaderSource = R"glsl(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 ourColor;

uniform mat4 transformations[2];


void main()
{
	gl_Position = transformations[gl_InstanceID] * vec4(aPos, 1.0);
	ourColor = aColor;
})glsl";

static const char* fragmentCameraShaderSource = R"glsl(#version 330
out vec4 FragColor;
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 0);
})glsl";

int main_gl_simple_triangles_transform()
{
	zap::Init();

	zap::Window window(1920, 1080, "Hello Window");
	zap::util::scope_guard zapDeleter(zap::Delete);
	zap::InitGlad();

	//Mesh
	//TriangleTexture mesh;
	std::vector<float> vertices =
	{
		// positions            // texture coords
		 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,   // top right 
		 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // bottom right  This causes the bug
		-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,     0.0f, 1.0f    // top left		And also this

	};
	std::vector<unsigned int> indices =
	{
		2, 1, 0,
		3, 2, 0
	};

	zap::Mesh mesh (vertices, indices);
	mesh.SetVertexShaderSource   (vertexCameraShaderSource);
	mesh.SetFragmentShaderSource (fragmentCameraShaderSource);
	mesh.SetAttribPointer(0, 3, 5, 0);
	mesh.SetAttribPointer(1, 2, 5, 3);

	mesh.Finish();

	window.UpdateViewport(true); //This is a set callback. Once set == forever set

	unsigned int i_transformation_location = glGetUniformLocation(mesh.GetProgram(), "transformations");

	while (window.Open())
	{

		cbi(window);

		//from here draw starts
		//there starts general draw
		glClear(GL_DEPTH_BUFFER_BIT); // PR
		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		zap::ClearBackground(zap::BackgroundColor::BLACK);

		//here starts current VAO for current program draw
		mesh.UseProgram();

		glm::mat4 trans[2] =
			{
				glm::translate(glm::mat4(1.0f), glm::vec3(-0.7f, -0.7f, 0.0f)),
				glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, 0.7f, 0.0f))
			};
		glUniformMatrix4fv(i_transformation_location, std::size(trans), GL_FALSE, &trans[0][0].x);


		mesh.Bind(); //set current context before any draw routines, it prevents mess in more complex programs
		mesh.DrawInstanced(2);
		//here draw ends

		//window.SetTitle(std::to_string(window.GetDelta()));

		window.Update();
		window.Draw();

	}


	return 0;
}
#include "enabler.h"
#ifdef MAIN_GL_SIMPLE_TRIANGLES_TRANSFORM_CPP
int main() { return main_gl_simple_triangles_transform(); }
#endif