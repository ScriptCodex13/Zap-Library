#pragma once

#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"
#include "Message.h"


#include <algorithm>
#include <chrono>
#include <thread>
#include <cmath>
#include <array>


#ifndef WINDOW_H
#define WINDOW_H

namespace zap
{
	enum class BackgroundColors
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


	/*****************************************************************************/

	//TODO: Yes it is Window, no doubt. Comment is redundant:
	//Window

	class Window
	{
	public:
		Window(int scale_x, int scale_y, const std::string Title, GLFWmonitor* monitor = NULL, GLFWwindow* other_window = NULL);
		Window(GLFWwindow* extern_window); // You dont have to handle the window anymore it is now handled by EasyGL
		~Window();

	public:
		GLFWwindow* GetNativeWindow();
		bool Open();
		void UpdateViewport();
		void SetViewport(unsigned int new_width, unsigned int new_height);
		void Close();
		void SetFullscreen(bool state);
		void SetFullscreen(bool state, GLFWmonitor* monitor);
		void Maximize(bool update_viewport = true);
		void Minimize();
		void ClearBackground(BackgroundColors color);
		void ClearBackground(float RED, float GREEN, float BLUE, float ALPHA);
		float GetFPS();
		float GetDelta();
		void SetSize(unsigned int new_width, unsigned int new_height);
		void SetPosition(int x, int y);
		void SetFPSLimit(unsigned int limit); //TODO Get it to work correctly
		void SetTitle(const std::string title);
		bool GetInput(Key key, State state);
		bool GetInput(int key, int   state);
		bool isKeyPressed(Key key);
		bool isKeyPressed(int key);
		bool isKeyReleased(Key key);
		bool isKeyReleased(int key);
		bool isMousePressed(Key key);
		bool isMousePressed(int key);
		bool isMouseReleased(Key key);
		bool isMouseReleased(int key);
		void ShowWireFrame(bool state);
		void SetIcon(const std::string path);
		std::array<double, 2> GetMousePosition();
		void HideCursor(bool state);

		void Update();
		//void Write(); Draw Things
		void Draw();

	private:
		GLFWwindow* intern_window = nullptr;
		GLFWmonitor* currentMonitor = nullptr;
		int width  = 0;
		int height = 0;
		int pos_x  = 0;
		int pos_y  = 0;
		int current_refresh_rate = 1000000000;

	private: //Icon
		GLFWimage windowIcon[1];
		int icon_channels;

	private: //Time
		float current_FPS = 0;
		float current_Frametime = 0;

		unsigned int FPSLimit = 1000000;
		float TargetTime;
		float FrametimeBuffer = 0;						// Adds the Frametimes together until the TargetTime is reached. 

		float LastTime;

		std::chrono::steady_clock::time_point waitTime;
		std::chrono::steady_clock::time_point currentTime; // Keeps track of the time which is passed to limit the FPS correctly

	private:
		void InternSwapBuffers(); // Better for managing the FPSLimit


	};


}

#endif