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
		void SetButtonText();
		void SetTextOffset();
		void SetZCoordinate(); // Don't know if you need it

		//void Update();
		void Draw();

	private:
		unsigned int i_size_uniform_location;
		unsigned int i_pos_uniform_location;

		std::array<float, 4> i_bounds; // x_min, x_max, y_min, y_max

		std::vector<unsigned int> i_button_indices = 
		{
			2, 1, 0,
			0, 3, 2
		};

		const char* i_vertex_shader_source = 
			R"glsl(
					#version 330 core 
					
					layout(location = 0) in vec3 aPos;
					uniform mat4 size;
					uniform mat4 pos;
					
					void main()
					{
						gl_Position = pos * size * vec4(aPos, 1.0);
						//gl_Position = vec4(aPos, 1.0);
					}
			)glsl";

		const char* i_fragment_shader_source = 
			R"glsl(
					#version 330 core

					out vec4 fragColor;

					void main()
					{
						fragColor = vec4(1.0);
					}
			)glsl";


		std::unique_ptr<zap::Text> i_button_text;
		std::unique_ptr<zap::Mesh> i_button_mesh;

		zap::Window* e_window;
	};
}

#endif