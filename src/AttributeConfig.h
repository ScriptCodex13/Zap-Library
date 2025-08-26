#pragma once

#ifndef ATTRIBUTE_CONFIG_H
#define ATTRIBUTE_CONFIG_H

#include "Message.h"
#include "Texture.h"

#include <glad/glad.h>

namespace zap
{
	struct AttributeConfig // Config for the vertex attributes and also executes
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


