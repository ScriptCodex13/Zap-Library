#pragma once

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <memory>

#include "Message.h"
#include "stb_image.h"

namespace ez
{
	enum class BufferAccessModes
	{
		LOW_ACCESS_STATIC,
		HIGH_ACESS_STATIC,
		HIGH_ACESS_DYNAMIC
	};

	enum class TextureFilters
	{
		NEAREST,
		LINEAR
	};

	enum class MipmapSettings
	{
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_LINEAR,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST
	};

	enum class TextureWrapping
	{
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	struct AttributeConfig
	{
		AttributeConfig(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos);
		~AttributeConfig();

		int i_shader_location;
		int i_value_ct;
		unsigned int i_data_stride;
		unsigned int i_start_pos;
	};

	struct Texture
	{
		Texture(unsigned int id, const std::string path, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);
		~Texture();

		unsigned int i_texture;
		std::string i_path;
		unsigned char* i_texturedata;

		TextureFilters i_filter = TextureFilters::LINEAR;
		MipmapSettings i_settings = MipmapSettings::LINEAR_MIPMAP_LINEAR;
		TextureWrapping i_wrapping = TextureWrapping::CLAMP_TO_BORDER;

		//bool i_usetexture = false;
		bool i_textureloaded = false;

		int i_width;
		int i_height;
		int i_nrChannels;

		unsigned int i_id;
	};

	class Mesh2D
	{
	public:
		Mesh2D(std::vector<float>* extern_vertices, std::vector<unsigned int>* extern_indices);
		~Mesh2D();

		//Cofig Process

		void SetVertexShaderSource(const char* source);
		void SetVertexShaderSource(const std::string* source);

		void SetFragmentShaderSource(const char* source);
		void SetFragmentShaderSource(const std::string* source);

		void SetVBOAccessMode(BufferAccessModes mode);
		void SetEBOAccessMode(BufferAccessModes mode);

		void UseEBO(bool state);

		void SetAttribPointer(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos);
		//

		void UseTexture(bool state);
		void InitTexture(unsigned int id, const std::string path, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping i_wrapping = TextureWrapping::CLAMP_TO_BORDER);
		

		void Finish(); // Alles wird erstellt. Es können keine Einstellungen mehr vorgenommen werden
	
		void SetTexture(unsigned int id);
		void Write(int vertices_count);
		// Transform func

	private:
		//Sources
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		const char* vertexShaderSource;
		const char* fragmentShaderSource;

		bool vSourceset = false;
		bool fSourceset = false;

		//Shaders
		unsigned int vertexShader;
		unsigned int fragmentShader;

		//Shader link
		unsigned int shaderProgram;

		//Buffer Objects

		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;

		BufferAccessModes VBO_ACCESS_MODE;
		BufferAccessModes EBO_ACCESS_MODE;
		
		bool useEBO;

		std::vector<std::unique_ptr<AttributeConfig>> attribcfg;

		//Textures

		std::vector<std::unique_ptr<Texture>> texturecfg;
		bool use_texture;
		
	};

	class Mesh3D
	{

	};
};


#endif