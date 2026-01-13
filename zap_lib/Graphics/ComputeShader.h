#pragma once

#ifndef COMPUTE_SHADER_H
#define COMPUTE_SHADER_H

#include "../Window/Resources.h"
#include "../Util/UtilBind.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <array>

namespace zap
{
	class ComputeShader
	{
	public:
		ComputeShader();
		~ComputeShader();

		void SetSource(const std::string& source);
		void SetShaderFilePath(const std::string& path);

		void Finish();

		void UseProgram();
		unsigned int GetProgram();

		void Run(unsigned int workgroup_size_x, unsigned int workgroup_size_y, unsigned int workgroup_size_z);
		void Run(std::array<unsigned int, 3> workgroup_dimensions);

	private:
		bool i_usable = IsComputeShaderUsable();

		const char* i_source;
		bool i_source_set = false;

		unsigned int i_compute_shader;
		unsigned int i_shader_program = -1;
	};
}

#endif