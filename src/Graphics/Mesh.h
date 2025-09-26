#pragma once

#ifndef MESH_H
#define MESH_H

#include "../Util/Message.h"
#include "Texture.h"
#include "AttributeConfig.h"
#include "../Util/Util.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <vector>
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
		Mesh(std::vector<float> extern_vertices, std::vector<unsigned int> extern_indices);
		Mesh(std::vector<float> extern_vertices);
		~Mesh();

		//Config Process
		// TODO: Move the shader functionality to a separate class, totally five functions to be moved
		void SetVertexShaderSource     (const std::string& source);
		void SetFragmentShaderSource   (const std::string& source);
		void SetVertexShaderFilePath   (const std::string& source);
		void SetFragmentShaderFilePath (const std::string& source);
		void BuildProgram();
		// END TODO:

		void SetVBOAccessMode  (BufferAccessModes mode);
		void SetEBOAccessMode  (BufferAccessModes mode);

		AttributeConfig& SetAttribPointer(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos);
		//

		Texture& AddTexture(unsigned int id, const std::string path, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);
		Texture& AddTexture(unsigned int id, unsigned char* texture_data, int texture_width, int texture_height, GLenum Type, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER); // Add texture but with manual loaded texture_data

		void GenObject();
		void Finish(); // Everything is finished you can't change the settings of the mesh anymore
		void UpdateMvpLocations();
		bool UseTexture (unsigned int id); //TODO: This is Bind, not Set, should be renamed | Done - ScriptCodex13

		std::vector<float>& GetVertices();
		std::vector<unsigned int>& GetIndices();
		
		//TODO: Never return reference in a getter
		//      There are explicit coding conventions for getters and setters
		//      Getters return by value, setters set the value
		void UpdateModel(); // For use with camera
		void SetProjection(glm::mat4 proj);
		void SetView(glm::mat4 view);
		void UpdateModel(glm::mat4 mdl);
		glm::mat4 GetModel() const;
		void SetModel(const glm::mat4 &);

		unsigned int GetVBO() const;
		void SetVBO(unsigned int&);
		unsigned int GetVAO() const;
		void SetVAO(unsigned int&);
		unsigned int GetEBO() const;
		void SetEBO(unsigned int&);

		//separate functions will be very useful in more complex logic
		unsigned int GetUniformLocation(const GLchar* name);
		unsigned int GetProgram ();
		void Bind       ();
		void Draw       (int vertices_count = 0);
		// Transform func

	    // private functions for intern use 
		void UseProgram();
		void BindVAO();

	protected:
		struct
		{
			unsigned int view_location = -1;
			unsigned int projection_location = -1;
			unsigned int model_location = -1;
		} mvp;
	private:
		glm::mat4 model; // PR

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

		//Textures
		std::vector<Texture> texturecfg; // Needed to use shared_ptr because otherwise the Program crashes. Please do not change. If that here is very problematic we can find out a solution together.

	};

};



#endif

