<img width="300" height="300" alt="EasyGLLogo" src="https://github.com/user-attachments/assets/530644b3-04f8-4606-9e3b-4001fb0a0fe6" />

# What is EasyGL?

EasyGL is a OpenGL abstraction/library for one of my game engine projects. I thought it could be useful for others because the APIs code is shorter than raw OpenGL code but still lets you control the important parts of the EasyGL code. You dont have to know anything about OpenGL. The API can be used by anyone who is familiar with C++.

# How to use 

## Build the project

### Build for Visual Studio using Premake <- Link here 

**Step 1:**

## Use the Library

**Step 1:**

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

# Please Note 

EasyGL isnt fully finished yet. There may still be some Issues. If you want to change something or you have got a question, feel free to write a Pull Request or an Issue. Help will be greatly appriciated ! 
