<<<<<<< HEAD
#include "Mesh.h"

/**************************************************************************************/

// extern includes

#include <filesystem>

/**************************************************************************************/

namespace zap
{


	Mesh2D::Mesh2D(std::vector<float>* extern_vertices, std::vector<unsigned int>* extern_indices)
	{
		vertices = *extern_vertices;
		indices  = *extern_indices;
	}
	Mesh2D::~Mesh2D()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
	}

	void Mesh2D::SetVertexShaderSource(const std::string& source)
	{
		if (!vSourceset)
		{
			vertexShaderSource = source;
			vSourceset = true;
		}
	}

	void Mesh2D::SetFragmentShaderSource(const std::string& source) 
	{
		if (!fSourceset)
		{
			fragmentShaderSource = source;
			fSourceset = true;
		}
	}

	void Mesh2D::SetVBOAccessMode(BufferAccessModes mode)
	{
		VBO_ACCESS_MODE = mode;
	}

	void Mesh2D::SetEBOAccessMode(BufferAccessModes mode)
	{
		EBO_ACCESS_MODE = mode;
	}

	AttributeConfig& Mesh2D::SetAttribPointer (int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos)
	{
		return attribcfg.emplace_back(AttributeConfig { shader_location, value_ct, data_stride, start_pos });
	}

	Texture& Mesh2D::AddTexture (unsigned int id, const std::string path, TextureFilters filter, MipmapSettings settings, TextureWrapping wrapping)
	{
		return texturecfg.emplace_back(Texture{ id, path, filter, settings, wrapping });
	}

	void Mesh2D::BuildProgram()
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

	void Mesh2D::GenObject()
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
		if (!indices.empty())
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), (GLenum)EBO_ACCESS_MODE);
		}
		//
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Mesh2D::Finish()
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

	bool Mesh2D::UseTexture(unsigned int id)
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

	unsigned int Mesh2D::GetUniformLocation(const GLchar* name)
	{ 
		return glGetUniformLocation(shaderProgram, name);
	}
	unsigned int Mesh2D::GetProgram()
	{ 
		return shaderProgram;
	}
	void Mesh2D::useProgram() 
	{ 
		glUseProgram (shaderProgram); 
	}

	void Mesh2D::bindVAO() 
	{ 
		glBindVertexArray (VAO); 
	}

	void Mesh2D::bind()
	{
		useProgram ();
		bindVAO ();
	}

	void Mesh2D::Write(int vertices_count)
	{
		if (!indices.empty())
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


=======
#include "Mesh.h"

/**************************************************************************************/

// extern includes

#include <filesystem>

/**************************************************************************************/

namespace zap
{


	Mesh2D::Mesh2D(std::vector<float>* extern_vertices, std::vector<unsigned int>* extern_indices)
	{
		vertices = *extern_vertices;
		indices  = *extern_indices;
	}
	Mesh2D::~Mesh2D()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
	}

	void Mesh2D::SetVertexShaderSource(const std::string& source)
	{
		if (!vSourceset)
		{
			vertexShaderSource = source;
			vSourceset = true;
		}
	}

	void Mesh2D::SetFragmentShaderSource(const std::string& source) 
	{
		if (!fSourceset)
		{
			fragmentShaderSource = source;
			fSourceset = true;
		}
	}

	void Mesh2D::SetVBOAccessMode(BufferAccessModes mode)
	{
		VBO_ACCESS_MODE = mode;
	}

	void Mesh2D::SetEBOAccessMode(BufferAccessModes mode)
	{
		EBO_ACCESS_MODE = mode;
	}

	AttributeConfig& Mesh2D::SetAttribPointer (int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos)
	{
		return attribcfg.emplace_back(AttributeConfig { shader_location, value_ct, data_stride, start_pos });
	}

	Texture& Mesh2D::AddTexture (unsigned int id, const std::string path, TextureFilters filter, MipmapSettings settings, TextureWrapping wrapping)
	{
		return texturecfg.emplace_back(Texture{ id, path, filter, settings, wrapping });
	}

	void Mesh2D::BuildProgram()
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

	void Mesh2D::GenObject()
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
		if (!indices.empty())
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), (GLenum)EBO_ACCESS_MODE);
		}
		//
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Mesh2D::Finish()
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

	bool Mesh2D::UseTexture(unsigned int id)
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

	unsigned int Mesh2D::GetUniformLocation(const GLchar* name)
	{ 
		return glGetUniformLocation(shaderProgram, name);
	}
	unsigned int Mesh2D::GetProgram()
	{ 
		return shaderProgram;
	}
	void Mesh2D::useProgram() 
	{ 
		glUseProgram (shaderProgram); 
	}

	void Mesh2D::bindVAO() 
	{ 
		glBindVertexArray (VAO); 
	}

	void Mesh2D::bind()
	{
		useProgram ();
		bindVAO ();
	}

	void Mesh2D::Draw(int vertices_count)
	{
		if (!indices.empty())
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


>>>>>>> 09c8a20d8a7b093eaf3d273304a5ce183f635844
