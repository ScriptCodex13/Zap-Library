#pragma once

#ifndef RENDER_CALLS_H
#define RENDER_CALLS_H

#include <glad/glad.h>
#include <algorithm>

namespace zap
{
	enum class BackgroundColor
	{
		WHITE,
		BLACK,
		RED,
		GREEN,
		BLUE,
		YELLOW,
		ORANGE,
		PURPLE,
		PINK

	};

	enum class Instruction
	{
		ANTIALIASING = GL_MULTISAMPLE,
		DEPTH = GL_DEPTH_TEST

	};

	void ClearBackground(BackgroundColor color);                       // Clears the backround with a predefined color 
	void ClearBackground(float RED, float GREEN, float BLUE, float ALPHA);  // Clears the Background with a self defined color based on the RGBA settings given in the parameters
	void ShowWireFrame(bool state);                                     // Enable or disable the wireframe
	void ClearDepthBuffer();											// Clears the depth buffer
	void ClearBuffers();												// Clears color and depth buffer

	void Enable(Instruction instruction);
	void Disable(Instruction instruction);


}

#endif