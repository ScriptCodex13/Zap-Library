#pragma once

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>
#include <filesystem>

#include "Message.h"


namespace zap
{
	enum class BufferAccessModes : GLenum
	{
		LOW_ACCESS_STATIC   = GL_STREAM_DRAW,
		HIGH_ACESS_STATIC   = GL_STATIC_DRAW,
		HIGH_ACESS_DYNAMIC  = GL_DYNAMIC_DRAW
	};

	enum class TextureFilters : GLint
	{
		NEAREST = GL_NEAREST,
		LINEAR  = GL_LINEAR
	};

	enum class MipmapSettings : GLint
	{
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR,
		NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST
	};

	enum class TextureWrapping : GLint
	{
		REPEAT          = GL_REPEAT,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

	struct AttributeConfig
	{
		AttributeConfig(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos);

		void* getPos() { return  reinterpret_cast<void*>( i_start_pos * sizeof(float)); }
		void  vertexAttribPointer();

		int             i_shader_location;
		int             i_value_ct;
		GLenum          i_type = GL_FLOAT;
		GLboolean       i_normalized = GL_FALSE;
		unsigned int    i_data_stride;
		unsigned int    i_start_pos;
	};

	struct Texture
	{
		Texture(unsigned int id, const std::string path, unsigned int texture_index, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);

		void genTexture();
		void bind();

		unsigned int i_texture;
		std::string i_path;

		TextureFilters i_filter = TextureFilters::LINEAR;
		MipmapSettings i_settings = MipmapSettings::LINEAR_MIPMAP_LINEAR;
		TextureWrapping i_wrapping = TextureWrapping::CLAMP_TO_BORDER;

		//bool i_usetexture = false;
		bool i_textureloaded = false;

		int i_width;
		int i_height;
		int i_nrChannels;

		bool i_usePNG = false;

		unsigned int i_id;
		unsigned int i_texture_index;
	};

	inline const char* defaultVertexShaderSource = R"glsl(
											#version 330 core
											layout (location = 0) in vec3 aPos;

											void main()
											{
												gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
											}
									)glsl";
	inline const char* defaultFragmentShaderSource = R"glsl(
											#version 330 core
											out vec4 FragColor;

											void main()
											{
												FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
											}
									)glsl";
	class Mesh2D
	{
	public:
		Mesh2D(std::vector<float>* extern_vertices, std::vector<unsigned int>* extern_indices);
		~Mesh2D();

		//Cofig Process

		void SetVertexShaderSource(const std::string& source);

		void SetFragmentShaderSource(const std::string& source);

		void SetVBOAccessMode(BufferAccessModes mode);
		void SetEBOAccessMode(BufferAccessModes mode);

		void UsePNG(unsigned int id);

		AttributeConfig& SetAttribPointer(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos);
		//

		Texture& InitTexture(unsigned int id, const std::string path, unsigned int texture_index, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping i_wrapping = TextureWrapping::CLAMP_TO_BORDER);

		void Finish(); // Everything is finished you can't change the settings of the mesh anymore
		bool SetTexture (unsigned int id); //TODO: This is Bind, not Set, should be renamed | Done - ScriptCodex13
		//separate functions will be very useful in more complex logic
		unsigned int getUniformLocation(const GLchar* name);
		unsigned int getProgram ();
		void useProgram ();
		void bindVAO    ();
		void bind       ();
		void Write      (int vertices_count = 0);
		// Transform func

	private:
		//Sources
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		std::string vertexShaderSource   = defaultVertexShaderSource;
		std::string fragmentShaderSource = defaultFragmentShaderSource;

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
		unsigned int EBO = -1;

		BufferAccessModes VBO_ACCESS_MODE = BufferAccessModes::HIGH_ACESS_DYNAMIC;
		BufferAccessModes EBO_ACCESS_MODE = BufferAccessModes::HIGH_ACESS_DYNAMIC;

		std::vector<AttributeConfig> attribcfg;

		//Textures
		std::vector<Texture> texturecfg; // Needed to use shared_ptr because otherwise the Program crashes. Please do not change. If that here is very problematic we can find out a solution together.
		
	};

	class Mesh3D
	{

	};
};


#endif