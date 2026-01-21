#include "enabler.h"

#include <Zap.h>
#include <Graphics.h>

#include <vector>

std::vector<float> vertices =
{
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

std::vector<unsigned int> indices =
{
	0,3,1,
	0,2,3
};

const char* computeshadersource = 
R"glsl(
		#version 460 core
		#extension GL_ARB_compute_shader: enable

		layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;

		layout(rgba32f, binding = 0) uniform image2D imgOutput;

		void main() 
		{
			vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
			ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	
			value.x = float(texelCoord.x)/(gl_NumWorkGroups.x);
			value.y = float(texelCoord.y)/(gl_NumWorkGroups.y);
	
			imageStore(imgOutput, texelCoord, value);
		}
)glsl";

const char* vertexshadersource = 
R"glsl(
		#version 460 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoords;

		out vec2 TexCoords;

		void main()
		{
			TexCoords = aTexCoords;
			gl_Position = vec4(aPos, 1.0);
		}
)glsl";

const char* fragmentshadersource = 
R"glsl(
		#version 460 core

		out vec4 FragColor;

		in vec2 TexCoords;

		uniform sampler2D tex;

		void main()
		{             
			vec3 texCol = texture(tex, TexCoords).rgb;      
			FragColor = vec4(texCol, 1.0);
		}
)glsl";



void showOpenGLExtensions()
{
	using std::cout;
	using std::endl;
	cout << "OpenGL Info:" << endl;
	cout << "       Vendor:       " << glGetString(GL_VENDOR) << endl;
	cout << "       Renderer:     " << glGetString(GL_RENDERER) << endl;
	cout << "       Version:      " << glGetString(GL_VERSION) << endl;
	cout << "       GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	GLint exts;
	glGetIntegerv(GL_NUM_EXTENSIONS, &exts);
	cout << "Number of extensions: " << exts << endl;
	for (int i = 0; i < exts; i++)
	{
		cout << "   " << i + 1 << ". " << glGetStringi(GL_EXTENSIONS, i) << endl;
	}
}
int main_compute_shader_test ()
{
	zap::Init(4, 6);

	zap::Window window(1920, 1080, "Window");

	zap::InitGlad();

	window.UpdateViewport(true);

	// Compute shader

	zap::ComputeShader computeshader;

	computeshader.SetSource(computeshadersource);

	computeshader.Finish();

	showOpenGLExtensions();
	//

	unsigned int texture;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1000, 1000, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	zap::Mesh mesh(vertices, indices);

	mesh.SetVertexShaderSource(vertexshadersource);
	mesh.SetFragmentShaderSource(fragmentshadersource);

	mesh.SetAttribPointer(0, 3, 5, 0);
	mesh.SetAttribPointer(1, 2, 5, 3);

	mesh.Finish();

	while(window.Open())
	{
		computeshader.UseProgram();
		computeshader.Run(1000 / 10, 1000 / 10, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		mesh.UseProgram();
		mesh.Bind();
		glBindTexture(GL_TEXTURE_2D, texture);
		mesh.Draw();

		window.Update();
		window.Draw();
	}

	glDeleteTextures(1, &texture);

	zap::Delete();
	return 0;
}

#include "enabler.h"
#ifdef MAIN_COMPUTE_SHADER_TEST
int main() { return main_compute_shader_test(); }
#endif