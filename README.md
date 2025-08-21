
<img width="300" height="300" alt="ZapLib Logo" src="https://github.com/user-attachments/assets/d9f62e46-1d9d-4e68-a9b6-8cb8943cf495" />

# What is Zap?

Zap is an OpenGL abstraction/ graphics library for one of my game engine projects. I thought it could be useful for others because the APIs code is shorter than raw OpenGL code but still lets you control the important parts of the Zap code. The API can be used by anyone who is familiar with C++ and has expierience with GLSL. But for everyone who know more OpenGL can also use OpenGL code in combination with Zap. If you want to know more about GLSL take a look at the awesome Website [LearnOpenGL](https://learnopengl.com/Getting-started/Shaders).

# What is it able to do ?

Zap can handle Conntroller and Keyboard input, Mouse input and events, handle windows and modify them and it can also render 2DMeshes.

# How to use 

## Build the project

### Build for Visual Studio using [Premake](https://github.com/premake/premake-core)

Windows:
    Requirements: Visual Studio 2022
	1. clone this repository

  	Visual Studio: Run the Windows_GenerateVS.bat in the Build_Premake folder


  
 	
	

## Use the compiled Library

Coming soon

## Start with your first code

```c++

#include "Zap.h"

int main()
{
	zap::Init();

	zap::Window window(1290, 720, "Hello Window");

	zap::InitGlad();

	while (window.Open())
	{
		window.ClearBackground(zap::BackgroundColors::ORANGE);

		if (window.GetInput(zap::Key::ESC, zap::State::EZ_PRESSED))
		{
			window.Close();
		}

		window.UpdateViewport();
		window.Update();
		window.Draw();

	}

	zap::Delete();
}


```

```c++
zap::Init();
zap::Delete();
```
You use these functions to manage the APIs OpenGL resources. Every call from Zap must be called between these two functions.

```c++
zap::Window window(1290, 720, "Hello Window");
```
We create a new Window class here. We specify the width and height and the Title of the window.

**Multiple windows:**

Only one window can exist per thread, so if you want to create more than one, you have to create and handle them in different threads.

```c++
zap::InitGlad();
```
Glad is a library that handles the OpenGL function locations. All you need to know is that this function must be called AFTER you create a window class instance.

```c++
while (window.Open())
{
	window.ClearBackground(zap::BackgroundColors::ORANGE);

	if (window.GetInput(zap::Key::ESC, zap::State::EZ_PRESSED))
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

You can visit the wiki [here](https://github.com/ScriptCodex13/Zap-Library/wiki)
# Used Software

[stb_image](https://github.com/nothings/stb) to load images,
[GLFW](https://github.com/glfw/glfw) to manage windows,
[GLAD](https://github.com/Dav1dde/glad) to load OpenGL functions
[Premake](https://github.com/premake/premake-core) to build the project,
glm for the math

# Please Note 

Zap isnt finished yet. To be precise, it is a fresh project. There are some Issues and the API is missing a lot of planned features. But if you want to change something or you have got a question, feel free to write a Pull Request or an Issue. Help will be greatly appriciated ! 
