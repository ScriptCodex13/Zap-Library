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
	class Button
	{
	public:
		Button(zap::Window& window, const std::string button_text, const std::string button_font_path);
		~Button();

		bool OnMouseHovered();

		bool Released();
		bool Pressed();

		void SetTextOffset();

		void Update();
		void Draw();

	private:
		std::vector<float> i_button_vertices = 
		{

		};

		std::vector<unsigned int> i_button_indices = 
		{

		};

		const char* i_vertex_shader_source = 
			R"glsl(
					#version 330 core 
					
					layout(location = 0) vec3 aPos;

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