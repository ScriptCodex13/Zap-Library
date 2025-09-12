#pragma once

#ifndef MESH_H
#define MESH_H

#include "Message.h"
#include "Texture.h"
#include "AttributeConfig.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <vector>
#include <string>


namespace zap
{
	enum class BufferAccessModes : GLenum
	{
		LOW_ACCESS_STATIC   = GL_STREAM_DRAW,
		HIGH_ACESS_STATIC   = GL_STATIC_DRAW,
		HIGH_ACESS_DYNAMIC  = GL_DYNAMIC_DRAW
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

	class Mesh
	{
	public:
		Mesh(std::vector<float> extern_vertices, std::vector<unsigned int> extern_indices);
		Mesh(std::vector<float> extern_vertices);
		~Mesh();

		//Config Process

		void SetVertexShaderSource   (const std::string& source);
		void SetFragmentShaderSource (const std::string& source);

		void SetVBOAccessMode  (BufferAccessModes mode);
		void SetEBOAccessMode  (BufferAccessModes mode);

		AttributeConfig& SetAttribPointer(int shader_location, int value_ct, unsigned int data_stride, unsigned int start_pos);
		//

		Texture& AddTexture(unsigned int id, const std::string path, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping i_wrapping = TextureWrapping::CLAMP_TO_BORDER);

		void BuildProgram();
		void GenObject();
		void Finish(); // Everything is finished you can't change the settings of the mesh anymore
		bool UseTexture (unsigned int id); //TODO: This is Bind, not Set, should be renamed | Done - ScriptCodex13

		std::vector<float>& GetVertices();
		std::vector<unsigned int>& GetIndices();
		
		void UpdateModel(unsigned int model_uniform_location); // For use with camera
		glm::mat4& GetModel();

		//separate functions will be very useful in more complex logic
		unsigned int GetUniformLocation(const GLchar* name);
		unsigned int GetProgram ();
		void Bind       ();
		void Draw       (int vertices_count = 0);
		// Transform func

	    // private functions for intern use 
		void UseProgram();
		void BindVAO();

	private:
		glm::mat4 model; // PR

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

		bool use_indices = true;
		
	};

};



#endif

