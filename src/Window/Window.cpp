#include "Window.h"
//do not produce code if platform is disabled
#ifdef GLFW_PLATFORM_ENABLED

#include "stb_image.h"
#include "../Util/Util.h"

namespace zap
{
	
	/***********************************************************************************/

	//Intern callback functions
	//A null window can't invoke callbacks
	static void internwindowCloseCallback(GLFWwindow* window)
	{
		glfwSetWindowShouldClose(window, true);
	}

	/***********************************************************************************/

	/***********************************************/

	//Viewport function
	//A null window can't invoke callbacks
	static void i_FramebuffersizeCallback(GLFWwindow* window ,int size_x, int size_y)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}
	static void i_nFramebuffersizeCallback(GLFWwindow* window, int size_x, int size_y) {}

	void MouseClickCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Window* pWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		pWindow->InvokeClickHandlers(button, action, mods);
		//if (action == GLFW_PRESS)
		//{
		//	switch (button)
		//	{
		//	case GLFW_MOUSE_BUTTON_LEFT:
		//		std::cout << "lbutton click" << std::endl;
		//		break;
		//	case GLFW_MOUSE_BUTTON_RIGHT:
		//		std::cout << "rbutton click" << std::endl;
		//		break;
		//	case GLFW_MOUSE_BUTTON_MIDDLE:
		//		std::cout << "mbutton click" << std::endl;
		//		break;
		//	}
		//}
	}
	/***********************************************/

	Window::Window(
		int size_x, int size_y, const std::string Title,
		GLFWmonitor* monitor, GLFWwindow* other_window)
		: buttonEventProvider(this), buttonContainer(this)
	{

		intern_window = glfwCreateWindow(size_x, size_y, Title.c_str(), monitor, other_window);

		glfwSetWindowUserPointer(intern_window, this);
		if (!intern_window)
		{
			messages::PrintMessage("Failed to create window", "Window.cpp/Window::Window(...)", MessageTypes::fatal_error);
			ZAP_INTERRUPT_FATAL_ERROR;
		}

		if (monitor == NULL)
			currentMonitor = glfwGetPrimaryMonitor();
		else
			currentMonitor = glfwGetWindowMonitor(intern_window); // The same for the other constructor

		glfwMakeContextCurrent(intern_window);
		glfwGetWindowSize(intern_window, &i_window_dimensions[0], &i_window_dimensions[1]);
		glfwGetWindowPos(intern_window, &pos_x, &pos_y);

		TargetFrameTime = std::round(1.0f / FPSLimit * 100000) / 100000;

		i_original_size = { size_x, size_y };
		glfwSetMouseButtonCallback(intern_window, MouseClickCallback);
	}
	Window::Window(
		GLFWwindow* extern_window, 
		int base_size_x, int base_size_y) : 
		buttonEventProvider(this), buttonContainer(this)
	{
		intern_window = extern_window;

		if (!intern_window)
		{
			messages::PrintMessage("Failed to get extern window", "Window.cpp/zap::Window::Window(...)", MessageTypes::fatal_error);
			ZAP_INTERRUPT_FATAL_ERROR;
		}
		glfwSetWindowUserPointer(intern_window, this);

		if (glfwGetWindowMonitor(intern_window) == NULL)
			currentMonitor = glfwGetPrimaryMonitor();
		else 
			currentMonitor = glfwGetWindowMonitor(intern_window);

		glfwMakeContextCurrent(intern_window);
		glfwGetWindowSize(intern_window, &i_window_dimensions[0], &i_window_dimensions[1]);
		glfwGetWindowPos(intern_window, &pos_x, &pos_y);

		i_original_size = { base_size_x, base_size_y };
		glfwSetMouseButtonCallback(intern_window, MouseClickCallback);
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
		if (!glfwWindowShouldClose(intern_window))
		{
			return true;
		}


		return false;
	}

	void Window::UpdateViewport(bool state)
	{
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
		glViewport(x, y, new_width, new_height);

	}

	void Window::Close()
	{
		glfwSetWindowShouldClose(intern_window, true);
	}

	void Window::SetFullscreen(bool state)
	{
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
		glfwIconifyWindow(intern_window);
	}

	void Window::SetIcon(const std::string path)
	{
		windowIcon[0].pixels = stbi_load(path.c_str(), &windowIcon[0].width, &windowIcon[0].height, &icon_channels, 0);


		glfwSetWindowIcon(intern_window, 1, windowIcon);


	}

	float Window::GetFPS() const
	{
		return current_FPS;
	}

	float Window::GetDelta() const
	{
		return current_Frametime;
	}

	void Window::SetSize(unsigned int new_width, unsigned int new_height)
	{
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

	std::array<int, 2> Window::GetSize() const
	{
		return i_window_dimensions;
	}

	void Window::SetPosition(int x, int y)
	{
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


		glfwSetWindowTitle(intern_window, title.c_str());


	}

	bool Window::GetInput (Key key, State state)

	{

		return GetInput(static_cast<int>(key), static_cast<int>(state));

	}
	bool Window::GetInput (int key, int state)
	{
		if (util::in(key, 0, 1, 2)) // Checks if the key is a mouse button and uses a different function
		{
			return glfwGetMouseButton(intern_window, key) == state;
		}

		return glfwGetKey(intern_window, key) == state;
	}
	bool Window::isKeyPressed (Key key)

	{

		return isKeyPressed(static_cast<int>(key));

	}
	bool Window::isKeyPressed (int key)
	{
		return glfwGetKey(intern_window, key) == GLFW_PRESS;
	}
	bool Window::isKeyReleased (Key key)

	{

		return isKeyReleased(static_cast<int>(key));

	}
	bool Window::isKeyReleased (int key)
	{
		return glfwGetKey(intern_window, key) == GLFW_RELEASE;
	}
	bool Window::isMousePressed (Key key)

	{

		return isMousePressed(static_cast<int>(key));

	}
	bool Window::isMousePressed (int key)
	{
		return glfwGetMouseButton (intern_window, key) == GLFW_PRESS;
	}
	bool Window::isMouseReleased (Key key)

	{

		return isMousePressed(static_cast<int>(key));

	}
	bool Window::isMouseReleased (int key)
	{
		return glfwGetMouseButton (intern_window, key) == GLFW_RELEASE;
	}

	std::array<double, 2> Window::GetMousePosition()
	{
		std::array<double, 2> pos{};



		glfwGetCursorPos(intern_window, &pos[0], &pos[1]);


		return { pos };
	}
	std::array<double, 2> Window::GetMouseGlPosition()
	{
		std::array<double, 2> pos = GetMousePosition();
		return zap::util::pixel_to_gl_coords(GetSize(), pos);

	}

	void Window::HideCursor(bool state)
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

	void Window::SetVSync(bool state)
	{
		glfwSwapInterval(state);
	}



	void Window::SetCursorinCameraMode(bool state)
	{
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

	void Window::InternSwapBuffers()
	{


		glfwSwapBuffers(intern_window);

		float CurrentTime = glfwGetTime();

		float Frametime = CurrentTime - LastTime;


		LastTime = glfwGetTime();

		// TODO: Something unfinished goes here on.
		current_FPS = 1.0f / Frametime;
		current_Frametime = Frametime;



	}

	//

	std::array<float, 2> Window::GetScaleDifference()
	{
		return { (float)i_original_size[0] / GetSize()[0], (float)i_original_size[1] / GetSize()[1] };
	}

	std::array<int, 2> Window::GetOriginalSize()
	{
		return  i_original_size ;
	}

	void Window::Update()
	{

		glfwPollEvents();                   // Should be called everytime


		glfwGetWindowSize(intern_window, &i_window_dimensions[0], &i_window_dimensions[1]);

		currentMonitor = glfwGetWindowMonitor(intern_window);
	}

	void Window::Draw()
	{
		InternSwapBuffers(); // Only executes the draw call if the target frame time is passed
	}


}




#endif