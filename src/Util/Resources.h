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
	void Init(unsigned int version_major, unsigned int version_minor);
	void InitGlad();   // Initialize glad
	void Delete();     // Delete GLFW resources

	/*****************************************************************************/

}

#endif
