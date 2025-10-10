#include "GlfwUtil.h"

namespace zap
{
	namespace util
	{
		GLFWmonitor* GetMonitor() { return glfwGetPrimaryMonitor(); }
	};
}