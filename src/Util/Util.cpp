#include "Util.h"

#ifdef GLFW_PLATFORM_ENABLED
#error GLFW platform dependency introduced in Render
#endif

#include <fstream>
#include <sstream>
#include <filesystem>

namespace zap
{
	namespace util
	{
		std::string GetTextFileContent(const char* shaderSourceFilePath)
		{
			ZAP_REQUIRE_ALL(std::filesystem::exists(shaderSourceFilePath) && "File path can not be found: " && shaderSourceFilePath);
			using std::cout;
			using std::endl;
			using std::string;
			using std::ifstream;
			using std::stringstream;

			ifstream shaderFile;
			shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
			try
			{
				shaderFile.open(shaderSourceFilePath);
				stringstream shaderStream;
				shaderStream << shaderFile.rdbuf();
				shaderFile.close();
				std::string shaderCode = shaderStream.str();
				return shaderCode;
			}
			catch (ifstream::failure e)
			{
				cout << "Shader file not successfully read: " << e.what() << endl << "path: " << shaderSourceFilePath << endl;
				throw e;
			}

			return "";
		}
	};
}