#pragma once

//gives the way to be platform neutral
#ifndef GLFW_PLATFORM_DISABLE
#define GLFW_PLATFORM_ENABLED
#endif

//check there is GLFW platform added
#ifdef GLFW_PLATFORM_ENABLED

#ifndef WINDOW_H
#define WINDOW_H


#include "../Window/Resources.h"

#include "../Window/Input.h"
#include "../Util/Message.h"
#include "../GUI/GUIInterfaces.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <chrono>
#include <thread>
#include <set>

namespace zap
{

	/*****************************************************************************/

	//Window

	class Window
	{
	public:
		Window(int scale_x, int scale_y, const std::string Title, GLFWmonitor* monitor = NULL, GLFWwindow* other_window = NULL);
		Window(GLFWwindow* extern_window, int base_resolution_x = 1920, int base_resolution = 1080); // You dont have to handle the window anymore it is now handled by Zap
		~Window();

	public:
		GLFWwindow* GetNativeWindow();                                      // Returns the intern GLFWwindow instance 
		bool Open();                                                        // Checks if the window should be open
		void UpdateViewport(bool state = true);                             // Enable viewport changes if the window resizes
		void SetViewport(int x, int y, unsigned int new_width, unsigned int new_height);  // Set the new viewport of the window
		void Close();                                                       // Closes the window 
		void SetFullscreen(bool state);                                     // Sets the window to fullacreen or to windowed based on the state
		void SetFullscreen(bool state, GLFWmonitor* monitor);               // Changes the fullscreen mode. If sate = true. The provided GLFWmonitor is going to be used 
		void Maximize(bool update_viewport = true);                         // Maximizes the window
		void Minimize();                                                    // Minimizes the window
		float GetFPS() const;                                               // Returns the current FPS of the window as a float. ! Cannot be replaced with manual FPS checking if .Draw() is used !
		float GetDelta() const;                                             // Returns the current Frametime as Delta
		void SetSize(unsigned int new_width, unsigned int new_height);      // Set the new size of the window 
		std::array<int, 2>& GetSizeRef();
		std::array<int, 2> GetSize() const;
		void SetPosition(int x, int y);                                     // Set the Position of the window on the screen
		// void SetFPSLimit(unsigned int limit);                               // Limit the max amount of frame updates per second
		void SetTitle(const std::string title);                             // Set the title of the window 
		bool GetInput(Key key, State state);                                // Checks the state of a key
		bool GetInput(int key, int   state);
		bool isKeyPressed(Key key);
		bool isKeyPressed(int key);
		bool isKeyReleased(Key key);
		bool isKeyReleased(int key);
		bool isMousePressed(Key key);
		bool isMousePressed(int key);
		bool isMouseReleased(Key key);
		bool isMouseReleased(int key);
		void SetIcon(const std::string path);                               // Loads the new window icon at the specified path
		std::array<double, 2> GetMousePosition();                           // Returns the cursor position in pixels relative to the top left corner of the window 
		std::array<double, 2> GetMouseGlPosition();                         // Returns the cursor position in pixels relative to the top left corner of the window 
		void HideCursor(bool state);                                        // Hide or unhide the cursor if the window is entered
		void SetVSync(bool state);
		void SetCursorinCameraMode(bool state);
		std::array<float, 2> GetScaleDifference();					        // 
		std::array<int, 2> GetOriginalSize();					            // Get the original window resolution so you can scale elements properly even after resizing or upscaling
		bool IsClosing();
		void Update();
		void Draw();

	private:
		GLFWwindow* intern_window = nullptr;
		GLFWmonitor* currentMonitor = nullptr;
		std::array<int, 2> i_window_dimensions = { 0,0 };
		int pos_x  = 0;
		int pos_y  = 0;
		int current_refresh_rate = 1000000000;

		std::array<int, 2> i_original_size;

	private: //Icon
		GLFWimage windowIcon[1];
		int icon_channels;

	private: //Time
		float current_FPS = 0;
		float current_Frametime = 0;

		unsigned int FPSLimit = 1000000;
		float TargetFrameTime;
		//float FrametimeBuffer = 0;						// Adds the Frametimes together until the TargetTime is reached. 

		double LastTime;

	private:
		void InternSwapBuffers(); // Better for managing the FPSLimit
	public:

		class ButtonEventProvider : public IUIButtonEventProvider
		{
			Window* windowPtr = nullptr;
			std::set<IUIButtonEventListener*> handlers;
			struct { std::array<double, 2> oldGlPos, newGlPos; } buttonListenerPos{ {1000., 1000.}, {1100., 1100.} };
		public:
			ButtonEventProvider(Window* _windowPtr);
			void InvokeDefaultHandler(IUIButtonEventListener* handler);

			//implement interface methods
			virtual void AddButtonEventHandler(IUIButtonEventListener* handler);
			//These must be always called
			virtual void InvokeDefaultHandlers();
			//These are called on mouse click events
			virtual void InvokeLMouseClickHandlers();
			virtual void InvokeRMouseClickHandlers();
			virtual void InvokeMMouseClickHandlers();

		};
		ButtonEventProvider buttonEventProvider;
		class ButtonContainer: public IUIButtonContainer
		{
			Window* window;
		public:
			ButtonContainer(Window* pwindow);
			virtual std::array<int, 2> GetClientSize();
			virtual std::array<int, 2> GetClientOriginalSize();
		};

		ButtonContainer buttonContainer;
		IUIButtonContainer* getButtonContainer();
		void InvokeHandlers();
		void InvokeLMouseClickHandlers();
		void InvokeRMouseClickHandlers();
		void InvokeMMouseClickHandlers();
		void AddButtonEventHandler(IUIButtonEventListener* handler);
		void InvokeClickHandlers(int button, int action, int mods);
	};


}

#endif //WINDOW_H

#endif //GLFW_PLATFORM_ENABLED
