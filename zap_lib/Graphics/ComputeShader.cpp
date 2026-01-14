#include "ComputeShader.h"

namespace zap
{
	ComputeShader::ComputeShader()
	{

	}

	ComputeShader::~ComputeShader()
	{

	}

	void ComputeShader::SetSource(const std::string& source)
	{
		i_source = source.c_str();
		i_source_set = true;
	}

	void ComputeShader::SetShaderFilePath(const std::string& path)
	{
		if(!std::filesystem::exists(path))
		{
			messages::PrintMessage
			(
				"Cannot find file at the given path: " + path, 
				"ComputeShader.cpp/ void zap::ComputeShader::SetShaderFilePath(...)", 
				MessageTypes::error
			);

			i_source_set = false;
			
			return;
		}

		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			file.open(path);
			std::stringstream stream;
			stream << file.rdbuf();
			file.close();
			i_source = stream.str();

			i_source_set = true;
		}
		catch (std::ifstream::failure e)
		{
			i_source_set = false;
			messages::PrintMessage("Error while reading file: " + (std::string)e.what(), "ComputeShader.cpp/ void zap::ComputeShader::SetShaderFilePath(...)", MessageTypes::error);
		}
	}

	void ComputeShader::Finish()
	{ 
		if(!i_usable)
		{
			messages::PrintMessage
			(
				"Cannot create compute shader: At least OpenGL version 4.3 required",
				"ComputeShader.cpp/ void zap::ComputeShader::SetShaderFilePath(...)",
				MessageTypes::fatal_error
			);

			return;
		}
		if(!i_source_set)
		{
			messages::PrintMessage
			(
				"Cannot create compute shader: No source provided",
				"ComputeShader.cpp/ void zap::ComputeShader::SetShaderFilePath(...)",
				MessageTypes::fatal_error
			);

			return;
		}
		
		i_compute_shader = glCreateShader(GL_COMPUTE_SHADER);
		char* ptr = i_source.data();
		glShaderSource(i_compute_shader, 1, &ptr, NULL);
		glCompileShader(i_compute_shader);

		// Check compile errors

		int sucess;
		char info[1024];

		glGetShaderiv(i_compute_shader, GL_COMPILE_STATUS, &sucess);
		if(!sucess)
		{
			glGetShaderInfoLog(i_compute_shader, 1024, NULL, info);
			messages::PrintMessage("Failed to compile compute shader:", "ComputeShader.cpp/void zap::ComputeShader::Finish()", MessageTypes::error);
			std::cerr << info << std::endl;
		}

		//

		i_shader_program = glCreateProgram();
		glAttachShader(i_shader_program, i_compute_shader);
		glLinkProgram(i_shader_program);
		glDeleteShader(i_compute_shader);

		// Check compile errors

		glGetProgramiv(i_compute_shader, GL_COMPILE_STATUS, &sucess);
		if (!sucess)
		{
			glGetProgramInfoLog(i_compute_shader, 1024, NULL, info);
			messages::PrintMessage("Failed to link compute shader program:", "ComputeShader.cpp/void zap::ComputeShader::Finish()", MessageTypes::error);
			std::cerr << info << std::endl;
		}

		//
	}

	void ComputeShader::UseProgram()
	{
		glUseProgram(i_shader_program);
	}

	unsigned int ComputeShader::GetProgram()
	{
		return i_shader_program;
	}

	void ComputeShader::Run(unsigned int workgroup_size_x, unsigned int workgroup_size_y, unsigned int workgroup_size_z)
	{
		glDispatchCompute(workgroup_size_x, workgroup_size_y, workgroup_size_z);
	}

	void ComputeShader::Run(std::array<unsigned int, 3> workgroup_dimensions)
	{
		Run(workgroup_dimensions[0], workgroup_dimensions[1], workgroup_dimensions[2]);
	}
}