#include "enabler.h"
#ifdef SAMPLE_TEXT_PAINTER_CPP

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

template <typename T> class window_invoker : public zap::util::callback_invoker<T>
{
public:
	inline window_invoker(T _callback) : callback_invoker(_callback) {}
	template<typename A1> void operator () (A1& a1) { callback(a1); }
} ;
window_invoker  cbi([](zap::Window& window)
	{
		if (window.isKeyPressed(zap::Key::ESC))
		{
			window.Close();
		}
	});

//This is a proof of concept only
class TextPainter : public zap::Mesh
{

	const char* vertexCameraShaderSource = R"glsl(#version 330 core
		layout(location = 0) in vec3 aPos;
		layout(location = 1) in vec2 aTexCoord;
		out vec2 TexCoord;
		void main()
		{
			gl_Position = vec4(aPos, 1.0);
			TexCoord = aTexCoord;
		})glsl";

	const char* fragmentCameraShaderSource = R"glsl(#version 330
		out vec4 FragColor;
		in vec2 TexCoord;
		uniform sampler2D ourTexture;
		const vec4 defaultColor = vec4(0.1, 0.1, 0.1, 1.);
		void main()
		{
			FragColor = texture(ourTexture, TexCoord);
			if (FragColor.xyz == vec3 (0., 0., 0.))  FragColor = defaultColor;
		})glsl";

	std::vector<float> vertices =
	{
		// positions           // texture coords
		 1.0f,  0.2f, 0.0f,    1.0f, 1.0f,   // top right 
		 1.0f, -0.2f, 0.0f,    1.0f, 0.0f,   // bottom right  This causes the bug
		-1.0f, -0.2f, 0.0f,    0.0f, 0.0f,   // bottom left
		-1.0f,  0.2f, 0.0f,    0.0f, 1.0f    // top left		And also this

	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int textureHash;
public:
	zap::TextureText text;
	TextPainter() //:zap::Mesh(vertices, indices)
	{
		PreSetIndices(indices);
		PreSetVertices(vertices);
		SetVertexShaderSource(vertexCameraShaderSource);
		SetFragmentShaderSource(fragmentCameraShaderSource);

		SetAttribPointer(0, 3, 5, 0);
		SetAttribPointer(1, 2, 5, 3);

		//text.LoadFont("C:/Windows/Fonts/arial.ttf");
		text.LoadFont("C:/Windows/Fonts/msgothic.ttc");
		//text.LoadFont("C:/Windows/Fonts/OLDENGL.TTF");

		textureHash = AddTextureFromData(0, 0, 0,
			GL_RED, zap::TextureFilter::LINEAR,
			zap::MipmapSetting::LINEAR_MIPMAP_LINEAR,
			zap::TextureWrapping::CLAMP_TO_EDGE).getHash();

		Mesh::Finish();
	}

	std::vector<wchar_t> wprintf_buffer;
	int printf(const std::wstring content, ...)
	{
		//evaluate size first
		va_list arglist;
		va_start(arglist, content);
		int result = _vsnwprintf (nullptr, 0, content.c_str(), arglist);
		va_end(arglist);

		zap::util::vector_realloc <wchar_t>(wprintf_buffer, result + 1);

		//print text
		va_start(arglist, content);
		int retval = text.printf_t(this->GetTextureByHash (textureHash), wprintf_buffer.data(), result + 1, content.c_str(), arglist);
		va_end(arglist);
		return retval;
	}

	void Draw()
	{
		//here starts current VAO for current program draw
		Bind(); //set current context before any draw routines, it prevents mess in more complex programs
		BindTextureByHash(textureHash);
		Mesh::Draw();

	}
};

int main()
{
	zap::Init();

	zap::Window window(1920, 1080, "Hello Window");
	zap::util::scope_guard zapDeleter(zap::Delete);
	zap::InitGlad();

	//Mesh
	TextPainter mesh;

	window.UpdateViewport(); //This is a set callback. Once set == forever set

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (window.Open())
	{

		cbi(window);

		//from here draw starts
		//there starts general draw
		glClear(GL_DEPTH_BUFFER_BIT); // PR
		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		zap::ClearBackground(zap::BackgroundColor::BLACK);


		//here starts current VAO for current program draw
		//mesh.Bind(); //set current context before any draw routines, it prevents mess in more complex programs
		mesh.printf(L"Hello texture %f give five! 你好", glfwGetTime() * glfwGetTime() * glfwGetTime() * glfwGetTime());
		mesh.Draw();

		window.SetTitle(std::to_string(window.GetDelta()));

		window.Update();
		window.Draw();

	}
}

#endif