#include "enabler.h"

#ifdef MAIN_ASSIMP_MODEL_LOADING_CPP

#include <Zap.h>
#include <Graphics.h>

std::string vertex_shader_source = 
	R"glsl(
			#version 330 core

			layout(location = 0) in vec3 aPos;

			uniform mat4 model;

			void main()
			{
				gl_Position = model * vec4(aPos, 1.0);
			}
	)glsl";

int main()
{
	zap::Init(4, 6);

	zap::Window window(1920, 1080, "Hello Model");

	zap::InitGlad();

	zap::ModelData modeldata = zap::LoadModel("models/TestCube.obj");

	window.Maximize();
	window.UpdateViewport(true);

	for(auto i : modeldata.meshes)
	{
		for(auto s : i.vertices)
		{
			std::cout << s << std::endl;
		}
	}

	zap::Mesh mesh(modeldata.meshes[0].vertices, modeldata.meshes[0].indices);

	mesh.SetAttribPointer(0, 3, 3, 0);

	mesh.Finish();



	while(window.Open())
	{
		glClear(GL_COLOR_BUFFER_BIT);

		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));

		mesh.Bind();
		mesh.Draw();

		window.Update();
		window.Draw();
	}

	zap::Delete();
}

#endif