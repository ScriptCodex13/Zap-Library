#include "Window.h"
#include "stb_image.h"

namespace zap
{
	/***********************************************************************************/

	//Intern callback functions

	void internwindowCloseCallback(GLFWwindow* window)
	{
		if (window)
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	/***********************************************************************************/

	/***********************************************/

	//Viewport function

	void i_setFramebuffersize(GLFWwindow* window, int size_x, int size_y)
	{
		glViewport(0, 0, size_x, size_y);
	}

	void i_FramebuffersizeCallback(GLFWwindow* window ,int size_x, int size_y)
	{
		if (window)
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			glViewport(0, 0, width, height);
		}
	}

	/***********************************************/

	Window::Window(int scale_x, int scale_y, const std::string Title, GLFWmonitor* monitor, GLFWwindow* other_window)
	{

		intern_window = glfwCreateWindow(scale_x, scale_y, Title.c_str(), monitor, other_window);

		if (intern_window)
		{
			currentMonitor = glfwGetWindowMonitor(intern_window);
		}

		if (!intern_window)
		{
			messages::PrintMessage("Failed to create window", "Window.cpp/Window::Window(...)", MessageTypes::fatal_error);
		}

		if (intern_window)
		{
			glfwMakeContextCurrent(intern_window);
			glfwGetWindowSize(intern_window, &width, &height);
			glfwGetWindowPos(intern_window, &pos_x, &pos_y);
		}
	}
	Window::Window(GLFWwindow* extern_window)
	{
		intern_window = extern_window;

		if (intern_window)
		{
			currentMonitor = glfwGetWindowMonitor(intern_window);
		}

		if (!intern_window)
		{
			messages::PrintMessage("Failed to get extern window", "Window.cpp/Window::Window(...)", MessageTypes::fatal_error);
		}

		if (intern_window)
		{
			glfwMakeContextCurrent(intern_window);
			glfwGetWindowSize(intern_window, &width, &height);
			glfwGetWindowPos(intern_window, &pos_x, &pos_y);
		}
	}

	Window::~Window()
	{
		glfwDestroyWindow(intern_window);
	}

	GLFWwindow* Window::GetNativeWindow()
	{
		if (intern_window)
		{
			return intern_window;
		}
		else
		{
			return nullptr;
		}
	}

