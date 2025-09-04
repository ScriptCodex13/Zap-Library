#include "Input.h"
#include "Message.h"

#include <iostream>

namespace zap
{

	bool Device::IsConnected()
	{
		if (Controller_assigned)
		{
			present = glfwJoystickPresent((int)ID);
			
			if (present)
			{
				return true;
			}
		}

		return false;
	}

	bool Device::GetButton(ControllerButtonsPS button, State state)
	{
		int count;

		const unsigned char* buttons = glfwGetJoystickButtons((int)ID, &count);

		if (Controller_assigned)
		{
			if (buttons[(int)button] == (GLenum)state)
			{
				return true;
			}
		}
		
		return false;
	}

	const float Device::GetTrigger(ControllerTriggersPS trigger)
	{
		if (Controller_assigned)
		{
			int count;
			const float* axis = glfwGetJoystickAxes((int)ID, &count);

			return axis[(int)trigger];
		}

		messages::PrintMessage("Controller is not connected" , "Input.cpp/ const float Device::GetTrigger(...)", MessageTypes::warning);
        
		return 0.0f;
	}


	Device AssignController()
	{
		Device i_device;

		i_device.ID = (ControllerID)highest_device_id;
		i_device.Controller_assigned = true;

		highest_device_id++;

		return i_device;
	}

	
}
