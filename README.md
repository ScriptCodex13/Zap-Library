<img width="300" height="300" alt="EasyGLLogo" src="https://github.com/user-attachments/assets/530644b3-04f8-4606-9e3b-4001fb0a0fe6" />

# What is EasyGL?

EasyGL is a OpenGL abstraction/library for one of my game engine projects. I thought it could be useful for others because the APIs code is shorter than raw OpenGL code but still lets you control the important parts of the EasyGL code. The API can be used by anyone who is familiar with C++ and has expierience with GLSL. But for everyone who know more OpenGL can also use OpenGL in combination with EasyGL. If you want to know more about GLSL take a look at the awesome Website [LearnOpenGL](https://learnopengl.com/Getting-started/Shaders).

# How to use 

## Build the project

### Build for Visual Studio using [Premake](https://github.com/premake/premake-core)

**Step 1:**

## Use the compiled Library

Go to the [Releases](https://github.com/ScriptCodex13/EasyGL/releases) of this Repository and choose your preferred version.

## Start with your first code

```c++

#include "EasyGL.h"

int main()
{
	ez::Init();

	ez::Window window(1290, 720, "Hello Window");

	ez::InitGlad();

	while (window.Open())
	{
		window.ClearBackground(ez::BackgroundColors::ORANGE);

		if (window.GetInput(ez::Key::ESC, ez::State::EZ_PRESSED))
		{
			window.Close();
		}

		window.UpdateViewport();
		window.Update();
		window.Draw();

	}

	ez::Delete();
}


```

```c++
ez::Init();
ez::Delete();
```
You use these functions to manage the APIs OpenGL resources. Every call from EasyGL must be called between these two functions.

```c++
ez::Window window(1290, 720, "Hello Window");
```
We create a new Window class here. We specify the width and height and the Title of the window.

**Multiple windows:**

Only one window can exist per thread, so if you want to create more than one, you have to create and handle them in different threads.

```c++
ez::InitGlad();
```
Glad is a library that handles the OpenGL function locations. All you need to know is that this function must be called AFTER you create a window class instance.

```c++
while (window.Open())
	{
		window.ClearBackground(ez::BackgroundColors::ORANGE);

		if (window.GetInput(ez::Key::ESC, ez::State::EZ_PRESSED))
		{
			window.Close();
		}

		window.UpdateViewport();
		window.Update();
		window.Draw();

	}
```
Now a while loop is created which checks if the window is open. In this loop the window is painted orange with the ClearBackground function. 

```c++
window.UpdateViewport();
```
In quite a few cases your window gets resized. So we call UpdateViewport to match the new window size. 

```c++
window.Update();
window.Draw();
```

These functions are one of the most important. Update() manages all the window events like Key Inputs. window.Draw() draws everything on the window. So every write operation from meshes etc. need to be called before. Otherwise these wont work.

# Wiki

You can visit the wiki here <- Link of the wiki

# Please Note 

EasyGL isnt fully finished yet. There may still be some Issues. If you want to change something or you have got a question, feel free to write a Pull Request or an Issue. Help will be greatly appriciated ! 
