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

	namespace input
	{
		void UpdateInputs(GLFWwindow* window)
		{
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				key_states[Key::A] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
			{
				key_states[Key::B] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			{
				key_states[Key::C] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				key_states[Key::D] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			{
				key_states[Key::E] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			{
				key_states[Key::F] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
			{
				key_states[Key::G] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			{
				key_states[Key::H] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			{
				key_states[Key::I] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			{
				key_states[Key::J] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			{
				key_states[Key::K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			{
				key_states[Key::L] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
			{
				key_states[Key::M] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
			{
				key_states[Key::N] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			{
				key_states[Key::O] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			{
				key_states[Key::P] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			{
				key_states[Key::Q] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			{
				key_states[Key::R] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				key_states[Key::S] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			{
				key_states[Key::T] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			{
				key_states[Key::U] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
			{
				key_states[Key::V] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				key_states[Key::W] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			{
				key_states[Key::X] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
			{
				key_states[Key::Y] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			{
				key_states[Key::Z] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				key_states[Key::left_arrow] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				key_states[Key::right_arrow] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				key_states[Key::up_arrow] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				key_states[Key::down_arrow] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
			{
				key_states[Key::NumPad_0] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			{
				key_states[Key::NumPad_1] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			{
				key_states[Key::NumPad_2] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			{
				key_states[Key::NumPad_3] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			{
				key_states[Key::NumPad_4] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
			{
				key_states[Key::NumPad_5] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
			{
				key_states[Key::NumPad_6] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
			{
				key_states[Key::NumPad_7] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
			{
				key_states[Key::NumPad_8] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
			{
				key_states[Key::NumPad_9] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS)
			{
				key_states[Key::Num_0K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
			{
				key_states[Key::Num_1K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
			{
				key_states[Key::Num_3K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
			{
				key_states[Key::Num_4K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
			{
				key_states[Key::Num_5K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
			{
				key_states[Key::Num_6K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
			{
				key_states[Key::Num_7K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
			{
				key_states[Key::Num_8K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
			{
				key_states[Key::Num_9K] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				key_states[Key::ESC] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			{
				key_states[Key::ENTER] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				key_states[Key::SPACE] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				key_states[Key::L_SHIFT] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			{
				key_states[Key::R_SHIFT] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
			{
				key_states[Key::TAB] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
			{
				key_states[Key::F1] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
			{
				key_states[Key::F2] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
			{
				key_states[Key::F3] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
			{
				key_states[Key::F4] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
			{
				key_states[Key::F5] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
			{
				key_states[Key::F6] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS)
			{
				key_states[Key::F7] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS)
			{
				key_states[Key::F8] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS)
			{
				key_states[Key::F9] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS)
			{
				key_states[Key::F10] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
			{
				key_states[Key::F11] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS)
			{
				key_states[Key::F12] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F13) == GLFW_PRESS)
			{
				key_states[Key::F13] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F14) == GLFW_PRESS)
			{
				key_states[Key::F14] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F15) == GLFW_PRESS)
			{
				key_states[Key::F15] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F16) == GLFW_PRESS)
			{
				key_states[Key::F16] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F17) == GLFW_PRESS)
			{
				key_states[Key::F17] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F18) == GLFW_PRESS)
			{
				key_states[Key::F18] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F19) == GLFW_PRESS)
			{
				key_states[Key::F19] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F20) == GLFW_PRESS)
			{
				key_states[Key::F20] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F21) == GLFW_PRESS)
			{
				key_states[Key::F21] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F22) == GLFW_PRESS)
			{
				key_states[Key::F22] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F23) == GLFW_PRESS)
			{
				key_states[Key::F23] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F24) == GLFW_PRESS)
			{
				key_states[Key::F24] = State::EZ_PRESSED;
			}
			if (glfwGetKey(window, GLFW_KEY_F25) == GLFW_PRESS)
			{
				key_states[Key::F25] = State::EZ_PRESSED;
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				key_states[Key::LEFT_MOUSE] = State::EZ_PRESSED;
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				key_states[Key::RIGHT_MOUSE] = State::EZ_PRESSED;
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
			{
				key_states[Key::MIDDLE_MOUSE] = State::EZ_PRESSED;
			}

			//**************************************************************

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
			{
				key_states[Key::A] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
			{
				key_states[Key::B] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
			{
				key_states[Key::C] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
			{
				key_states[Key::D] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
			{
				key_states[Key::E] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
			{
				key_states[Key::F] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
			{
				key_states[Key::G] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE)
			{
				key_states[Key::H] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE)
			{
				key_states[Key::I] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE)
			{
				key_states[Key::J] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
			{
				key_states[Key::K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
			{
				key_states[Key::L] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
			{
				key_states[Key::M] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE)
			{
				key_states[Key::N] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE)
			{
				key_states[Key::O] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
			{
				key_states[Key::P] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
			{
				key_states[Key::Q] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
			{
				key_states[Key::R] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
			{
				key_states[Key::S] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
			{
				key_states[Key::T] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_U) == GLFW_RELEASE)
			{
				key_states[Key::U] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE)
			{
				key_states[Key::V] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
			{
				key_states[Key::W] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE)
			{
				key_states[Key::X] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_RELEASE)
			{
				key_states[Key::Y] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
			{
				key_states[Key::Z] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
			{
				key_states[Key::left_arrow] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
			{
				key_states[Key::right_arrow] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
			{
				key_states[Key::up_arrow] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
			{
				key_states[Key::down_arrow] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_0) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_0] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_1] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_2] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_3] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_4] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_5) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_5] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_6) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_6] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_7) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_7] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_8) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_8] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_9) == GLFW_RELEASE)
			{
				key_states[Key::NumPad_9] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_RELEASE)
			{
				key_states[Key::Num_0K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_RELEASE)
			{
				key_states[Key::Num_1K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_RELEASE)
			{
				key_states[Key::Num_3K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_RELEASE)
			{
				key_states[Key::Num_4K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_RELEASE)
			{
				key_states[Key::Num_5K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_RELEASE)
			{
				key_states[Key::Num_6K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_RELEASE)
			{
				key_states[Key::Num_7K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_RELEASE)
			{
				key_states[Key::Num_8K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_RELEASE)
			{
				key_states[Key::Num_9K] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
			{
				key_states[Key::ESC] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
			{
				key_states[Key::ENTER] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
			{
				key_states[Key::SPACE] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			{
				key_states[Key::L_SHIFT] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
			{
				key_states[Key::R_SHIFT] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
			{
				key_states[Key::TAB] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
			{
				key_states[Key::F1] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE)
			{
				key_states[Key::F2] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_RELEASE)
			{
				key_states[Key::F3] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_RELEASE)
			{
				key_states[Key::F4] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_RELEASE)
			{
				key_states[Key::F5] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_RELEASE)
			{
				key_states[Key::F6] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_RELEASE)
			{
				key_states[Key::F7] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_RELEASE)
			{
				key_states[Key::F8] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_RELEASE)
			{
				key_states[Key::F9] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_RELEASE)
			{
				key_states[Key::F10] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_RELEASE)
			{
				key_states[Key::F11] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F12) == GLFW_RELEASE)
			{
				key_states[Key::F12] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F13) == GLFW_RELEASE)
			{
				key_states[Key::F13] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F14) == GLFW_RELEASE)
			{
				key_states[Key::F14] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F15) == GLFW_RELEASE)
			{
				key_states[Key::F15] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F16) == GLFW_RELEASE)
			{
				key_states[Key::F16] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F17) == GLFW_RELEASE)
			{
				key_states[Key::F17] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F18) == GLFW_RELEASE)
			{
				key_states[Key::F18] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F19) == GLFW_RELEASE)
			{
				key_states[Key::F19] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F20) == GLFW_RELEASE)
			{
				key_states[Key::F20] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F21) == GLFW_RELEASE)
			{
				key_states[Key::F21] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F22) == GLFW_RELEASE)
			{
				key_states[Key::F22] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F23) == GLFW_RELEASE)
			{
				key_states[Key::F23] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F24) == GLFW_RELEASE)
			{
				key_states[Key::F24] = State::EZ_RELEASED;
			}
			if (glfwGetKey(window, GLFW_KEY_F25) == GLFW_RELEASE)
			{
				key_states[Key::F25] = State::EZ_RELEASED;
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			{
				key_states[Key::LEFT_MOUSE] = State::EZ_RELEASED;
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
			{
				key_states[Key::RIGHT_MOUSE] = State::EZ_RELEASED;
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
			{
				key_states[Key::MIDDLE_MOUSE] = State::EZ_RELEASED;
			}
		}

		bool CheckInput(Key key, State state)
		{
			
			if (key_states.find(key) == key_states.end())
			{
				return false;
			}

			return key_states[key] == state;
		}
	}

}
