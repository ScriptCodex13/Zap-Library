#include "Window.h"
#include "stb_image.h"
#include "../Util//Util.h"

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

	void i_FramebuffersizeCallback(GLFWwindow* window ,int size_x, int size_y)
	{
		if (window)
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			glViewport(0, 0, width, height);
		}
	}

	void i_nFramebuffersizeCallback(GLFWwindow* window, int size_x, int size_y) {}

	/***********************************************/

	Window::Window(int scale_x, int scale_y, const std::string Title, GLFWmonitor* monitor, GLFWwindow* other_window)
	{

		intern_window = glfwCreateWindow(scale_x, scale_y, Title.c_str(), monitor, other_window);

		if (!intern_window)
		{
			messages::PrintMessage("Failed to create window", "Window.cpp/Window::Window(...)", MessageTypes::fatal_error);
			ZAP_INTERRUPT_FATAL_ERROR;
		}

		if (monitor == NULL) currentMonitor = glfwGetPrimaryMonitor();

		else currentMonitor = glfwGetWindowMonitor(intern_window); // The same for the other constructor

		

		glfwMakeContextCurrent(intern_window);
		glfwGetWindowSize(intern_window, &i_window_dimensions[0], &i_window_dimensions[1]);
		glfwGetWindowPos(intern_window, &pos_x, &pos_y);

		TargetFrameTime = std::round(1.0f / FPSLimit * 100000) / 100000;

	}
	Window::Window(GLFWwindow* extern_window)
	{
		intern_window = extern_window;

		if (!intern_window)
		{
			messages::PrintMessage("Failed to get extern window", "Window.cpp/zap::Window::Window(...)", MessageTypes::fatal_error);
			ZAP_INTERRUPT_FATAL_ERROR;
		}

		if (glfwGetWindowMonitor(intern_window) == NULL) currentMonitor = glfwGetPrimaryMonitor();
		
		else currentMonitor = glfwGetWindowMonitor(intern_window);

		glfwMakeContextCurrent(intern_window);
		glfwGetWindowSize(intern_window, &i_window_dimensions[0], &i_window_dimensions[1]);
		glfwGetWindowPos(intern_window, &pos_x, &pos_y);

	}

	Window::~Window()
	{
		glfwDestroyWindow(intern_window);
	}

	GLFWwindow* Window::GetNativeWindow()
	{
		return intern_window;
	}

	bool Window::Open()
	{
		if (!intern_window) return false;

		if (!glfwWindowShouldClose(intern_window))
		{
			return true;
		}
		
		return false;
	}

	void Window::UpdateViewport(bool state)
	{
		if (!intern_window) return;
		
		if (state)
		{
			glfwSetFramebufferSizeCallback(intern_window, i_FramebuffersizeCallback);
		}
		else
		{

			glfwSetFramebufferSizeCallback(intern_window, i_nFramebuffersizeCallback);
		}
		
	}

	void Window::SetViewport(int x, int y, unsigned int new_width, unsigned int new_height)
	{
		if (!intern_window) return;
		
		glViewport(x, y, new_width, new_height);
		
	}

	void Window::Close()
	{
		if (!intern_window) return;

		glfwSetWindowShouldClose(intern_window, true);
		
	}

	void Window::SetFullscreen(bool state)
	{
		if (!intern_window) return;
		
		if (state)
		{
			glfwSetWindowMonitor(intern_window, currentMonitor, pos_x, pos_y, i_window_dimensions[0], i_window_dimensions[1], current_refresh_rate);
		}
		else
		{
			glfwSetWindowMonitor(intern_window, NULL, pos_x, pos_y, i_window_dimensions[0], i_window_dimensions[1], current_refresh_rate);
		}
			
		
	}

	void Window::SetFullscreen(bool state, GLFWmonitor* monitor)
	{
		if (intern_window) return;
		
		if (state)
		{
			glfwSetWindowMonitor(intern_window, monitor, pos_x, pos_y, i_window_dimensions[0], i_window_dimensions[1], current_refresh_rate);
			currentMonitor = glfwGetWindowMonitor(intern_window);
		}
		else
		{
			glfwSetWindowMonitor(intern_window, NULL, pos_x, pos_y, i_window_dimensions[0], i_window_dimensions[1], current_refresh_rate);
		}

		
	}

	void Window::Maximize(bool update_viewport)
	{
		if (!intern_window) return;
		
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

	void Window::Minimize()
	{
		if (!intern_window) return;
		
		glfwIconifyWindow(intern_window);
		
	}

	void Window::SetIcon(const std::string path)
	{
		if (!intern_window) return;

		windowIcon[0].pixels = stbi_load(path.c_str(), &windowIcon[0].width, &windowIcon[0].height, &icon_channels, 0);
		
		glfwSetWindowIcon(intern_window, 1, windowIcon);
		
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
		if (!intern_window) return;

		i_window_dimensions[0] = new_width;
		i_window_dimensions[1] = new_height;

		if (currentMonitor)
		{
			glfwSetWindowMonitor(intern_window, currentMonitor, pos_x, pos_y, i_window_dimensions[0], i_window_dimensions[1], current_refresh_rate);
		}
		
	}

	std::array<int, 2>& Window::GetSizeRef()
	{
		return i_window_dimensions;
	}

	std::array<int, 2> Window::GetSize()
	{
		return i_window_dimensions;
	}

	void Window::SetPosition(int x, int y)
	{
		if (!intern_window) return;

		pos_x = x;
		pos_y = y;

		if (currentMonitor)
		{
			glfwSetWindowMonitor(intern_window, currentMonitor, pos_x, pos_y, i_window_dimensions[0], i_window_dimensions[1], current_refresh_rate);
		}
		
	}

	/*void Window::SetFPSLimit(unsigned int limit)
	{
		FPSLimit = limit;

		TargetFrameTime = std::round(1.0f / FPSLimit * 100000) / 100000;
	}*/

	void Window::SetTitle( const std::string title)
	{
		if (intern_window)
		{
			glfwSetWindowTitle(intern_window, title.c_str());
		}
	}

	bool Window::GetInput (Key key, State state) 
	{ 
		return GetInput(static_cast<int>(key), static_cast<int>(state)); 
	}
	bool Window::GetInput (int key, int state)
	{
		if (!intern_window)
		{
			return false;
		}
		
		return glfwGetKey(intern_window, key) == state;
	}
	bool Window::isKeyPressed (Key key) 
	{ 
		return isKeyPressed(static_cast<int>(key)); 
	}
	bool Window::isKeyPressed (int key)
	{
		if (!intern_window)
		{
			return false;
		}

		return glfwGetKey(intern_window, key) == GLFW_PRESS;
	}
	bool Window::isKeyReleased (Key key) 
	{ 
		return isKeyReleased(static_cast<int>(key)); 
	}
	bool Window::isKeyReleased (int key)
	{
		if (!intern_window)
		{
			return false;
		}

		return glfwGetKey(intern_window, key) == GLFW_RELEASE;
	}
	bool Window::isMousePressed (Key key) 
	{ 
		return isMousePressed(static_cast<int>(key)); 
	}
	bool Window::isMousePressed (int key)
	{
		if (!intern_window)
		{
			return false;
		}

		return glfwGetMouseButton (intern_window, key) == GLFW_PRESS;
	}
	bool Window::isMouseReleased (Key key) 
	{ 
		return isMousePressed(static_cast<int>(key)); 
	}
	bool Window::isMouseReleased (int key)
	{
		if (!intern_window)
		{
			return false;
		}

		return glfwGetMouseButton (intern_window, key) == GLFW_RELEASE;
	}

	std::array<double, 2> Window::GetMousePosition()
	{
		std::array<double, 2> pos;

		if (intern_window)
		{
			glfwGetCursorPos(intern_window, &pos[0], &pos[1]);
			
			return { pos };
		}

		messages::PrintMessage("Can not get the mouse Position", "Window.cpp/std::array<double, 2> zap::Window::GetMousePosition()", MessageTypes::error);

		return { 0,0 };
	}

	void Window::HideCursor(bool state)
	{
		if (!intern_window) return;
		
		if (state)
		{
			glfwSetInputMode(intern_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else
		{
			glfwSetInputMode(intern_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		
	}

	void Window::SetVSync(bool state)
	{
		if (intern_window) glfwSwapInterval(state);
		
	}

	
	void Window::SetCursorinCameraMode(bool state)
	{
		if (!intern_window) return;
		
		if (state)
		{
			glfwSetInputMode(intern_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (glfwRawMouseMotionSupported())
			{
				glfwSetInputMode(intern_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			}
		}
		else
		{
			glfwSetInputMode(intern_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		
	}

	// Please let this in here
	// NOTE: It is ok here, because it is tied to Window (glfw) and not to rendering (gl)
	// TODO: This function causes huge performance issues with heavy visual impact. Fix it later.
	void Window::InternSwapBuffers()
	{
		if (intern_window)
		{
			glfwSwapBuffers(intern_window);

			float CurrentTime = glfwGetTime();

			float Frametime = CurrentTime - LastTime;
			// float wait_time = (TargetFrameTime - Frametime) * 1000;

			LastTime = glfwGetTime();

			// Now use sleep operation with wait time

			current_FPS = 1.0f / Frametime;
			current_Frametime = Frametime;



			//FrametimeBuffer = 0.0f;

			//std::cout << TargetFrameTime << std::endl;
			
		}

	}

	//

	void Window::Update()
	{
		if (intern_window)
		{
			glfwPollEvents();                   // Should be called everytime 

			glfwGetWindowSize(intern_window, &i_window_dimensions[0], &i_window_dimensions[1]);

			currentMonitor = glfwGetWindowMonitor(intern_window);
		}
	}

	void Window::Draw()
	{
		InternSwapBuffers(); // Only executes the draw call if the target frame time is passed
		glClear(GL_COLOR_BUFFER_BIT);
	}

}




