<img width="300" height="300" alt="ZapLib Logo" src="https://github.com/user-attachments/assets/d9f62e46-1d9d-4e68-a9b6-8cb8943cf495" />

# What is Zap?

Zap is an API designed for creating graphics applications such as games. It provides simple function calls while imposing almost no limitations compared to raw OpenGL.

The API can be used by anyone familiar with C++ who has experience with GLSL and basic OpenGL. Developers who know more OpenGL can also use additional OpenGL code in combination with Zap.

# What can it do?

<img width="961" height="561" alt="Result" src="https://github.com/user-attachments/assets/4da45106-46f1-4dd9-bc71-27dedd4ffa13" />

This is what Zap is currently able to do:

- Controller and keyboard input
- Mouse input and events
- Create and manage windows
- Render meshes
- Draw text and buttons

With a little more effort, you can also add lighting quickly.

# So why Zap?

Features:
- Allows direct control over OpenGL calls
- Easily extendable and modifiable
- High performance
- Mix Zap with raw OpenGL code
- Optional additions (audio, GUI, etc.)
- Good for learning OpenGL (replace parts of your code with raw OpenGL)
- Faster development than using raw OpenGL

# How to use

## Build the project

### Build for Visual Studio using [Premake](https://github.com/premake/premake-core)

Windows:

- Requirements: Visual Studio 2022

Clone this repository and run Windows_GenerateVS.bat in the Build_Premake/Windows folder.

For more detailed information, see the [wiki](https://github.com/ScriptCodex13/Zap-Library/wiki/Project-setup-with-Windows-and-Visual-Studio-2022).

## Use the compiled library

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

Use these functions to manage the API's OpenGL resources. Every Zap call must be made between these two functions.

```c++
zap::Window window(1280, 720, "Hello Window");
```

This creates a Window instance. The parameters specify the width, height, and title of the window.

**Multiple windows:**

Only one window can exist per thread. To create multiple windows, create and manage them in different threads.

```c++
zap::InitGlad();
```

GLAD is a loader that retrieves OpenGL function pointers. It must be initialized after creating a window.

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

This while loop runs while the window is open. Inside the loop, ClearBackground fills the window with an orange color.

```c++
window.UpdateViewport();
```

If the window is resized, call UpdateViewport to update the viewport to the new window size.

```c++
window.Update();
window.Draw();
```

These functions are among the most important. Update() processes window events such as keyboard and mouse input. Draw() renders the scene. All rendering and drawing operations should occur before calling Draw().

# Wiki

See the wiki: https://github.com/ScriptCodex13/Zap-Library/wiki

# Licensing

You can use this library without including the license file in your binary distribution; however, you must comply with the licenses of its dependencies. Remember to include those licenses in your project.

# Used Software

- [stb_image](https://github.com/nothings/stb) to load images
- [GLFW](https://github.com/glfw/glfw) to manage windows
- [GLAD](https://github.com/Dav1dde/glad) to load OpenGL functions
- [Premake](https://github.com/premake/premake-core) to build the project
- [glm](https://github.com/g-truc/glm) for math
- [FreeType](https://freetype.org/) (under the FTL license) for font loading
- [miniaudio](https://github.com/mackron/miniaudio) for audio
- Provided fonts come from Google Fonts: https://fonts.google.com/

# Questions?

Join our [Discord server](https://discord.gg/hr2vjemJSV) to ask questions and chat with the developers.

# Contributing

We need more help to grow and improve this project. If you'd like to contribute or have questions, feel free to open a pull request or an issue. Any help is greatly appreciated.
