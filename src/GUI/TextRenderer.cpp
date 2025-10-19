#include "TextRenderer.h"
#include <fstream>

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <vector>

namespace zap
{

	Text::Text(const std::string font_path, const std::string content, const std::array<int, 2>& window_size) : Mesh()
	{
		projection = glm::ortho(0.0f, (float)window_size[0], 0.0f, (float)window_size[1]);

		i_font_path = font_path;
		i_content = content;

		e_window_size = window_size;

		// i_text_mesh init

		SetVertexShaderSource(i_vertex_shader_source);
		SetFragmentShaderSource(i_fragment_shader_source);
		//
		AttributeConfig& config = SetAttribPointer(0, 4, 4, 0);
		//
		Finish();
		UseProgram();

		i_text_color_uniform_location = glGetUniformLocation(GetProgram(), "textColor");
		i_text_projection_uniform_location = glGetUniformLocation(GetProgram(), "projection");

		glUniformMatrix4fv(i_text_projection_uniform_location, 1, GL_FALSE, glm::value_ptr(projection));

		//

		if (FT_Init_FreeType(&ft_instance))
		{
			messages::PrintMessage("Failed to initialze the FreeType library", "TextRenderer.cpp/ zap::Text::Text(...)", MessageTypes::fatal_error);
			ZAP_INTERRUPT_FATAL_ERROR;
		}

		if (FT_New_Face(ft_instance, i_font_path.c_str(), 0, &i_font))
		{
			messages::PrintMessage("Failed to load font from " + i_font_path, "TextRenderer.cpp/ zap::Text::Text(...)", MessageTypes::error);
		}
		else
		{
			GenerateCharacters();
		}

		// Do the steps of Mesh::Finish manually

		glBindBuffer(GL_ARRAY_BUFFER, GetVBO());
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		//presets
		SetColor(0.0f, 1.0f, 0.0f);
		SetPosition(500.0f, 500.0f);
	}

	Text::~Text()
	{
		FT_Done_Face(i_font);
		FT_Done_FreeType(ft_instance);
	}

	void Text::GenerateCharacters()
	{
		ClearAllTextures();

        FT_Set_Pixel_Sizes(i_font, 0, i_character_size); // Or i carracter size

		GLint currentAlignment;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &currentAlignment);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//restore alignment, so it remains local and doesn't affect global state:
		util::scope_guard restoreAlignment([currentAlignment]() {glPixelStorei(GL_UNPACK_ALIGNMENT, currentAlignment); });

