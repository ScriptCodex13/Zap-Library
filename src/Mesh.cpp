#include "Mesh.h"

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

#include <filesystem>
namespace zap
{
	AttributeConfig::AttributeConfig(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos) :
		i_shader_location (shader_location),
		i_value_ct        (value_ct),
		i_data_stride     (data_stride),
		i_start_pos       (start_pos)
	{
	}
	void AttributeConfig::vertexAttribPointer() 
	{
		glVertexAttribPointer (i_shader_location, i_value_ct, i_type, i_normalized, i_data_stride * sizeof(float), getPos());
		glEnableVertexAttribArray(i_shader_location);
	}

	Texture::Texture(unsigned int id, const std::string path,
						unsigned int     texture_index,
						TextureFilters   filter,
						MipmapSettings   settings,
						TextureWrapping  wrapping) :
		i_id             (id),
		i_path           (path),
		i_filter         (filter),
		i_settings       (settings),
		i_wrapping       (wrapping),
		i_texture_index  (texture_index)

	{
		glGenTextures(1, &i_texture);
	}
	void Texture::genTexture()
	{
		namespace fs = std::filesystem;
		using std::cout;
		using std::endl;
		//glBindTexture (GL_TEXTURE_2D, i_texture);
		bind();

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)i_wrapping);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)i_wrapping);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_settings);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_filter);

		bool usePng = fs::path(i_path).extension() == ".png"; //TODO: so what?

		stbi_set_flip_vertically_on_load(true);
		unsigned char* texturedata = stbi_load(i_path.c_str(), &i_width, &i_height, &i_nrChannels, 0); // Bessere Lösung nutze Filepath wie im Tutorial !
		if (!texturedata)
		{
			messages::PrintMessage("Failed to load Texture at path: " + i_path, "Mesh.cpp/ez::Texture::Texture(...)", MessageTypes::error);
			return;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i_width, i_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
		stbi_image_free(texturedata);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::bind () { glBindTexture(GL_TEXTURE_2D, i_texture); }

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

	void Mesh2D::UsePNG (unsigned int id)
	{
		auto cfg = std::find_if(texturecfg.begin(), texturecfg.end(), [id](const auto& x) {return x.i_id == id; });
		if (cfg == texturecfg.end()) return;
		cfg->i_usePNG = true; //TODO: so what?
	}

	AttributeConfig& Mesh2D::SetAttribPointer (int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos)
	{
		return attribcfg.emplace_back(AttributeConfig{ shader_location, value_ct, data_stride, start_pos });
	}

	Texture& Mesh2D::InitTexture (unsigned int id, const std::string path, unsigned int texture_index, TextureFilters filter, MipmapSettings settings, TextureWrapping wrapping)
	{
		return texturecfg.emplace_back(Texture { id, path, texture_index, filter, settings, wrapping });
	}

	void Mesh2D::Finish()
	{
		/******************************************************************************************/

		// Shader 
		// Vertex Shader
		vertexShader = glCreateShader (GL_VERTEX_SHADER);
		const char* src = vertexShaderSource.c_str();
		glShaderSource  (vertexShader, 1, &src, NULL);
		glCompileShader (vertexShader);

		int ok;
		char info[512];

		glGetShaderiv (vertexShader, GL_COMPILE_STATUS, &ok);

		if (!ok)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, info);
			messages::PrintMessage("Failed to compile Vertex Shader:", "Mesh.cpp/void ez::Mesh2D::Finish()", MessageTypes::error);
			std::cerr << info << std::endl;
		}

		// FragmentShader
		fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
		src = fragmentShaderSource.c_str();
		glShaderSource  (fragmentShader, 1, &src, NULL);
		glCompileShader (fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ok);
		
		if (!ok)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, info);
			messages::PrintMessage("Failed to compile Fragment Shader:", "Mesh.cpp/void ez::Mesh2D::Finish()", MessageTypes::error);
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
			messages::PrintMessage("Failed to link Shader Program:", "Mesh.cpp/void ez::Mesh2D::Finish()", MessageTypes::error);
			std::cerr << info << std::endl;
		}


		//

		//Delete Shaders
		glDeleteShader (vertexShader);
		glDeleteShader (fragmentShader);
		//

		/*****************************************************************************************/
		// Buffer
		// VAO
		glGenVertexArrays (1, &VAO);
		glBindVertexArray (VAO);

		// VBO
		// F
		glGenBuffers (1, &VBO);
		glBindBuffer (GL_ARRAY_BUFFER, VBO);
		glBufferData (GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), (GLenum)VBO_ACCESS_MODE);

		for (auto& cfg : attribcfg)
			cfg.vertexAttribPointer();

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
		//f

		//Textures
		for (auto& texcfg : texturecfg) texcfg.genTexture();

		/*****************************************************************************************/
	}

	bool Mesh2D::SetTexture(unsigned int id)
	{
		auto cfg = std::find_if(texturecfg.begin(), texturecfg.end(), [id] (const auto& x) {return x.i_id == id; });
		if (cfg == texturecfg.end()) return false;
		cfg->bind();

		return true;
	}

	void Mesh2D::useProgram () { glUseProgram (shaderProgram); }
	void Mesh2D::bindVAO    () { glBindVertexArray (VAO); }
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
