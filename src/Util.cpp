#include "Util.h"

namespace zap
{
	namespace util
	{
		GLFWmonitor* GetMonitor()
		{
			return glfwGetPrimaryMonitor();
		}
	};
}