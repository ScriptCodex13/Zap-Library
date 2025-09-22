#include "Mesh.h"

/**************************************************************************************/

// extern includes

/**************************************************************************************/

namespace zap
{


	Mesh::Mesh(std::vector<float> extern_vertices, std::vector<unsigned int> extern_indices)
	{
		vertices = extern_vertices;
		indices  = extern_indices;

		if (indices.empty()) use_indices = false;
	}
	Mesh::Mesh(std::vector<float> extern_vertices)
	{
		vertices = extern_vertices;

		use_indices = false;
	}
	Mesh::Mesh()
	{
		use_indices = false;
	}
	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
	}

	void Mesh::SetVertexShaderSource(const std::string& source)
	{
		if (!vSourceset)
		{
			vertexShaderSource = source;
			vSourceset = true;
		}
	}

	void Mesh::SetFragmentShaderSource(const std::string& source)
	{
		if (!fSourceset)
		{
			fragmentShaderSource = source;
			fSourceset = true;
		}
	}

	void Mesh::SetVBOAccessMode(BufferAccessModes mode)
	{
		VBO_ACCESS_MODE = mode;
	}

	void Mesh::SetEBOAccessMode(BufferAccessModes mode)
	{
		EBO_ACCESS_MODE = mode;
	}

	AttributeConfig& Mesh::SetAttribPointer (int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos)
	{
		return attribcfg.emplace_back(AttributeConfig { shader_location, value_ct, data_stride, start_pos });
	}

	Texture& Mesh::AddTexture (unsigned int id, const std::string path, TextureFilters filter, MipmapSettings settings, TextureWrapping wrapping)
	{
		return texturecfg.emplace_back(Texture{ id, path, filter, settings, wrapping });
	}

	Texture& Mesh::AddTexture(unsigned int id, unsigned char* texture_data, int texture_width, int texture_height, GLenum Type, TextureFilters filter, MipmapSettings settings, TextureWrapping wrapping)
	{
		if (texture_width < 0 || texture_height < 0)
		{
			messages::PrintMessage("Given texture dimensions are not valid", "Texture& zap::Mesh::AddTexture(...)", MessageTypes::fatal_error);
			ZAP_INTERRUPT_FATAL_ERROR;
		}

		return texturecfg.emplace_back(Texture{ id, texture_data, texture_width, texture_height, Type, filter, settings, wrapping });
	}

	void Mesh::BuildProgram()
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* src = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &src, NULL);
		glCompileShader(vertexShader);

		int ok;
		char info[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ok);

		if (!ok)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, info);
			messages::PrintMessage("Failed to compile Vertex Shader:", "Mesh.cpp/void zap::Mesh2D::Finish()", MessageTypes::error);
			std::cerr << info << std::endl;
		}

		// FragmentShader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		src = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &src, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ok);

		if (!ok)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, info);
			messages::PrintMessage("Failed to compile Fragment Shader:", "Mesh.cpp/void zap::Mesh2D::Finish()", MessageTypes::error);
			std::cerr << info << std::endl;
		}

		//


		//Shader Program

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ok);

		if (!ok)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, info);
			messages::PrintMessage("Failed to link Shader Program:", "Mesh.cpp/void zap::Mesh2D::Finish()", MessageTypes::error);
			std::cerr << info << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	void Mesh::GenObject()
	{
		/*****************************************************************************************/
		// Buffer
		// VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// VBO
		// F
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), (GLenum)VBO_ACCESS_MODE);

		for (auto& cfg : attribcfg)
		{
			cfg.vertexAttribPointer();
		}

		// EBO
		if (use_indices)
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), (GLenum)EBO_ACCESS_MODE);
		}
		//
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Mesh::Finish()
	{
		/******************************************************************************************/
		if (shaderProgram == -1)
		{
			BuildProgram();
		}

		GenObject();

		//f
		//Textures
		for (auto& texcfg : texturecfg)
		{
			texcfg.genTexture();
		}
		/*****************************************************************************************/
	}

	bool Mesh::UseTexture(unsigned int id)
	{
		//auto cfg = std::find_if(texturecfg.begin(), texturecfg.end(), [id] (const auto& x) {return x.i_id == id; });
		
		//why did you replace the STL find?
		for (auto cfg : texturecfg)
		{
			if (cfg.i_id == id) 
			{
				cfg.bind();
				return true;
			}
		}
		
		return false;
	}

	std::vector<float>& Mesh::GetVertices()
	{
		return vertices;
	}

	std::vector<unsigned int>& Mesh::GetIndices()
	{
		return indices;
	}

	void Mesh::UpdateModel(unsigned int model_uniform_location)
	{
		glUniformMatrix4fv(model_uniform_location, 1, GL_FALSE, glm::value_ptr(model));
	}

	//TODO: Never return reference in a getter
	//      There are explicit coding conventions for getters and setters
	//      Getters return by value, setters set the value
	glm::mat4 Mesh::GetModel()
	{
		return model;
	}
	void Mesh::SetModel(const glm::mat4& mdl)
	{
		model = mdl;
	}

	unsigned int Mesh::GetVBO()
	{
		return VBO;
	}
	void Mesh::SetVBO(unsigned int& BO)
	{
		VBO = BO;
	}

	unsigned int Mesh::GetVAO()
	{
		return VAO;
	}
	void Mesh::SetVAO(unsigned int& AO)
	{
		VAO = AO;
	}

	unsigned int Mesh::GetEBO()
	{
		return EBO;
	}
	void Mesh::SetEBO(unsigned int& EB)
	{
		EBO = EB;
	}

	unsigned int Mesh::GetUniformLocation(const GLchar* name)
	{ 
		return glGetUniformLocation(shaderProgram, name);
	}
	unsigned int Mesh::GetProgram()
	{ 
		return shaderProgram;
	}
	void Mesh::UseProgram()
	{ 
		glUseProgram (shaderProgram); 
	}

	void Mesh::BindVAO()
	{ 
		glBindVertexArray (VAO); 
	}

	void Mesh::Bind()
	{
		UseProgram ();
		BindVAO ();
	}

	void Mesh::Draw(int vertices_count)
	{
		if (use_indices)
		{
			glUseProgram (shaderProgram);
			glBindVertexArray (VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			//vertices_count = vertices.size() / attribcfg[0].i_value_ct;
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertices_count);
		}
	}

}


