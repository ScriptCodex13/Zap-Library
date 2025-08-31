#include "Resources.h"
#include "Message.h"

namespace zap
{
	void Init()
	{
		/******************************************************************************/
		if (!glfwInit())
		{
			messages::PrintMessage("Failed to initialize GLFW", "EasyGL.cpp/void Init()", MessageTypes::fatal_error);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
			#ifdef TARGET_OS_MAC

				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

			#elif TARGET_OS_IPHONE
				#error Zap is not for iOS !
			#endif
		#endif

		#ifndef GLFW_INIT
			#define GLFW_INIT
		#endif

		/******************************************************************************/
	}

	void InitGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			messages::PrintMessage("Failed to initialize GLAD", "EasyGL.cpp/void InitGlad()", MessageTypes::fatal_error);
		}
		else
		{
			glEnable(GL_DEPTH_TEST);			// Enable Depth if glad can be used 
		}
	}

	void Delete()
	{
		/******************************************************************************/

		//GLFW Delete

		glfwTerminate();

		#ifndef GLFW_DELETE
			#define GLFW_DELETE
		#endif

		/******************************************************************************/
	}

}
