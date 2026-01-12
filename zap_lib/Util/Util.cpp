#include "Util.h"

#if defined(GLFW_PLATFORM_ENABLED) || defined (_glfw3_h_) || defined(_WINDOWS_)
#error GLFW or WINAPI platform dependency introduced in Render
#endif

#include <fstream>
#include <sstream>
#include <filesystem>

namespace zap
{
	namespace util
	{
		std::string GetFileContent(const char* path)
		{
			ZAP_REQUIRE_ALL(std::filesystem::exists(path) && "File path can not be found: " && path);
			using std::cout;
			using std::endl;
			using std::string;
			using std::ifstream;
			using std::stringstream;

			ifstream file;
			file.exceptions(ifstream::failbit | ifstream::badbit);
			try
			{
				file.open(path);
				stringstream shaderStream;
				shaderStream << file.rdbuf();
				file.close();
				std::string shaderCode = shaderStream.str();
				return shaderCode;
			}
			catch (ifstream::failure e)
			{
				cout << "File not successfully read: " << e.what() << endl << "path: " << path << endl;
				throw e;
			}

			return "";
		}
	};
}