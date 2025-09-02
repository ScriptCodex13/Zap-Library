#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

namespace zap
{   

    // Button/States

	enum class State : int
	{
		NONE        = -1,
		EZ_PRESSED  = GLFW_PRESS,
		EZ_RELEASED = GLFW_RELEASE
	};

	enum class Key : int
	{
		NONE = GLFW_KEY_UNKNOWN,
		A    = GLFW_KEY_A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		right_arrow = GLFW_KEY_RIGHT,
		left_arrow,
		down_arrow,
		up_arrow,
		NumPad_0 = GLFW_KEY_0,
		NumPad_1,
		NumPad_2,
		NumPad_3,
		NumPad_4,
		NumPad_5,
		NumPad_6,
		NumPad_7,
		NumPad_8,
		NumPad_9,
		Num_0K   = GLFW_KEY_KP_0,
		Num_1K,
		Num_2K,
		Num_3K,
		Num_4K,
		Num_5K,
		Num_6K,
		Num_7K,
		Num_8K,
		Num_9K,
		ESC      = GLFW_KEY_ESCAPE,
		ENTER    = GLFW_KEY_ENTER,
		SPACE    = GLFW_KEY_SPACE,
		L_SHIFT  = GLFW_KEY_LEFT_SHIFT,
		R_SHIFT  = GLFW_KEY_RIGHT_SHIFT,
		TAB      = GLFW_KEY_TAB,
		F1       = GLFW_KEY_F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		F25,
		LEFT_MOUSE = GLFW_MOUSE_BUTTON_LEFT,
		RIGHT_MOUSE,
		MIDDLE_MOUSE
	};

	enum class ControllerID : int
	{
		NONE = - 1,
		CONTROLLER_1 = GLFW_JOYSTICK_1,
		CONTROLLER_2,
		CONTROLLER_3,
		CONTROLLER_4,
		CONTROLLER_5,
		CONTROLLER_6,
		CONTROLLER_7,
		CONTROLLER_8,
		CONTROLLER_9,
		CONTROLLER_10,
		CONTROLLER_11,
		CONTROLLER_12,
		CONTROLLER_13,
		CONTROLLER_14,
		CONTROLLER_15,
		CONTROLLER_16
	};

	enum class ControllerButtonsPS  // Playstation Layout | ToDo: Add XBox 
	{
		RECTANGLE = 0,
		CROSS,
		CIRCLE,
		TRIANGLE,
		L1,
		R1,
		L2_Press,
		R2_Press,
		BACK,
		MENU,
		LEFT_STICK_PRESS,
		RIGHT_STICK_PRESS,
		PS_BUTTON,
		GUIDE_BUTTON,
		MUTE_BUTTON,
		DPAD_UP,
		DPAD_RIGHT,
		DPAD_DOWN,
		DPAD_LEFT
	};

	enum class ControllerTriggersPS // Playstation Layout
	{
		LEFT_STICK_X,
		LEFT_STICK_Y,
		RIGHT_STICK_X,
		L2,
		R2,
		RIGHT_STICK_Y,
	};

    //

	inline unsigned int highest_device_id = (unsigned int)ControllerID::CONTROLLER_1; //keeps track of the highest, not used Glfw_Joystick 

	class Device // manages a assigned GLFW_Joystick
	{
	public:
		bool IsConnected();
		bool GetButton(ControllerButtonsPS button, State state);
		const float GetTrigger(ControllerTriggersPS trigger);

	public:
		int present = 0;
		ControllerID ID;

	public:
		bool Controller_assigned = false;
	};

	Device AssignController(); // sets the settings for the new device class 

	namespace input
	{
		inline std::unordered_map<Key, State> key_states; // saves and manages all key states
		void UpdateInputs(GLFWwindow* window); // Updates all key states
		bool CheckInput(Key key, State state); // checks a key state
	};
}


#endif
