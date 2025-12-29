#pragma once

#ifndef MESH_H
#define MESH_H

#include "../Util/Message.h"
#include "Texture.h"
#include "AttributeConfig.h"
#include "../Util/Util.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <map>
#include <string>
#include <bitset>


namespace zap
{
	enum class BufferAccessModes : GLenum
	{
		LOW_ACCESS_STATIC   = GL_STREAM_DRAW,
		HIGH_ACESS_STATIC   = GL_STATIC_DRAW,
		HIGH_ACESS_DYNAMIC  = GL_DYNAMIC_DRAW
	};

	//This is general code, it must be static and not contained in the object instances
	//It is accessible like this and does not make sense to make it non-static
	inline static const char* defaultVertexShaderSource = R"glsl(
											#version 330 core
											layout (location = 0) in vec3 aPos;
											void main()
											{
												gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
											}
									)glsl";
	inline static const char* defaultFragmentShaderSource = R"glsl(
											#version 330 core
											out vec4 FragColor;
											void main()
											{
												FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
											}
									)glsl";

	class Mesh
	{
	public:
		Mesh();
		// Avoid copy by value for large data structures
		Mesh(const std::vector<float>& extern_vertices, const std::vector<unsigned int>& extern_indices);
		Mesh(const std::vector<float>& extern_vertices);
		virtual ~Mesh();

		//Config Process
		// TODO: Move the shader functionality to a separate class, totally seven functions to be moved
		void SetVertexShaderSource     (const std::string& source);
		void SetFragmentShaderSource   (const std::string& source);
		void SetVertexShaderFilePath   (const std::string& vertexShaderFilepath);
		void SetFragmentShaderFilePath (const std::string& fragmentShaderFilepath);
		void SetVertexShaderPath       (const std::string& vertexShaderFilepath); // Automatic: If source is an existing path, load from path, else use as source code
		void SetFragmentShaderPath     (const std::string& fragmentShaderFilepath); // Automatic: If source is an existing path, load from path, else use as source code
		void BuildProgram();
		// END TODO:

		void SetVBOAccessMode  (BufferAccessModes mode);
		void SetEBOAccessMode  (BufferAccessModes mode);

		void PreSetVertices(const std::vector<float>& extern_vertices);
		void PreSetIndices(const std::vector<unsigned int>& extern_indices);
		AttributeConfig& SetAttribPointer(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos, bool instanced = false);
		//
		void ClearAllTextures();
		Texture& AddTextureFromPath(unsigned int hash, const std::string path, TextureFilter filter = TextureFilter::LINEAR, MipmapSetting settings = MipmapSetting::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);
		Texture& AddTextureFromData(unsigned int hash, unsigned char* texture_data, int texture_width, int texture_height, GLenum Type, TextureFilter filter = TextureFilter::LINEAR, MipmapSetting settings = MipmapSetting::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER); // Add texture but with manual loaded texture_data
		Texture& AddTextureFromPath(const std::string path, TextureFilter filter = TextureFilter::LINEAR, MipmapSetting settings = MipmapSetting::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);
		Texture& AddTextureFromData(unsigned char* texture_data, int texture_width, int texture_height, GLenum Type, TextureFilter filter = TextureFilter::LINEAR, MipmapSetting settings = MipmapSetting::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER); // Add texture but with manual loaded texture_data

		//Separate vertex buffer data functions for more complex logic
		void VertexBufferData();
		void VertexBufferData(const std::vector<float>& newBufferData);

		void GenObject();
		void Finish(); // ToDo: Check if one of the functions which require Finish is called before and throw errors 
		void UpdateMvpLocations();
		bool BindTextureByHash (unsigned int hash);
		Texture& GetTextureByHash(unsigned int hash);

		std::vector<float>& GetVertices();
		std::vector<unsigned int>& GetIndices();
		
		//TODO: Never return reference in a getter
		//      There are explicit coding conventions for getters and setters
		//      Getters return by value, setters set the value
		// For use with camera
		void UpdateModel() const;              //do not set but update in shader
		void UpdateModel(glm::mat4 mdl);       //set and update in shader
		void UpdateProjection() const;         //do not set but update in shader
		void UpdateProjection(glm::mat4 proj); //set and update in shader
		void UpdateView() const;               //do not set but update in shader
		void UpdateView(glm::mat4 view);       //set and update in shader

		glm::mat4 GetModel() const;
		glm::mat4 GetView() const;
		glm::mat4 GetProjection() const;

	public:
		unsigned int GetVBO() const;
		unsigned int GetVAO() const;
		unsigned int GetEBO() const;
	
	public:
		//separate functions will be very useful in more complex logic
		unsigned int GetUniformLocation(const GLchar* name)  const;
		unsigned int GetProgram () const;
		void Bind       () const;
		void Draw       (int vertices_count = 0);
		void DrawInstanced(unsigned int draw_amount, int vertices_count = 0);
		// Transform func

	    // private functions for intern use 
		void UseProgram() const;
		void BindVAO() const;

	private: //TODO: may be still useful in child classes 
		void SetVBO(unsigned int&);
		void SetVAO(unsigned int&);
		void SetEBO(unsigned int&);

	private: //TODO: may be still useful in child classes
		void SetModel(const glm::mat4&);
		void SetView(const glm::mat4& vw);
		void SetProjection(const glm::mat4& pj);

	protected:
		struct
		{
			glm::mat4 model; // PR
			glm::mat4 view; // PR
			glm::mat4 projection; // PR
			unsigned int model_location = -1;
			unsigned int view_location = -1;
			unsigned int projection_location = -1;
		} mvp;
	private:

	private:
		//Sources
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		std::string vertexShaderSource;
		std::string fragmentShaderSource;

		//Shaders
		unsigned int vertexShader;
		unsigned int fragmentShader;

		//Shader link
		unsigned int shaderProgram = -1;

		//Buffer Objects

		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO = -1;

		BufferAccessModes VBO_ACCESS_MODE = BufferAccessModes::HIGH_ACESS_DYNAMIC;
		BufferAccessModes EBO_ACCESS_MODE = BufferAccessModes::HIGH_ACESS_DYNAMIC;

		std::vector<AttributeConfig> attribcfg;

	protected:
		//Textures
		//std::vector<Texture> texturecfg; // Needed to use shared_ptr because otherwise the Program crashes. Please do not change. If that here is very problematic we can find out a solution together.
		std::map<unsigned int, Texture> texturecfg; // Needed to use shared_ptr because otherwise the Program crashes. Please do not change. If that here is very problematic we can find out a solution together.

	};

};



#endif

