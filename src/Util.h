#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace zap
{
	namespace util 
	{
		GLFWmonitor* GetMonitor(); // Gets the main monitor
	};
}


#endif
