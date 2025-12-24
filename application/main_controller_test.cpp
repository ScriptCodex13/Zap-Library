#include "enabler.h"

#ifdef MAIN_CONTROLLER_TEST_CPP

#include <Zap.h>

int main()
{
	zap::Init();

	zap::Window window(1920, 1080, "Controller Test");

	zap::InitGlad();

	zap::Device controller = zap::AssignController();

	while(window.Open())
	{
		if (controller.IsConnected())
		{
			if (controller.GetButton(zap::ControllerButtonsXB::A, zap::State::PRESSED))
			{
				std::cout << "Pressed" << std::endl;
			}

			std::cout << controller.GetTrigger(zap::ControllerTriggersXB::RT) << std::endl;
		}

		window.Update();
		window.Draw();
	}

	zap::Delete();
}

#endif
