#include "GlfwUtil.h"

namespace zap
{
	namespace util
	{
		//What is exactly the purpose of burying glfwGetPrimaryMonitor into GetMonitor?
		GLFWmonitor* GetMonitor() { return glfwGetPrimaryMonitor(); }
	};
}