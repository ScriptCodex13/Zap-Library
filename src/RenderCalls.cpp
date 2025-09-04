#include "RenderCalls.h"

namespace zap
{
	void ClearBackground(BackgroundColors color)
	{
		switch (color)
		{
		case BackgroundColors::WHITE:
			glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColors::BLACK:
			glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColors::RED:
			glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColors::GREEN:
			glClearColor(0.0f, 1.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColors::BLUE:
			glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColors::YELLOW:
			glClearColor(1.0f, 0.9f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColors::ORANGE:
			glClearColor(1.0f, 0.7f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColors::PURPLE:
			glClearColor(0.8f, 0.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		case BackgroundColors::PINK:
			glClearColor(1.0f, 0.0f, 0.5f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			return;
		}
	}

	void ClearBackground(float RED, float GREEN, float BLUE, float ALPHA)
	{
		RED = std::clamp(RED, 0.0f, 1.0f);
		GREEN = std::clamp(GREEN, 0.0f, 1.0f);
		BLUE = std::clamp(BLUE, 0.0f, 1.0f);
		ALPHA = std::clamp(ALPHA, 0.0f, 1.0f);

		glClearColor(RED, GREEN, BLUE, ALPHA);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void ShowWireFrame(bool state)
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

	void ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}