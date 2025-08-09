#pragma once

#ifndef EASYGL_H
#define EASYGL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Additional includes

#include "Message.h"
#include "Input.h"

#include <algorithm>
#include <chrono>
#include <thread>
#include <cmath>

//

namespace ez
{
	/*****************************************************************************/
	
	//APIs Resources

	void Init();
	void InitGlad();
	void Delete();

	/*****************************************************************************/

	
	
	/*****************************************************************************/

	//Window 

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
		void SetIcon(const std::string path);
		void StartTime();
		float GetFPS();
		float GetDelta();
		void SetSize(unsigned int new_width, unsigned int new_height);
		void SetPosition(int x, int y);
		void SetFPSLimit(unsigned int limit); //TODO Get it to work correctly
		void SetTitle(const std::string title);	
		bool GetInput(Key key, State state);
		void ShowWireFrame(bool state);

		void Update();
		//void Write(); Draw Things
		void Draw();

	private:
		GLFWwindow* intern_window = nullptr;
		GLFWmonitor* currentMonitor = nullptr;
		int width;
		int height;
		int pos_x;
		int pos_y;
		int current_refresh_rate = 1000000000;
		int FPSTarget = 1000000000;

	private: //Time
		float current_FPS = 0;
		float current_Frametime = 0;
		std::chrono::steady_clock::time_point waitTime;
		std::chrono::steady_clock::time_point currentTime; // Keeps track of the time which is passed to limit the FPS correctly
		
	};

	/*****************************************************************************/

}

#endif
