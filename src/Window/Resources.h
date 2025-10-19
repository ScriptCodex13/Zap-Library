#pragma once

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

#endif
