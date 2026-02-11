#pragma once

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

// ToDo make a GUI part of the library later on and move it there

// includes

#include "../Graphics/Mesh.h"
#include "../Graphics/Texture.h"
#include "../Util/Message.h"
#include "../Util/Util.h"

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

		void SetTextureFilter(zap::TextureFilter filter);
		void SetMipmapSettings(zap::MipmapSetting setting);

		void Draw();
	private:
		void GenerateCharacters();

	private:
		zap::TextureFilter i_texture_filter = zap::TextureFilter::LINEAR;
		zap::MipmapSetting i_mipmap_setting = zap::MipmapSetting::LINEAR_MIPMAP_NEAREST;

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

		FT_Library getLibrary() const { return ft_instance; }
		FT_Face getFace() const { return i_font; }
		void LoadFont(const std::string font_path) {
			FT_Init_FreeType(&ft_instance);
			FT_New_Face(ft_instance, font_path.c_str(), 0, &i_font);
		}
	};

	//texture bitmap buffer generator for texture text
	//TODO: redesign, it is too tied to Mesh and Texture
	class TextureText
	{
		FreeType freetype;
		unsigned int fontSize = 48; //default font size

		//info for caching
		//caching chars greatly reduce font data readings
		//    number of font data readings rapidly approaches to 0
		//    and practially in most cases drops instantly to 0
		class CharInfo
		{
		public:
			size_t width = 0, height = 0, top = 0, advance_x = 0;
			std::vector <unsigned char> buffer{};
		public:
			CharInfo() {}
			CharInfo(size_t _width, size_t _height, size_t _top, size_t _advance_x, unsigned char* buf):
				width(_width), height(_height), top(_top), advance_x(_advance_x)
			{
				buffer.resize(width * height);
				memcpy(buffer.data(), buf, width * height);
			}
		};
		//cache
		std::map<wchar_t, CharInfo> cachedChars;

		//Internal buffers helping to minimize number of allocations/reallocations/relocations
		//These are grow only buffers
		//texture buffers
		std::vector<unsigned char> texture_data_target; //used for draw draw text pixel data
		std::vector<unsigned char> texture_data_source; //used to collect text pixed data
		//format storage routines buffer
		std::vector<wchar_t> wprintf_buffer; //used for whide char wchar_t sprintf routines

		//caching chars greatly reduce font data readings
		//    number of font data readings rapidly approaches to 0
		//    and practially in most cases drops instantly to 0
		//    Experimentally proven. It does almost nothing during execution, even in case of intensive text update
		bool drawGlythBitmapFromCache(util::buffer_view2D<unsigned char> target_view, size_t& pen_x, size_t& pen_y, wchar_t c, size_t bufsize);

		void drawString3TIntoBitman(util::buffer_view2D<unsigned char> buf, const wchar_t* str, size_t& outer_width, size_t& pen_y, size_t bufsize);
		void drawGlythBitmap(util::buffer_view2D<unsigned char> target_view, size_t& pen_x, size_t& pen_y, wchar_t c, size_t bufsize);
		void drawString3TIntoBitmap(const wchar_t* str,  size_t& outer_width, size_t& pen_y);

	public:
		TextureText();
		TextureText(std::string font_path);
		//set only once
		void SetFontSize(unsigned int _fontSize) { FT_Set_Pixel_Sizes(freetype.getFace(), 0, fontSize); fontSize = _fontSize; }
		void LoadFont(std::string font_path);
		Texture& print (Texture& texture, wchar_t* const content);
		size_t printf     (Texture& texture, wchar_t const* const  format, ...);
		size_t printf_t   (Texture& texture, wchar_t* const _Buffer, size_t const _BufferCount, wchar_t const* const  format, va_list arglist);

	};
}

#endif