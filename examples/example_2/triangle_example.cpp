#include <Zap.h>

std::vector<float> vertices
{
	-0.4f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	 0.4f, -0.5f, 0.0f
};

const char* vertex_shader_source =
R"glsl(
		#version 330 core

		layout(location = 0) in vec3 vertices;

		void main()
		{
			gl_Position = vec4(vertices, 1.0);
		}
)glsl";

const char* fragment_shader_source =
R"glsl(
		#version 330 core

		out vec4 frag_color;

		void main()
		{
			frag_color = vec4(1.0, 0.0, 0.0, 1.0);
		}
)glsl";


int main()
{
	zap::Init(4, 6); // Use the newest OpenGL version

	zap::Window window(1920, 1080, "Hello Triangle ");

	zap::InitGlad();

	zap::Mesh triangle(vertices);

	triangle.SetVertexShaderSource(vertex_shader_source);
	triangle.SetFragmentShaderSource(fragment_shader_source);

	triangle.SetAttribPointer(0, 3, 3, 0);

	triangle.Finish();

	window.Maximize();
	window.UpdateViewport(true);

	while(window.Open())
	{
		glClear(GL_COLOR_BUFFER_BIT);

		triangle.UseProgram();
		triangle.Bind();
		triangle.Draw(3);

		zap::ShowWireFrame(window.isKeyPressed(zap::Key::A));

		window.Update();
		window.Draw();
	}

	zap::Delete();
}