		//texturecfg.reserve(128);
        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(i_font, c, FT_LOAD_RENDER))
            {
				messages::PrintMessage("Failed to load glyph", "TextRenderer.cpp/ void zap::Text::GenerateCharacters(...)", MessageTypes::error);
			}

			Texture& tex = AddTextureFromData((unsigned int)c,
				i_font->glyph->bitmap.buffer,
				i_font->glyph->bitmap.width, i_font->glyph->bitmap.rows,  GL_RED, 
				i_texture_filter, i_mipmap_setting,
				zap::TextureWrapping::CLAMP_TO_EDGE);

			Character character =
			{
				tex.getID(),
				glm::ivec2(i_font->glyph->bitmap.width, i_font->glyph->bitmap.rows), //Size
				glm::ivec2(i_font->glyph->bitmap_left,  i_font->glyph->bitmap_top),  //Bearing //ypos = y - (ch.Size.y - ch.Bearing.y)
				(unsigned int)i_font->glyph->advance.x
            };

            Characters.insert(std::pair<char, Character>(c, character));
        }

		glBindTexture(GL_TEXTURE_2D, 0);
		
	}

	void Text::SetContent(const std::string new_content)
	{
		i_content = new_content;
	}

	void Text::SetCharacterSize(unsigned int new_size_pixel)
	{
		i_character_size = new_size_pixel;
	}

	void Text::SetPosition(float new_x, float new_y)
	{
		//ToDo: Flip the y axis so the orign (0,0) is in the top left corner of the window 

		pos_x = new_x;
		pos_y = new_y;
	}

	void Text::SetColor(TextColors color)
	{
		switch (color)
		{
		case TextColors::WHITE:
			i_text_color = glm::vec3(1.0f, 1.0f, 1.0f);
			return;
		case TextColors::BLACK:
			i_text_color = glm::vec3(0.0f, 0.0f, 0.0f);
			return;
		case TextColors::RED:
			i_text_color = glm::vec3(0.8f, 0.0f, 0.0f);
			return;
		case TextColors::GREEN:
			i_text_color = glm::vec3(0.0f, 0.8f, 0.0f);
			return;
		case TextColors::BLUE:
			i_text_color = glm::vec3(0.0f, 0.0f, 0.8f);
			return;
		case TextColors::YELLOW:
			i_text_color = glm::vec3(1.0f, 0.9f, 0.0f);
			return;
		case TextColors::ORANGE:
			i_text_color = glm::vec3(1.0f, 0.7f, 0.0f);
			return;
		case TextColors::PURPLE:
			i_text_color = glm::vec3(0.8f, 0.0f, 1.0f);
			return;
		case TextColors::PINK:
			i_text_color = glm::vec3(1.0f, 0.0f, 0.5f);
			return;
		}
	}

	void Text::SetColor(float RED, float GREEN, float BLUE)
	{
		RED = std::clamp(RED, 0.0f, 1.0f);
		GREEN = std::clamp(GREEN, 0.0f, 1.0f);
		BLUE = std::clamp(BLUE, 0.0f, 1.0f);

		i_text_color = glm::vec3(RED, GREEN, BLUE);
	}

	void Text::SetScale(float scale_x, float scale_y)
	{
		i_scale_x = scale_x;
		i_scale_y = scale_y;
	}

	void Text::EnableRequriedOpenGLSettings()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Text::SetTextureFilter(zap::TextureFilter filter)
	{
		i_texture_filter = filter;
	}

	void Text::SetMipmapSettings(zap::MipmapSetting setting)
	{
		i_mipmap_setting = setting;
	}

	void Text::Draw()
	{
		Bind();
		glUniform3f(i_text_color_uniform_location, i_text_color.x, i_text_color.y, i_text_color.z);
		glActiveTexture(GL_TEXTURE0);

		std::string::const_iterator c;

		float x = pos_x;
		float y = e_window_size.at(1) - pos_y;

		for (c = i_content.begin(); c != i_content.end(); c++)
		{
			Character ch = Characters[*c];

			float xpos = x + ch.Bearing.x * i_scale_x;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * i_scale_y;

			float w = ch.Size.x * i_scale_x;
			float h = ch.Size.y * i_scale_y;
			// update VBO for each character
			float vertices[6][4] = 
			{
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			
			//Change


			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, GetVBO());
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			//glDrawArrays(GL_TRIANGLES, 0, 6);

			Mesh::Draw(6);

			//
			x += (ch.Advance >> 6) * i_scale_x;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//TODO: To implement some dynamic approach. The implemented way is fast enough but is too direct.
	void TextureText::drawGlythBitmap(FT_Face ftface, util::buffer_view2D<unsigned char> target_view, size_t& pen_x, size_t& pen_y, wchar_t c, size_t bufsize, unsigned int fontSizeFT)
	{
		FT_Load_Char(ftface, c, FT_LOAD_RENDER);

		FT_GlyphSlot& glyph = ftface->glyph;
		FT_Bitmap& btm = ftface->glyph->bitmap;
		FT_Bitmap& pbtm = glyph->bitmap;
		int ytop = pen_y + (glyph->metrics.vertBearingY >> 6); //Works for MSGothic, doesn't work for Arial

		//Works for Arial, doesn't work for MSGothic
		if (!FT_HAS_VERTICAL(freetype.getFace()))
		{
			int descent = fontSizeFT - glyph->bitmap_top + (ftface->bbox.yMin >> 6);
			ytop = pen_y + descent;
		}

		util::buffer_view2D<unsigned char> char_buf_view (pbtm.buffer, pbtm.width);
		for (size_t i = 0, xi = pen_x; i < pbtm.width; i++, xi++)
		{
			for (size_t j = 0; j < pbtm.rows; j++)
			{
				char pixel_val = char_buf_view[j][i];
				//assert(pos < bufsize);
				target_view[ytop + j][pen_x + i] = pixel_val;
			}
		}

		pen_x += glyph->advance.x >> 6;
	}

	void TextureText::drawString3TIntoBitman(FT_Face ftface, util::buffer_view2D<unsigned char> buf, const wchar_t* str, size_t& outer_width, size_t& pen_y, size_t bufsize, unsigned int fontSizeFT)
	{
		for (int i = 0; i < wcslen(str); i++) //wcslen(str) / 40
			drawGlythBitmap(ftface, buf, outer_width, pen_y, str[i], bufsize, fontSizeFT);
	}


	void TextureText::drawString3TIntoBitmap (const wchar_t* str, unsigned int fontSizeFT, size_t& outer_width, size_t& pen_y)
	{
		size_t src_width = util::align<4>(fontSizeFT * wcslen(str) * 2); //make it twice as wide as widest possible two chars sequence
		size_t src_size = fontSizeFT * src_width; //2D array, fontSizeFT rows, tg_width columns

		//never schrink
		util::vector_realloc<unsigned char>(texture_data_source, src_size, 0x00);

		size_t tg_width = 0, tg_height = 0;
		FT_Set_Pixel_Sizes(freetype.getFace (), 0, fontSizeFT);
		drawString3TIntoBitman(freetype.getFace(), util::buffer_view2D(texture_data_source.data(), src_width), str, tg_width, tg_height, src_size, fontSizeFT);

		tg_width = util::align<4>(tg_width); //realign to 32bit
		size_t tg_size = tg_width * fontSizeFT; //2D array, fontSizeFT rows, tg_width columns
		util::vector_realloc<unsigned char> (texture_data_target, tg_size, 0x00);

		unsigned char* psrc = texture_data_source.data(), * tsrc = texture_data_target.data();
		util::buffer_view2D src_view = util::buffer_view2D(texture_data_source.data(), src_width), 
			tg_view = util::buffer_view2D(texture_data_target.data(), tg_width);
		tg_view.reverse_rows_read (src_view, fontSizeFT); //target reads fontSizeFT lines from source

		outer_width += tg_width;
	}

	TextureText::TextureText()
	{
	}
	TextureText::TextureText(std::string font_path)
	{
		LoadFont(font_path);
	}
	void TextureText::LoadFont(std::string font_path) {
		freetype.LoadFont(font_path);
	}

	////each line[] has cols length
	//void dumpTextureBuffer(const unsigned char* buf, size_t cols, size_t lines) //cols ie:x/width/    lines ie:y/heigth
	//{
	//	////dump texture to file;
	//	std::ofstream dump_content;
	//	dump_content.open("dump_texture.txt");
	//	util::buffer_view2D dumper_view(buf, cols);
	//	for (int j = 0; j < lines; j++)
	//	{
	//		for (size_t i = 0; i < dumper_view.get_width(); i++)
	//			dump_content << util::decode<unsigned char>(dumper_view[j][i], 0, ' ', 'W');
	//		dump_content << std::endl;
	//	}
	//	dump_content << std::flush;
	//	dump_content.close();
	//}

	//this is central
	//Texture& TextureText::print(Texture& texture, const std::wstring content)
	Texture& TextureText::print(Texture& texture, wchar_t* const content)
	{
		size_t width = 0, height = 0;
		//drawString3TIntoBitmap(content.c_str(), fontSize, width, height);
		drawString3TIntoBitmap(content, fontSize, width, height);
		//dumpTextureBuffer(texture_data_target.data(), width, fontSize);

		GLint currentAlignment;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &currentAlignment);
		//4 is the default for GL_UNPACK_ALIGNMENT, probably for a good reason
		//Make sure it is not changed somewhere, so different parts of program do not interfere here
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		//restore to what it was on scope exit:
		util::scope_guard restoreAlignment([currentAlignment]() {glPixelStorei(GL_UNPACK_ALIGNMENT, currentAlignment); });

		texture.flushData(width, fontSize, texture_data_target.data());
		return texture;
	
	}
	//uses printf_t, printf_t calls print
	size_t TextureText::printf(Texture& texture, wchar_t const* const  format, ...)
	{
		//evaluate size first
		va_list arglist;
		va_start(arglist, format);
		size_t result = _vsnwprintf(nullptr, 0, format, arglist);
		va_end(arglist);

		//Assure that buffer never schrinks, so reduce the number of allocations
		util::vector_realloc<wchar_t>(wprintf_buffer, result + 1);

		va_start(arglist, format);
		result = printf_t(texture, wprintf_buffer.data(), wprintf_buffer.size() + 1, format, arglist);
		va_end(arglist);
		return result;
	}

	size_t TextureText::printf_t(Texture& texture, wchar_t* const buffer_out, size_t const buffer_size, wchar_t const* const format, va_list arglist)
	{
		size_t retval = _vswprintf_s_l(buffer_out, buffer_size, format, NULL, arglist);
		print(texture, buffer_out);
		return retval;
	}

}