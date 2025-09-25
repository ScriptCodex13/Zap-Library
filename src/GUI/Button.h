#pragma once 

#ifndef BUTTON_H
#define BUTTON_H

#include <memory>
#include <string>
#include <vector>

#include "../Graphics/Mesh.h"
#include "../Window/Window.h"

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

		void SetPosition();
		void SetButtonText();
		void SetTextOffset();
		void SetZCoordinate(); // Don't know if you need it

		void Update();
		void Draw();

	private:
		std::array<float, 4> bounds; // x_min, x_max, y_min, y_max

		std::vector<unsigned int> i_button_indices = 
		{
			2, 1, 0,
			0, 3, 2
		};

		const char* i_vertex_shader_source = 
			R"glsl(
					#version 330 core 
					
					layout(location = 0) in vec3 aPos;

					void main()
					{
						gl_Position = vec4(aPos, 1.0);
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