	bool Window::Open()
	{
		if (intern_window)
		{
			if (!glfwWindowShouldClose(intern_window))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	void Window::UpdateViewport()
	{
		if (intern_window)
		{
			glfwSetFramebufferSizeCallback(intern_window, i_FramebuffersizeCallback);
		}
	}

	void Window::SetViewport(unsigned int new_width, unsigned int new_height)
	{
		if (intern_window)
		{
			i_setFramebuffersize(intern_window, new_width, new_height);
		}
	}

	void Window::Close()
	{
		if (intern_window)
		{
			glfwSetWindowShouldClose(intern_window, true);
		}
	}

	void Window::SetFullscreen(bool state)
	{
		if (intern_window)
		{
			if (state)
			{
				glfwSetWindowMonitor(intern_window, currentMonitor, pos_x, pos_y, width, height, current_refresh_rate);
			}
			else
			{
				glfwSetWindowMonitor(intern_window, NULL, pos_x, pos_y, width, height, current_refresh_rate);
			}
			
		}
	}

	void Window::SetFullscreen(bool state, GLFWmonitor* monitor)
	{
		if (intern_window)
		{
			if (state)
			{
				glfwSetWindowMonitor(intern_window, monitor, pos_x, pos_y, width, height, current_refresh_rate);
				currentMonitor = glfwGetWindowMonitor(intern_window);
			}
			else
			{
				glfwSetWindowMonitor(intern_window, NULL, pos_x, pos_y, width, height, current_refresh_rate);
			}

		}
	}

	void Window::Maximize(bool update_viewport)
	{
		if (intern_window)
		{
			glfwMaximizeWindow(intern_window);
			
			//Updating the Viewport

			if (update_viewport)
			{
				int width, height;

				glfwGetWindowSize(intern_window, &width, &height);
				glViewport(0, 0, width, height);
			}

			//
		}
	}

	void Window::Minimize()
	{
		if (intern_window)
		{
			glfwIconifyWindow(intern_window);
		}
	}

	void Window::ClearBackground(BackgroundColors color)
	{
		if (color == BackgroundColors::WHITE)
		{
			glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (color == BackgroundColors::BLACK)
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (color == BackgroundColors::RED)
		{
			glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (color == BackgroundColors::GREEN)
		{
			glClearColor(0.0f, 1.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (color == BackgroundColors::BLUE)
		{
			glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (color == BackgroundColors::YELLOW)
		{
			glClearColor(1.0f, 0.9f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (color == BackgroundColors::ORANGE)
		{
			glClearColor(1.0f, 0.7f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (color == BackgroundColors::PURPLE)
		{
			glClearColor(0.8f, 0.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (color == BackgroundColors::PINK)
		{
			glClearColor(1.0f, 0.0f, 1.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	void Window::ClearBackground(float RED, float GREEN, float BLUE, float ALPHA)
	{
		RED = std::clamp(RED, 0.0f, 1.0f);
		GREEN = std::clamp(GREEN, 0.0f, 1.0f);
		BLUE = std::clamp(BLUE, 0.0f, 1.0f);
		ALPHA = std::clamp(ALPHA, 0.0f, 1.0f);

		glClearColor(RED, GREEN, BLUE, ALPHA);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::SetIcon(const std::string path)
	{
		windowIcon[0].pixels = stbi_load(path.c_str(), &windowIcon[0].width, &windowIcon[0].height, &icon_channels, 0);

		if (intern_window)
		{
			glfwSetWindowIcon(intern_window, 1, windowIcon);
		}
	}

	float Window::GetFPS()
	{
		return current_FPS;
	}

	float Window::GetDelta()
	{
		return current_Frametime;
	}

	void Window::SetSize(unsigned int new_width, unsigned int new_height)
	{
		width = new_width;
		height = new_height;

		if (intern_window)
		{
			if (currentMonitor)
			{
				glfwSetWindowMonitor(intern_window, currentMonitor, pos_x, pos_y, width, height, current_refresh_rate);
			}
		}
	}

	void Window::SetPosition(int x, int y)
	{
		pos_x = x;
		pos_y = y;

		if (intern_window)
		{
			if (currentMonitor)
			{
				glfwSetWindowMonitor(intern_window, currentMonitor, pos_x, pos_y, width, height, current_refresh_rate);
			}
		}
	}

	void Window::SetFPSLimit(unsigned int limit)
	{
		FPSLimit = limit;

		TargetTime = std::round(1.0f / limit * 1000) / 1000;

		std::cout << TargetTime << std::endl;
	}

	void Window::SetTitle( const std::string title)
	{
		if (intern_window)
		{
			glfwSetWindowTitle(intern_window, title.c_str());
		}
	}

	bool Window::GetInput(Key key, State state)
	{
		if (intern_window)
		{
			return input::CheckInput(key, state);
		}
		
		return false;
	}

	void Window::ShowWireFrame(bool state)
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

	std::array<double, 2> Window::GetMousePosition()
	{
		double x, y;

		if (intern_window)
		{
			glfwGetCursorPos(intern_window, &x, &y);
			
			return { x, y };
		}

		messages::PrintMessage("Can not get the mouse Position", "Window.cpp/std::array<double, 2> ez::Window::GetMousePosition()", MessageTypes::error);

		return { 0,0 };
	}

	void Window::HideCursor(bool state)
	{
		if (intern_window)
		{
			if (state)
			{
				glfwSetInputMode(intern_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
			else
			{
				glfwSetInputMode(intern_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}


	//Private func 

	void Window::InternSwapBuffers()
	{
		if (intern_window)
		{
			auto now = std::chrono::steady_clock::now();

			float CurrentTime = glfwGetTime();

			std::chrono::duration<float> Time = now - currentTime;





			float Frametime = CurrentTime - LastTime;

			FrametimeBuffer += Frametime;

			//std::cout << FrametimeBuffer << std::endl;

			//std::cout << Time.count() << std::endl;

			if (FrametimeBuffer >= TargetTime)
			{
				glfwSwapBuffers(intern_window);

				//std::cout << FrametimeBuffer << std::endl;

				current_FPS = 1.0f / FrametimeBuffer;

				FrametimeBuffer = 0.0f;

				auto now = std::chrono::steady_clock::now();

				std::chrono::duration<float> frametime = now - waitTime;
				current_Frametime = frametime.count();

				waitTime = std::chrono::steady_clock::now();
			}
		}

		currentTime = std::chrono::steady_clock::now(); // Restart the time counter

		LastTime = glfwGetTime();
	}

	//

	void Window::Update()
	{
		if (intern_window)
		{
			glfwPollEvents();

			input::UpdateInputs(intern_window);

		}
	}

	void Window::Draw()
	{
		InternSwapBuffers();
	}

}