#include "Resources.h"
#include "Message.h"
#include "Util.h"

#include <cmath>

namespace zap
{
	void Init()
	{
		Init(3, 3);
	}

	void Init(unsigned int version_major, unsigned int version_minor)
	{
		/******************************************************************************/
		if (!glfwInit())
		{
			messages::PrintMessage("Failed to initialize GLFW", "EasyGL.cpp/void Init()", MessageTypes::fatal_error);
			ZAP_INTERRUPT_FATAL_ERROR;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
			#ifdef TARGET_OS_MAC

				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

			#elif TARGET_OS_IPHONE
				#error Zap is not for iOS !
				ZAP_INTERRUPT_FATAL_ERROR;
			#endif
		#endif

		#ifndef GLFW_INIT
			#define GLFW_INIT
		#endif

		glfwWindowHint(GLFW_SAMPLES, 4);

		/******************************************************************************/

		messages::PrintMessage("Using OpenGL Version " + std::to_string(version_major) + "." + std::to_string(version_minor), "", MessageTypes::api_core_info, false);
	}

	void InitGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			messages::PrintMessage("Failed to initialize GLAD", "EasyGL.cpp/void InitGlad()", MessageTypes::fatal_error);
			ZAP_INTERRUPT_FATAL_ERROR;
		}
		else 
		{
			messages::PrintMessage("glad initialized sucessfully", "", MessageTypes::api_core_info, false);
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

		messages::PrintMessage("Deleted reseources", "", MessageTypes::api_core_info, false);

		/******************************************************************************/
	}

}


