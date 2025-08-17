#pragma once

#ifndef EASYGL_H
#define EASYGL_H

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

	void Init();
	void InitGlad();
	void Delete();

	/*****************************************************************************/

}

#endif
