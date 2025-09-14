#include "AttributeConfig.h"

#include <filesystem>

namespace zap
{
	AttributeConfig::AttributeConfig(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos) :
		i_shader_location (shader_location),
		i_value_ct        (value_ct),
		i_data_stride     (data_stride),
		i_start_pos       (start_pos)
	{
		// Currently empty
	}

	void AttributeConfig::vertexAttribPointer() 
	{
		glVertexAttribPointer (i_shader_location, i_value_ct, i_type, i_normalized, i_data_stride * sizeof(float), getPos());
		glEnableVertexAttribArray(i_shader_location);
	}


}


