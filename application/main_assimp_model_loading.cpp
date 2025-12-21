#include "enabler.h"

#ifdef MAIN_ASSIMP_MODEL_LOADING_CPP

#include <Zap.h>
#include <Graphics.h>

int main()
{
	zap::Init(4, 6);

	zap::Window window(1920, 1080, "Hello Model");

	zap::InitGlad();

	zap::ModelData modeldata = zap::LoadModel("TestCube.obj");

	window.Maximize();
	window.UpdateViewport(true);

	while(window.Open())
	{

		window.Update();
		window.Draw();
	}

	zap::Delete();
}

#endif