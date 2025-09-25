#pragma once

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

// ToDo make a GUI part of the library later on and move it there

// includes

#include "../Graphics/Mesh.h"
#include "../Util/Message.h"
#include "../Util/Util.h"
#include "../Window/Window.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <string>
#include <array>
#include <filesystem>
#include <memory>
#include <algorithm>
#include <map>

//

// ToDo add function to load a font in the loader or here

namespace zap
{
	// Text class will be here

	enum class TextColors
	{
		WHITE,
		BLACK,
		RED,
		GREEN,
		BLUE,
		YELLOW,
		ORANGE,
		PURPLE,
		PINK

	};

	struct Character 
	{
		unsigned int TextureID;
		glm::ivec2   Size;      
		glm::ivec2   Bearing;   
		unsigned int Advance;   
	};

	class Text : public zap::Mesh
	{
	public:
		Text(const std::string font_path, const std::string content, std::array<int, 2>& window_size); // ToDo: Add function to Update the window size var if you don't provide reference
		~Text();

		void SetContent(const std::string new_content);
		void SetCharacterSize(unsigned int new_size_pixel);
		void SetPosition(float new_x, float new_y);
		void SetColor(TextColors color);
		void SetColor(float RED, float GREEN, float BLUE);
		void SetScale(float scale_x, float scale_y);

		void Draw();
		void GenerateCharacters();

	private:
		std::string i_content;
		std::string i_font_path;

		unsigned int i_character_size = 24;

		std::array<int, 2>* e_window_size;

		float i_scale_x = 1.0f;
		float i_scale_y = 1.0f;

		glm::mat4 projection;

		std::map<GLchar, Character> i_character_map;

		unsigned int i_text_color_uniform_location;
		unsigned int i_text_projection_uniform_location;

		std::map<GLchar, Character> Characters;

		glm::vec3 i_text_color = glm::vec3(1.0f, 0.0f, 0.0f);

		float pos_x = 10.0f;
		float pos_y = 10.0f;

	private:
		FT_Library ft_instance;
		FT_Face i_font;


	private:
		const char* i_fragment_shader_source =
			R"glsl(
				#version 330 core
				in vec2 TexCoords;
				out vec4 color;

				uniform sampler2D text;
				uniform vec3 textColor;

				void main()
				{    
					vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
					color = vec4(textColor, 1.0) * sampled;
				}
			)glsl";

		const char* i_vertex_shader_source =
			R"glsl(
				#version 330 core
				layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
				out vec2 TexCoords;

				uniform mat4 projection;

				void main()
				{
					gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
					TexCoords = vertex.zw;
				}
			)glsl";
	};
}

#endif