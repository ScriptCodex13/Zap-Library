#pragma once

#ifndef RESOURCES_H
#define RESOURCES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace zap
{
	/*****************************************************************************/
	
	//APIs Resources

	void Init();       // Initialize GLFW 
	void InitGlad();   // Initialize glad
	void UseDepthTest();
	void Delete();     // Delete GLFW resources

	/*****************************************************************************/

}

#endif
