#pragma once 

#ifndef BUTTON_H
#define BUTTON_H

#include <memory>
#include <string>
#include <vector>

#include "../Graphics/Mesh.h"
#include "../Window/Window.h"

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

		bool Hovered();

		bool Pressed(zap::Key key); // Returns true if button is pressed and false if not
		bool Released(zap::Key key);

		void SetPosition(float x, float y); // Sets the top left point to the given coordinates
		void SetButtonText();
		void SetTextOffset();
		void SetZCoordinate(); // Don't know if you need it

		void Update();
		void Draw();

	private:
		unsigned int i_bounds_uniform_location;

		std::array<float, 4> i_bounds; // x_min, x_max, y_min, y_max
		std::array<float, 4> i_scale;
		std::array<float, 2> i_pos; // top left corner of the button

		glm::mat4 i_position_transform;

		std::vector<unsigned int> i_button_indices = 
		{
			2, 1, 0,
			0, 3, 2
		};

		const char* i_vertex_shader_source = 
			R"glsl(
					#version 330 core 
					
					layout(location = 0) in vec3 aPos;

					uniform mat4 bounds;
					
					void main()
					{
						gl_Position = bounds * vec4(aPos, 1.0);
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