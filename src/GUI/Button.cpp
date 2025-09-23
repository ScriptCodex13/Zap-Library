#include "Button.h"

namespace zap
{
	Button::Button(zap::Window& window, const std::string button_text, const std::string button_text_font_path)
	{
		e_window = &window;

		//i_button_text = std::make_unique<zap::Text>(button_text_font_path, button_text, e_window->GetSizeRef());

		i_button_mesh = std::make_unique<zap::Mesh>(i_button_vertices, i_button_indices);
		
		i_button_mesh->SetVertexShaderSource(i_vertex_shader_source);
		i_button_mesh->SetFragmentShaderSource(i_fragment_shader_source);

		i_button_mesh->SetAttribPointer(0, 3, 3, 0);

		i_button_mesh->Finish();
	}

	Button::~Button()
	{

	}

	void Button::Update()
	{
		i_button_mesh->UseProgram();
	}

	void Button::Draw()
	{
		i_button_mesh->Bind();
		i_button_mesh->Draw();

		//i_button_text->Draw();
	}
}