#pragma once

#ifndef MESH_H
#define MESH_H

#include "Message.h"
#include "Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>


namespace zap
{
	enum class BufferAccessModes : GLenum
	{
		LOW_ACCESS_STATIC   = GL_STREAM_DRAW,
		HIGH_ACESS_STATIC   = GL_STATIC_DRAW,
		HIGH_ACESS_DYNAMIC  = GL_DYNAMIC_DRAW
	};

	struct AttributeConfig
	{
		AttributeConfig(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos);

		void* getPos() { return  reinterpret_cast<void*>( i_start_pos * sizeof(float)); }
		void  vertexAttribPointer();

		int             i_shader_location;
		int             i_value_ct;
		GLenum          i_type = GL_FLOAT;
		GLboolean       i_normalized = GL_FALSE;
		unsigned int    i_data_stride;
		unsigned int    i_start_pos;
	};
};



#endif

