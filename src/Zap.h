#pragma once

#ifndef ZAP_H
#define ZAP_H
//TODO: Is zap.h intendet to gather headers or to add functionality?
//      It must be either only the one, or only the second one, not both at the same time.
//      The "std" prefix is widely used for headers containing only #includes
//      Widespread examples of such names as StdAfx.h, stdlibc++.h
//      Consider adopting this practice
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Additional includes

#include "Message.h"
#include "Input.h"

//

// Library includes

#include "Window.h"
#include "Graphics.h"

//

namespace zap
{
	/*****************************************************************************/
	
	//APIs Resources

	void Init();       // Initialize GLFW 
	void InitGlad();   // Initialize glad
	void Delete();     // Delete GLFW resources

	/*****************************************************************************/

}

#endif


