#include "Zap.h"

int main()
{
	zap::Init();

	zap::Window window(1280, 720, "Hello Window");

	zap::InitGlad(); // There should be a error check if this never gets called

	while (window.Open())
	{
		zap::ClearBackground(zap::BackgroundColors::WHITE);

		window.Update();
		window.Draw();
	}

	zap::Delete();
}