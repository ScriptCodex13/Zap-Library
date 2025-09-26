#include "TextRenderer.h"

namespace zap
{

	Text::Text(const std::string font_path, const std::string content, std::array<int, 2>& window_size) : Mesh({})
	{
		projection = glm::ortho(0.0f, (float)window_size[0], 0.0f, (float)window_size[1]);

		i_font_path = font_path;
		i_content = content;

		e_window_size = &window_size;

		// i_text_mesh init


		SetVertexShaderSource(i_vertex_shader_source);
		SetFragmentShaderSource(i_fragment_shader_source);
		//
		auto config = SetAttribPointer(0, 4, 4, 0);
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

		//config.vertexAttribPointer();

		//
	}

	Text::~Text()
	{
		FT_Done_Face(i_font);
		FT_Done_FreeType(ft_instance);
	}

	void Text::GenerateCharacters()
	{
        FT_Set_Pixel_Sizes(i_font, 0, i_character_size); // Or i carracter size

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(i_font, c, FT_LOAD_RENDER))
            {
				messages::PrintMessage("Failed to load glyph", "TextRenderer.cpp/ void zap::Text::GenerateCharacters(...)", MessageTypes::error);
			}
            
			Texture& tex = AddTexture((unsigned int)c, i_font->glyph->bitmap.buffer, i_font->glyph->bitmap.width, i_font->glyph->bitmap.rows, GL_RED, i_texture_filter, i_mipmap_setting, zap::TextureWrapping::CLAMP_TO_EDGE);

			tex.genTexture();
		
			Character character =
			{
				tex.getID(),
				glm::ivec2(i_font->glyph->bitmap.width, i_font->glyph->bitmap.rows),
				glm::ivec2(i_font->glyph->bitmap_left, i_font->glyph->bitmap_top),
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
		GenerateCharacters(); // Regenerate the characters with the new character size
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

	void Text::SetTextureFilter(zap::TextureFilters filter)
	{
		i_texture_filter = filter;

		GenerateCharacters();
	}

	void Text::SetMipmapSettings(zap::MipmapSettings setting)
	{
		i_mipmap_setting = setting;

		GenerateCharacters();
	}

	void Text::Draw()
	{
		Bind();
		glUniform3f(i_text_color_uniform_location, i_text_color.x, i_text_color.y, i_text_color.z);
		glActiveTexture(GL_TEXTURE0);

		std::string::const_iterator c;

		float x = pos_x;
		float y = e_window_size->at(1) - pos_y;

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
}