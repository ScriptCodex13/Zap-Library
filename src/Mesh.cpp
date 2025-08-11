#include "Mesh.h"

namespace ez
{
	AttributeConfig::AttributeConfig(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos)
	{
		i_shader_location = shader_location;
		i_value_ct = value_ct;
		i_data_stride = data_stride;
		i_start_pos = start_pos;
	}

	AttributeConfig::~AttributeConfig()
	{

	}

	Texture::Texture(unsigned int id, const std::string path, TextureFilters filter, MipmapSettings settings, TextureWrapping wrapping)
	{
		i_id = id;
		i_path = path;

		i_texturedata = stbi_load(i_path.c_str(), &i_width, &i_height, &i_nrChannels, 0); // Bessere Lösung nutze Filepath wie im Tutorial !

		i_filter = filter;
		i_settings = settings;
		i_wrapping = wrapping;

		if (i_texturedata)
		{
			i_textureloaded = true;
		}
		else
		{
			messages::PrintMessage("Failed to load Texture at path: " + i_path, "Mesh.cpp/ez::Texture::Texture(...)", MessageTypes::error);
		}
	}

	Texture::~Texture()
	{
		stbi_image_free(i_texturedata);
	}


	Mesh2D::Mesh2D(std::vector<float>* extern_vertices, std::vector<unsigned int>* extern_indices)
	{
		vertices = *extern_vertices;
		indices = *extern_indices;

		VBO_ACCESS_MODE = BufferAccessModes::HIGH_ACESS_DYNAMIC;
		EBO_ACCESS_MODE = BufferAccessModes::HIGH_ACESS_DYNAMIC;

	}
	Mesh2D::~Mesh2D()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
	}

	void Mesh2D::SetVertexShaderSource(const char* source)
	{
		if (!vSourceset)
		{
			vertexShaderSource = source;
			vSourceset = true;
		}
	}

	void Mesh2D::SetVertexShaderSource(const std::string* source)
	{
		if (!vSourceset)
		{
			vertexShaderSource = source->c_str();
			vSourceset = true;
		}
	}

	void Mesh2D::SetFragmentShaderSource(const char* source)
	{
		if (!fSourceset)
		{
			fragmentShaderSource = source;
			fSourceset = true;
		}
	}

	void Mesh2D::SetFragmentShaderSource(const std::string * source) 
	{
		if (!fSourceset)
		{
			fragmentShaderSource = source->c_str();
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

	void Mesh2D::UseEBO(bool state)
	{
		useEBO = state;
	}

	void Mesh2D::SetAttribPointer(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos)
	{
		attribcfg.push_back(std::make_unique<AttributeConfig>(shader_location, value_ct, data_stride, start_pos));
	}

	void Mesh2D::UseTexture(bool state)
	{
		use_texture = state;
	}

	void Mesh2D::InitTexture(unsigned int id, const std::string path, TextureFilters filter, MipmapSettings settings, TextureWrapping wrapping)
	{
		texturecfg.push_back(std::make_unique<Texture>(id, path, filter, settings, wrapping));
	}


	void Mesh2D::Finish()
	{
		/******************************************************************************************/

		// Shader 
		
		//Source

		if (!vSourceset)
		{
			vertexShaderSource =    R"glsl(
											#version 330 core
											layout (location = 0) in vec3 aPos;

											void main() 
											{
												gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
											}
									)glsl";
		}

		if (!fSourceset)
		{
			fragmentShaderSource = R"glsl(
											#version 330 core
											out vec4 FragColor;

											void main() 
											{
												FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
											}
									)glsl";
		}

		//

		// Vertex Shader

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int ok;
		char info[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ok);

		if (!ok)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, info);
			messages::PrintMessage("Failed to compile Vertex Shader:", "Mesh.cpp/void ez::Mesh2D::Finish()", MessageTypes::error);
			std::cerr << info << std::endl;
		}

		//



		// FragmentShader

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

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

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//

		/*****************************************************************************************/
	
		
		
		/*****************************************************************************************/

		// Buffer

		//VAO

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//

		//VBO

		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		if (VBO_ACCESS_MODE == BufferAccessModes::LOW_ACCESS_STATIC)
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);
		}
		else if (VBO_ACCESS_MODE == BufferAccessModes::HIGH_ACESS_STATIC)
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		}
		else if (VBO_ACCESS_MODE == BufferAccessModes::HIGH_ACESS_DYNAMIC)
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
		}

		if (!attribcfg.empty())
		{
			for (int i = 0; i < attribcfg.size(); i++)
			{
				glVertexAttribPointer(attribcfg[i]->i_shader_location, attribcfg[i]->i_value_ct, GL_FLOAT, GL_FALSE, attribcfg[i]->i_data_stride * sizeof(float), (void*)(attribcfg[i]->i_start_pos * sizeof(float)));
				glEnableVertexAttribArray(attribcfg[i]->i_shader_location);
			}
		}
		else
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		//


		// EBO

		if (useEBO)
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

			if (EBO_ACCESS_MODE == BufferAccessModes::LOW_ACCESS_STATIC)
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STREAM_DRAW);
			}
			else if (EBO_ACCESS_MODE == BufferAccessModes::HIGH_ACESS_STATIC)
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
			}
			else if (EBO_ACCESS_MODE == BufferAccessModes::HIGH_ACESS_DYNAMIC)
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
			}
		}

		//

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/*****************************************************************************************/


		/*****************************************************************************************/

		//Textures

		if (use_texture)
		{
			for (int t = 0; t < texturecfg.size(); t++)
			{
				glGenTextures(1, &texturecfg[t]->i_texture);
				glBindTexture(GL_TEXTURE_2D, texturecfg[t]->i_texture);

				if (texturecfg[t]->i_wrapping == TextureWrapping::REPEAT)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				}
				else if (texturecfg[t]->i_wrapping == TextureWrapping::MIRRORED_REPEAT)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				}
				else if (texturecfg[t]->i_wrapping == TextureWrapping::CLAMP_TO_EDGE)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				}
				else if (texturecfg[t]->i_wrapping == TextureWrapping::CLAMP_TO_BORDER)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				}

				if (texturecfg[t]->i_settings == MipmapSettings::NEAREST_MIPMAP_NEAREST)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				}
				else if (texturecfg[t]->i_settings == MipmapSettings::LINEAR_MIPMAP_LINEAR)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				}
				else if (texturecfg[t]->i_settings == MipmapSettings::NEAREST_MIPMAP_LINEAR)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				}
				else if (texturecfg[t]->i_settings == MipmapSettings::LINEAR_MIPMAP_NEAREST)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				}

				if (texturecfg[t]->i_filter == TextureFilters::NEAREST)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				else if (texturecfg[t]->i_filter == TextureFilters::LINEAR)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
			}
		}

		/*****************************************************************************************/
	}

	void Mesh2D::SetTexture(unsigned int id)
	{
		if (use_texture)
		{
			for (int e = 0; e < texturecfg.size(); e++)
			{
				if (texturecfg[e]->i_id == id)
				{
					glBindTexture(GL_TEXTURE_2D, texturecfg[e]->i_texture);
				}
			}
		}
	}


	void Mesh2D::Write(int vertices_count)
	{
		if (useEBO)
		{
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, vertices_count, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertices_count);
		}
	}

}
