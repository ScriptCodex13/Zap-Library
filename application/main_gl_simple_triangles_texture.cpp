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

template <typename T> class window_camera_invoker : public zap::util::callback_invoker<T>
{
public:
	inline window_camera_invoker(T _callback) : callback_invoker(_callback) {}
	template<typename A1> void operator () (A1& a1) { callback(a1); }
};
window_camera_invoker  cbi([](zap::Window& window)
	{
		if (window.isKeyPressed(zap::Key::ESC))
		{
			window.Close();
		}
	});

class TextPainter : public zap::Mesh
{

	const char* vertexCameraShaderSource = R"glsl(#version 330 core
		layout(location = 0) in vec3 aPos;
		layout(location = 1) in vec2 aTexCoord;
		out vec2 TexCoord;

		void main()
		{
			gl_Position =  vec4(aPos, 1.0);
			TexCoord = aTexCoord;
		})glsl";

	const char* fragmentCameraShaderSource = R"glsl(#version 330
		out vec4 FragColor;
		in vec2 TexCoord;

		uniform sampler2D ourTexture;
		void main()
		{
			FragColor = texture(ourTexture, TexCoord);
		})glsl";


	std::vector<float> vertices =
	{
		// positions           // texture coords
		 0.5f,  0.5f, 0.0f,    1.0f, 1.0f,   // top right 
		 0.5f, -0.5f, 0.0f,    1.0f, 0.0f,   // bottom right  
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,    0.0f, 1.0f    // top left

	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int textureHash;
public:
	TextPainter() //:zap::Mesh(vertices, indices)
	{
		PreSetIndices(indices);
		PreSetVertices(vertices);
		SetVertexShaderSource(vertexCameraShaderSource);
		SetFragmentShaderSource(fragmentCameraShaderSource);

		SetAttribPointer(0, 3, 5, 0);
		SetAttribPointer(1, 2, 5, 3);
		//SetAttribPointer(2, 2, 5, 6);
		//textureHash = AddTexture(0, "textures/texture.png", zap::TextureFilter::NEAREST, zap::MipmapSetting::LINEAR_MIPMAP_LINEAR, zap::TextureWrapping::CLAMP_TO_BORDER).getHash();
		textureHash = AddTextureFromPath(0, "textures/learnopengl/airplane.png", zap::TextureFilter::NEAREST, zap::MipmapSetting::NEAREST_MIPMAP_NEAREST, zap::TextureWrapping::CLAMP_TO_EDGE).getHash();
		Finish();
	}

	void Draw()
	{
		BindTextureByHash(textureHash);
		Mesh::Draw();
	}
};
//TODO: this app crashes
int main_gl_simple_triangles_texture()
{
	zap::Init();

	zap::Window window(1920, 1080, "Hello Window");
	zap::util::scope_guard zapDeleter(zap::Delete);
	zap::InitGlad();

	//Mesh
	{
		TextPainter mesh;

		//mesh.Finish();

		window.UpdateViewport(); //This is a set callback. Once set == forever set

		while (window.Open())
		{
			cbi(window);

			//from here draw starts
			//there starts general draw
			glClear(GL_DEPTH_BUFFER_BIT); // PR
			zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));
			zap::ClearBackground(zap::BackgroundColor::BLACK);

			//here starts current VAO for current program draw
			mesh.Bind(); //set current context before any draw routines, it prevents mess in more complex programs
			mesh.Draw();
			//here draw ends

			window.SetTitle(std::to_string(window.GetDelta()));

			window.Update();
			window.Draw();

		}
	}
	return 0;
}


#include "enabler.h"
#ifdef MAIN_GL_SIMPLE_TRIANGLES_TEXTURE_CPP
int main() { return main_gl_simple_triangles_texture(); }
#endif