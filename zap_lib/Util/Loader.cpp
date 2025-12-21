#include "Loader.h"

#if defined(GLFW_PLATFORM_ENABLED) || defined (_glfw3_h_) || defined(_WINDOWS_)
#error GLFW or WINAPI platform dependency introduced in Render
#endif

namespace zap
{
	std::string LoadShaderSourcefromFile(const std::string filepath)
	{
		std::ifstream file(filepath);
		std::stringstream buffer;

		if (file.is_open())
		{
			buffer << file.rdbuf();
			
			return buffer.str();
		}
		
		messages::PrintMessage("Failed to load shader source from path: " + filepath, "Loader.cpp/ std::string zap::LoadShaderSourcefromFile(...)", MessageTypes::error);

		return "";
	}
}