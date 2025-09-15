#include "Loader.h"

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