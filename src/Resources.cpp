#include "Resources.h"
#include "Message.h"
<<<<<<< HEAD
=======
#include "Util.h"
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844

namespace zap
{
	void Init()
	{
		/******************************************************************************/
		if (!glfwInit())
		{
			messages::PrintMessage("Failed to initialize GLFW", "EasyGL.cpp/void Init()", MessageTypes::fatal_error);
<<<<<<< HEAD
=======
			ZAP_INTERRUPT_FATAL_ERROR;
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
			#ifdef TARGET_OS_MAC

				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

			#elif TARGET_OS_IPHONE
				#error Zap is not for iOS !
<<<<<<< HEAD
=======
				ZAP_INTERRUPT_FATAL_ERROR;
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
			#endif
		#endif

		#ifndef GLFW_INIT
			#define GLFW_INIT
		#endif

		/******************************************************************************/
<<<<<<< HEAD
=======

		messages::PrintMessage("Using OpenGL Version 3.3", "", MessageTypes::api_core_info, false);
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
	}

	void InitGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			messages::PrintMessage("Failed to initialize GLAD", "EasyGL.cpp/void InitGlad()", MessageTypes::fatal_error);
<<<<<<< HEAD
		}
		else
		{
			glEnable(GL_DEPTH_TEST);			// Enable Depth if glad can be used 
=======
			ZAP_INTERRUPT_FATAL_ERROR;
		}
		else
		{
			messages::PrintMessage("glad initialized sucessfully", "", MessageTypes::api_core_info, false);
>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
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

<<<<<<< HEAD
=======
		messages::PrintMessage("Deleted reseources", "", MessageTypes::api_core_info, false);

>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
		/******************************************************************************/
	}

}
<<<<<<< HEAD
=======


>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
