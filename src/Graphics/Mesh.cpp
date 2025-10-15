#include "Mesh.h"
#include <filesystem>

/**************************************************************************************/

// extern includes

/**************************************************************************************/

namespace zap
{
	// Avoid copy by value for large data structures
	Mesh::Mesh(const std::vector<float>& extern_vertices, const std::vector<unsigned int>& extern_indices) : Mesh (extern_vertices)
	{
		PreSetIndices (extern_indices);
	}
	Mesh::Mesh(const std::vector<float>& extern_vertices) : Mesh()
	{
		PreSetVertices(extern_vertices);
	}
	Mesh::Mesh()
	{
	}
	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
	}

	void Mesh::PreSetVertices(const std::vector<float>& extern_vertices)
	{
		vertices = extern_vertices;
	}
	void Mesh::PreSetIndices(const std::vector<unsigned int>& extern_indices)
	{
		indices = extern_indices;
	}
	void Mesh::SetVBOAccessMode(BufferAccessModes mode)
	{
		VBO_ACCESS_MODE = mode;
	}

	void Mesh::SetEBOAccessMode(BufferAccessModes mode)
	{
		EBO_ACCESS_MODE = mode;
	}

	AttributeConfig& Mesh::SetAttribPointer (int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos, bool instanced)
	{
		return attribcfg.emplace_back(AttributeConfig { shader_location, value_ct, data_stride, start_pos, instanced });
	}

	void Mesh::ClearAllTextures()
	{
		for (auto& cfg : texturecfg)
		{
			cfg.second.deleteTexture();
		}
		texturecfg.clear();
	}
	Texture& Mesh::AddTextureFromPath (unsigned int hash, const std::string path, TextureFilter filter, MipmapSetting settings, TextureWrapping wrapping)
	{
		assert(texturecfg.find(hash) == texturecfg.end() && "This ID is already used, please fix nonsense");
		texturecfg.emplace(hash, Texture{ hash, path, filter, settings, wrapping });
		return texturecfg[hash];
	}

	Texture& Mesh::AddTextureFromData(unsigned int hash, unsigned char* texture_data, int texture_width, int texture_height, GLenum Type, TextureFilter filter, MipmapSetting settings, TextureWrapping wrapping)
	{
		assert(texturecfg.find(hash) == texturecfg.end() && "This ID is already used, please fix nonsense");
		if (texture_width < 0 || texture_height < 0)
		{
			messages::PrintMessage("Given texture dimensions are not valid", "Texture& zap::Mesh::AddTexture(...)", MessageTypes::fatal_error);
			ZAP_INTERRUPT_FATAL_ERROR;
		}
		texturecfg.emplace(hash, Texture{ hash, texture_data, texture_width, texture_height, Type, filter, settings, wrapping });
		return texturecfg[hash];
	}

	Texture& Mesh::AddTextureFromPath(const std::string path, TextureFilter filter, MipmapSetting settings, TextureWrapping wrapping)
	{
		int hash = texturecfg.rbegin()->first + 1;//autogenerate hash, not tested
		return AddTextureFromPath(hash, path, filter, settings, wrapping);
	}

	Texture& Mesh::AddTextureFromData(unsigned char* texture_data, int texture_width, int texture_height, GLenum Type, TextureFilter filter, MipmapSetting settings, TextureWrapping wrapping)
	{
		int hash = texturecfg.rbegin()->first + 1;//autogenerate hash, not tested
		return AddTextureFromData(hash, texture_data, texture_width, texture_height, Type, filter, settings, wrapping);
	}

	// TODO: Move the shader functionality to a separate class, totally five functions to be moved
	void Mesh::SetVertexShaderSource(const std::string& source)
	{
		// Just assert nothing fatal here. Nothing truly bad happens if the source is set multiple times, but it indicates bad redundant code.
		//ZAP_ASSERT_TRUE(vertexShaderSource.empty() && "Vertex Shader Source was already set, redundant code should be identified and removed");
		vertexShaderSource = source;
	}

	void Mesh::SetFragmentShaderSource(const std::string& source)
	{
		// Just assert nothing fatal here. Nothing truly bad happens if the source is set multiple times, but it indicates bad redundant code.
		//ZAP_ASSERT_TRUE(fragmentShaderSource.empty() && "Fragment Shader Source was already set, redundant code should be identified and removed");
		fragmentShaderSource = source;
	}

	void Mesh::SetVertexShaderFilePath(const std::string& vertexShaderFilepath)
	{
		//Require path to exist. Missing path is a fatal error, no go from this point.
		ZAP_REQUIRE_ALL(std::filesystem::exists(vertexShaderFilepath.c_str()) && "Vertex Shader File Path can not be found: " && vertexShaderFilepath.c_str());
		SetVertexShaderSource(zap::util::GetTextFileContent(vertexShaderFilepath.c_str())); // Reuse code, avoid redundancy
	}
	void Mesh::SetFragmentShaderFilePath(const std::string& fragmentShaderFilepath)
	{
		//Require path to exist. Missing path is a fatal error, no go from this point.
		ZAP_REQUIRE_ALL(std::filesystem::exists(fragmentShaderFilepath.c_str()) && "Vertex Shader File Path can not be found: " && fragmentShaderFilepath.c_str());
		SetFragmentShaderSource (zap::util::GetTextFileContent(fragmentShaderFilepath.c_str())); // Reuse code, avoid redundancy
	}
	void Mesh::SetVertexShaderPath(const std::string& source)
	{
		// If source is an existing path, load from path, else use as source code
		if (std::filesystem::exists(source.c_str()))
		{
			SetVertexShaderFilePath(source);
		}
		else
		{
			SetVertexShaderSource(source);
		}
	}
	void Mesh::SetFragmentShaderPath(const std::string& source)
	{
		// If source is an existing path, load from path, else use as source code
		if (std::filesystem::exists(source.c_str()))
		{
			SetFragmentShaderFilePath(source);
		}
		else
		{
			SetFragmentShaderSource(source);
		}
	}


	void Mesh::BuildProgram()
	{
		// Use default shaders when none provided
		if (vertexShaderSource.empty ()) vertexShaderSource = defaultVertexShaderSource;
		if (fragmentShaderSource.empty()) fragmentShaderSource = defaultFragmentShaderSource;

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

	void Mesh::VertexBufferData()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), (GLenum)VBO_ACCESS_MODE);
	}
	void Mesh::VertexBufferData (const std::vector<float>& newBufferData)
	{
		ZAP_REQUIRE(newBufferData.size() == vertices.size() && "New buffer data must be of the same size as the existing vertex buffer data");
		vertices = newBufferData;
		VertexBufferData();
	}

	void Mesh::GenObject()
	{
		/*****************************************************************************************/
		// Buffer
		// VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// VBO
		glGenBuffers(1, &VBO);
		VertexBufferData();

		for (auto& cfg : attribcfg)
		{
			cfg.vertexAttribPointer();
		}

		// EBO
		if (!indices.empty ())
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), (GLenum)EBO_ACCESS_MODE);
		}
		//
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Mesh::UpdateMvpLocations()
	{
		mvp.view_location = GetUniformLocation("view");
		mvp.projection_location = GetUniformLocation("projection");
		mvp.model_location = GetUniformLocation("model");
	}
	void Mesh::Finish()
	{
		/******************************************************************************************/
		if (shaderProgram == -1)
		{
			BuildProgram();
		}

		GenObject();
		UpdateMvpLocations();

		//TODO: Remove this nonsense
		//      The genTexture will be done in the Textures constructor
		//      Texture generation is not program dependent, it is OpenGL Global
		//Textures
		//for (auto& texcfg : texturecfg)
		//{
		//	texcfg.genTexture();
		//}
		/*****************************************************************************************/
	}

	bool Mesh::BindTextureByHash(unsigned int hash)
	{
		assert(texturecfg.find(hash) != texturecfg.end() && "Missing hash is utterly nonsense, find the bug and fix it");
		texturecfg[hash].bind();
		return true;
	}
	//Texture texture;
	Texture& Mesh::GetTextureByHash(unsigned int hash)
	{
		assert(texturecfg.find(hash) != texturecfg.end() && "Missing hash is utterly nonsense, find the bug and fix it");
		return texturecfg[hash];
	}

	std::vector<float>& Mesh::GetVertices()
	{
		return vertices;
	}

	std::vector<unsigned int>& Mesh::GetIndices()
	{
		return indices;
	}

	void Mesh::UpdateModel()
	{
		glUniformMatrix4fv(mvp.model_location, 1, GL_FALSE, glm::value_ptr(model));
	}
	void Mesh::SetProjection(glm::mat4 proj)
	{
		glUniformMatrix4fv(mvp.projection_location, 1, GL_FALSE, glm::value_ptr(proj));
	}
	void Mesh::SetView(glm::mat4 view)
	{
		glUniformMatrix4fv(mvp.view_location, 1, GL_FALSE, glm::value_ptr(view));
	}

	void Mesh::UpdateModel(glm::mat4 mdl) 
	{
		glUniformMatrix4fv(mvp.model_location, 1, GL_FALSE, glm::value_ptr(mdl));
	}

	glm::mat4 Mesh::GetModel() const
	{
		return model;
	}
	void Mesh::SetModel(const glm::mat4& mdl)
	{
		model = mdl;
	}

	unsigned int Mesh::GetVBO() const
	{
		return VBO;
	}
	void Mesh::SetVBO(unsigned int& BO)
	{
		VBO = BO;
	}

	unsigned int Mesh::GetVAO() const
	{
		return VAO;
	}
	void Mesh::SetVAO(unsigned int& AO)
	{
		VAO = AO;
	}

	unsigned int Mesh::GetEBO() const
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
		if (!indices.empty())
		{
			glUseProgram (shaderProgram);
			glBindVertexArray (VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertices_count);
		}
	}

	void Mesh::DrawInstanced(unsigned int draw_amount, int vertices_count)
	{
		if(!indices.empty())
		{
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, draw_amount);
		}
		else 
		{
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElementsInstanced(GL_TRIANGLES, vertices_count, GL_UNSIGNED_INT, 0, draw_amount);
		}
	}

}


