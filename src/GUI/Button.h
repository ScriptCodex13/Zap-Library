#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include <memory>
#include <string>
#include <vector>

#include "../Graphics/Mesh.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "TextRenderer.h"

namespace zap
{
	class Button : zap::Mesh
	{

	public:
		Button(zap::Window& window, const std::string button_text = "", const std::string button_text_font_path = "");
		Button(zap::Window& window, const std::array<float, 4>& bounds, const std::string button_text = "", const std::string button_text_font_path = "");
		
		~Button();
		
		int LoadTexture(unsigned int id, const std::string path_to_texture, zap::TextureFilters filter, int shader_location = 1);
		void FinishMesh();
		void UseTextureShaders(bool state);
		void UseTextureShaders(const char* vertex_shader_source, const char* fragment_shader_source);

		zap::Text* GetTextObject();
		void UpdatePosition();
		bool Hovered();

		bool Pressed(zap::Key key); // Returns true if button is pressed and false if not
		bool Released(zap::Key key);

		void SetGlSize(float width, float height);
		void SetGlSize(std::array<float, 2>& width_height);
		void SetGlWidth(float width);
		void SetGlHeight(float height);
		void SetGlPosition(float xmin, float ymin); // Equivalent to move window to XY pos. Sets the xmin/ymin = bottom/left point to the given coordinates
		void SetGlPosition(std::array<float, 2>& gl_xy_min); // Equivalent to move window to XY pos. Sets the top left point to the given coordinates
		void SetGlPosition(float xmin, float ymin, float xmax, float ymax); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetGlPosition(std::array<float, 2>& gl_xy_min, std::array<float, 2>& gl_xy_max); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetGlPosition(std::array<float, 4>& gl_xy_min_xy_max); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetColor(float RED, float GREEN, float BLUE, float ALPHA);
		
		void ActivateTexture(bool state);
		void UseText(bool state);
		void SetButtonText(const std::string text);
		void SetTextOffset(float x_offset, float y_offset);
		void SetTextColor(zap::TextColors color);
		void SetTextColor(float RED, float GREEN, float BLUE);
		void SetZCoordinate(); // Don't know if you need it

		void Update();
		void Draw(int texture_id = 0);

	private: // Private functions
		//void SetVertexShaderSource(const char* source);
		//void SetFragmentShaderSource(const char* source);

	private:
		bool i_use_texture = false;
		bool texture_attribute_ptr_set = false;
		bool i_use_text = true;
		bool i_font_missing_flag = false;

		bool i_use_text_offset = false;

		std::unique_ptr<zap::Text> i_button_text;

		std::array<float, 2> i_text_offset = { 0.0f, 0.0f };

		glm::vec4 i_button_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	protected:
		unsigned int i_moveto_uniform_location;
		unsigned int i_button_color_location;

		std::array<float, 4> i_bounds; // x_min, x_max, y_min, y_max

		const char* i_vertex_shader_source =
			R"glsl(
					#version 330 core

					layout(location = 0) in vec3 aPos;
					uniform mat4 moveto;

					void main()
					{
						gl_Position = moveto * vec4(aPos, 1.0);
					}
			)glsl";

		const char* i_fragment_shader_source =
			R"glsl(
					#version 330 core

					out vec4 fragColor;

					uniform vec4 button_color;

					void main()
					{
						fragColor = button_color;
					}
			)glsl";

		const char* i_vertex_shader_source_t =
			R"glsl(
					#version 330 core

					layout(location = 0) in vec3 aPos;
					layout(location = 1) in vec2 aTexCoord;

					uniform mat4 moveto;

					out vec2 TexCoord;

					void main()
					{
						gl_Position = moveto * vec4(aPos, 1.0);
						TexCoord = aTexCoord;
					}
			)glsl";

		const char* i_fragment_shader_source_t =
			R"glsl(
					#version 330 core
					out vec4 FragColor;

					in vec2 TexCoord;

					uniform vec4 button_color;

					uniform sampler2D texture1;

					void main()
					{
						FragColor = button_color * texture(texture1, TexCoord);
					}
			)glsl";

		zap::Window* e_window;
	};

	class ButtonText : public zap::Mesh
	{

	public:
		ButtonText(zap::Window& window, const std::string button_text = "", const std::string button_text_font_path = "");
		ButtonText(zap::Window& window, const std::array<float, 4>& bounds, const std::string button_text = "", const std::string button_text_font_path = "");

		~ButtonText();

		int LoadTexture(unsigned int id, const std::string path_to_texture, zap::TextureFilters filter, int shader_location = 1);
		void FinishMesh();
		void UseTextureShaders(bool state);
		void UseTextureShaders(const char* vertex_shader_source, const char* fragment_shader_source);

		zap::Text* GetTextObject();
		void UpdatePosition();
		bool Hovered();

		bool Pressed(zap::Key key); // Returns true if button is pressed and false if not
		bool Released(zap::Key key);

		void SetGlSize(float width, float height);
		void SetGlSize(std::array<float, 2>& width_height);
		void SetGlWidth(float width);
		void SetGlHeight(float height);
		void SetGlPosition(float xmin, float ymin); // Equivalent to move window to XY pos. Sets the xmin/ymin = bottom/left point to the given coordinates
		void SetGlPosition(std::array<float, 2>& gl_xy_min); // Equivalent to move window to XY pos. Sets the top left point to the given coordinates
		void SetGlPosition(float xmin, float ymin, float xmax, float ymax); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetGlPosition(std::array<float, 2>& gl_xy_min, std::array<float, 2>& gl_xy_max); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetGlPosition(std::array<float, 4>& gl_xy_min_xy_max); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetColor(float RED, float GREEN, float BLUE, float ALPHA);

		void ActivateTexture(bool state);
		void UseText(bool state);
		void SetButtonText(const std::string text);
		void SetTextOffset(float x_offset, float y_offset);
		void SetTextColor(zap::TextColors color);
		void SetTextColor(float RED, float GREEN, float BLUE);
		void SetZCoordinate(); // Don't know if you need it

		void Update();
		void Draw(int texture_id = 0);

	private: // Private functions
		//void SetVertexShaderSource(const char* source);
		//void SetFragmentShaderSource(const char* source);

	private:
		//bool i_use_texture = false;
		//bool texture_attribute_ptr_set = false;
		//bool i_use_text = true;
		//bool i_font_missing_flag = false;

		//bool i_use_text_offset = false;


		//std::array<float, 2> i_text_offset = { 0.0f, 0.0f };

		glm::vec4 i_button_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	protected:
		unsigned int i_moveto_uniform_location;
		unsigned int i_button_color_location;

		std::array<float, 4> i_bounds; // x_min, x_max, y_min, y_max

		const char* i_vertex_shader_source =
			R"glsl(
					#version 330 core

					layout(location = 0) in vec3 aPos;
					layout(location = 1) in vec2 aTexCoord;

					uniform mat4 moveto;
					out vec2 TexCoord;
					void main()
					{
						gl_Position = moveto * vec4(aPos, 1.0);
						TexCoord = aTexCoord;
					}
			)glsl";

		const char* i_fragment_shader_source =
			R"glsl(
					#version 330 core

					out vec4 FragColor;
					in vec2 TexCoord;

					uniform sampler2D texture1;
					uniform vec4 button_color;

					void main()
					{
						//fragColor = button_color;
						FragColor = button_color * texture(texture1, TexCoord);
					}
			)glsl";


		zap::Window* e_window;
	};

}

#endif