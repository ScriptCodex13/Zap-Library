#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

namespace ez
{
	enum class State
	{
		NONE,
		EZ_PRESSED,
		EZ_RELEASED
	};

	enum class Key
	{
		NONE,
		A,
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
		left_arrow,
		right_arrow,
		up_arrow,
		down_arrow,
		NumPad_0,
		NumPad_1,
		NumPad_2,
		NumPad_3,
		NumPad_4,
		NumPad_5,
		NumPad_6,
		NumPad_7,
		NumPad_8,
		NumPad_9,
		Num_0K,
		Num_1K,
		Num_2K,
		Num_3K,
		Num_4K,
		Num_5K,
		Num_6K,
		Num_7K,
		Num_8K,
		Num_9K,
		ESC,
		ENTER,
		SPACE,
		L_SHIFT,
		R_SHIFT,
		TAB,
		F1,
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
		F25
	};


	namespace input
	{
		inline std::unordered_map<Key, State> key_states;

		void UpdateInputs(GLFWwindow* window);
		bool CheckInput(Key key, State state);
	};
}

#endif