#include "RenderCalls.h"

namespace zap
{
	void ClearBackground(BackgroundColor color)  // Clears the backround with a predefined color 
	{
		switch (color)
		{
		case BackgroundColor::WHITE:
			glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColor::BLACK:
			glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColor::RED:
			glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColor::GREEN:
			glClearColor(0.0f, 1.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColor::BLUE:
			glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColor::YELLOW:
			glClearColor(1.0f, 0.9f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColor::ORANGE:
			glClearColor(1.0f, 0.7f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColor::PURPLE:
			glClearColor(0.8f, 0.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColor::PINK:
			glClearColor(1.0f, 0.0f, 0.5f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		}
	}

	void ClearBackground(float RED, float GREEN, float BLUE, float ALPHA)  // Clears the Background with a self defined color based on the RGBA settings given in the parameters
	{
		RED = std::clamp(RED, 0.0f, 1.0f);
		GREEN = std::clamp(GREEN, 0.0f, 1.0f);
		BLUE = std::clamp(BLUE, 0.0f, 1.0f);
		ALPHA = std::clamp(ALPHA, 0.0f, 1.0f);

		glClearColor(RED, GREEN, BLUE, ALPHA);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void ShowWireFrame(bool state)  // Enable or disable the wireframe mode
	{
		if (state)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void ClearDepthBuffer() // Clears the depth buffer
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void ClearBuffers() // Clears color and depth buffer
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Enable(Instruction instruction)
	{
		glEnable((GLenum)instruction);
	}

	void Disable(Instruction instruction)
	{
		glDisable((GLenum)instruction);
	}
}