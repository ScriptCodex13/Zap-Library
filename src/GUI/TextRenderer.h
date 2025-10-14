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

	class Text : protected zap::Mesh
	{
	public:
		Text(const std::string font_path, const std::string content, const std::array<int, 2>& window_size); // ToDo: Add function to Update the window size var if you don't provide reference
		~Text();

		void SetContent(const std::string new_content);
		void SetCharacterSize(unsigned int new_size_pixel);
		void SetPosition(float new_x, float new_y);
		void SetColor(TextColors color);
		void SetColor(float RED, float GREEN, float BLUE);
		void SetScale(float scale_x, float scale_y);

		void EnableRequriedOpenGLSettings();

		void SetTextureFilter(zap::TextureFilters filter);
		void SetMipmapSettings(zap::MipmapSettings setting);

		void Draw();
		void GenerateCharacters();

	private:
		zap::TextureFilters i_texture_filter = zap::TextureFilters::LINEAR;
		zap::MipmapSettings i_mipmap_setting = zap::MipmapSettings::LINEAR_MIPMAP_NEAREST;

		std::string i_content;
		std::string i_font_path;

		unsigned int i_character_size = 24;

		std::array<int, 2> e_window_size;

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
	//This is a RAII
	class FreeType
	{
		FT_Library ft_instance = nullptr;
		FT_Face i_font = nullptr;
	public:
		FreeType() {}
		~FreeType() { free(); }
		void free() //to call outside destructor if needed
		{
			if (i_font) FT_Done_Face(i_font);
			if (ft_instance) FT_Done_FreeType(ft_instance);
			ft_instance = nullptr;
			i_font = nullptr;
		}

		FT_Library getLibrary() { return ft_instance; }
		FT_Face getFace() { return i_font; }
		void LoadFont(const std::string font_path) {
			FT_Init_FreeType(&ft_instance);
			FT_New_Face(ft_instance, font_path.c_str(), 0, &i_font);
		}
		FT_Error loadChar (FT_Face face, wchar_t c)
		{
			return FT_Load_Char(face, c, FT_LOAD_RENDER);
			//FT_New_Face(ft_instance, "C:/Windows/Fonts/arial.ttf", 0, &i_font);
			//pface = std::make_unique<FT_Face, FT_Done_Face>(i_font);
			//uptr = std::make_unique < FT_Face, std::function<FT_Done_Face> > (i_font);
		}
	};

	//texture bitmap buffer generator for texture text
	//TODO: redesign, it is too tied to Mesh and Texture
	class TextureText
	{
		FreeType freetype;
		unsigned int fontSize = 48; //default font size

		std::vector<unsigned char> texture_data_target;
		std::vector<unsigned char> texture_data_source;


		std::vector<wchar_t> wprintf_buffer;

		void drawString3TIntoBitman(FT_Face ftface, util::buffer_view2D<unsigned char> buf, const wchar_t* str, int& outer_width, int& pen_y, size_t bufsize);
		void drawGlythBitmap(FT_Face ftface, util::buffer_view2D<unsigned char> target_view, int& pen_x, int& pen_y, wchar_t c, size_t bufsize);
		void drawString3TIntoBitmap(const wchar_t* str, unsigned int fontSizeFT, int& outer_width, int& pen_y);

	public:
		TextureText();
		TextureText(const std::string font_path);
		void SetFontSize(unsigned int _fontSize) { fontSize = _fontSize; } //set only once
		void LoadFont(const std::string font_path);
		Texture& print(zap::Mesh* pMesh, unsigned int hash, const std::wstring content);
		void printf(zap::Mesh* pMesh, unsigned int hash, const std::wstring content, ...);
		int TextureText::printf_t(zap::Mesh* pMesh, unsigned int hash, wchar_t* const _Buffer, size_t const _BufferCount, wchar_t const* const _Format, va_list _ArgList);

		Texture& ApplyTextureTo(zap::Mesh* pMesh, const std::wstring content);

	};
}

#endif