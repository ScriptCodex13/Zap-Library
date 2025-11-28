#pragma once

//gives the way to be platform neutral
#ifndef GLFW_PLATFORM_DISABLE
#define GLFW_PLATFORM_ENABLED
#endif

//check there is GLFW platform added
#ifdef GLFW_PLATFORM_ENABLED

#ifndef RESOURCES_H
#define RESOURCES_H



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

#endif //GLFW_PLATFORM_ENABLED

#endif