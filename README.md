
<img width="300" height="300" alt="ZapLib Logo" src="https://github.com/user-attachments/assets/d9f62e46-1d9d-4e68-a9b6-8cb8943cf495" />

# What is Zap?

Zap is an API which is designed for creating graphics applications like games with easy calls and still control over the OpenGL processes behind the scenes. 


The API can be used by anyone who is familiar with C++ and has expierience with GLSL and some basics of OpenGL. But for everyone who know more OpenGL can also use more OpenGL code in combination with Zap. If you want to know more about GLSL take a look at the awesome Website [LearnOpenGL](https://learnopengl.com/Getting-started/Shaders).

# What is it able to do ?

<img width="961" height="561" alt="Result" src="https://github.com/user-attachments/assets/4da45106-46f1-4dd9-bc71-27dedd4ffa13" />


This is what Zap is currently able to do:

- Controller and Keyboard input, 
- Mouse input and events 
- handle windows and modify them 
- render Meshes.
- Draw Text and Buttons
 
And with a little bit more time you get also lighting done quickly.

# So why Zap ?

We are focused to build a very flexable library solution. The API isn't just a simple library abstraction. With the tools it provides you can shape it in the form you want. You can go more low level and use more OpenGL code with the core or go the high level route and abstract the core library more. So you could for example create a sprite class which let's you create a sprite with texture in under 5 lines of code for example. The core abstraction layer is not very big so you won't run in serious performance drawbacks. We also provide more abstracted classes build on the own core which helps piecing together a quick game. So if the abstraction isn't for you you can change it to match your coding style.
You can also contact us on Discord if you run into issues with the API.


# How to use 

## Build the project

### Build for Visual Studio using [Premake](https://github.com/premake/premake-core)

Windows:

    - Requirements: Visual Studio 2022

clone this repository and run the Windows_GenerateVS.bat in the Build_Premake/Windows folder

If you want more detailed information look at the [wiki](https://github.com/ScriptCodex13/Zap-Library/wiki/Project-setup-with-Windows-and-Visual-Studio-2022).


  
 	
	

## Use the compiled Library

Choose your [release](https://github.com/ScriptCodex13/Zap-Library/releases) and follow the provided instructions.

## Start with your first code

```c++

#include "Zap.h"

int main()
{
	zap::Init();

	zap::Window window(1280, 720, "Hello Window");

	zap::InitGlad();

    window.UpdateViewport(true);

	while (window.Open())
	{
		zap::ClearBackground(zap::BackgroundColors::ORANGE);

		if (window.GetInput(zap::Key::ESC, zap::State::PRESSED))
		{
			window.Close();
		}

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
zap::Window window(1280, 720, "Hello Window");
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
	zap::ClearBackground(zap::BackgroundColors::ORANGE);

	if (window.GetInput(zap::Key::ESC, zap::State::PRESSED))
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

# Licensing

You can use this library without carrying the license around. But not for our dependencies. Please remember to add the Licenses to your project.

# Used Software

[stb_image](https://github.com/nothings/stb) to load images,
[GLFW](https://github.com/glfw/glfw) to manage windows,
[GLAD](https://github.com/Dav1dde/glad) to load OpenGL functions,
[Premake](https://github.com/premake/premake-core) to build the project,
[glm](https://github.com/g-truc/glm) for the math,
[freetype](https://freetype.org/) (under the FTL license) for font loading,
[miniaudio](https://github.com/mackron/miniaudio) for audio,
[provided fonts](https://fonts.google.com/) come from google fonts

# Questions ?

Come to our [discord server](https://discord.gg/hr2vjemJSV) to ask questions and chat with the developers.

# Contributing

We need more help with this project to grow and improve. So if you want to change something or you have got a question, feel free to write a Pull Request or an Issue. Any help will be greatly appriciated ! Here is our [contributing page](https://github.com/ScriptCodex13/Zap-Library?tab=contributing-ov-file).
