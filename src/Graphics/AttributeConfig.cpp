#include "AttributeConfig.h"

#if defined(GLFW_PLATFORM_ENABLED) || defined (_glfw3_h_) || defined(_WINDOWS_)
#error GLFW or WINAPI platform dependency introduced in Render
#endif


namespace zap
{
	AttributeConfig::AttributeConfig(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos, bool instanced) :
		i_shader_location (shader_location),
		i_value_ct        (value_ct),
		i_data_stride     (data_stride),
		i_start_pos       (start_pos),
		i_instanced		  (instanced)
	{
		// Currently empty
	}

	void AttributeConfig::vertexAttribPointer()
	{
		glVertexAttribPointer (i_shader_location, i_value_ct, i_type, i_normalized, i_data_stride * sizeof(float), getPos());
		glEnableVertexAttribArray(i_shader_location);

		if(i_instanced)
		{
			glVertexAttribDivisor(i_shader_location, 1);
		}
	}


}


