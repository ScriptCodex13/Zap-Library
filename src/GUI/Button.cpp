#include "Button.h"

namespace zap
{
	Button::Button(zap::Window& window, const std::string button_text, const std::string button_text_font_path) :
		Button(window, { -0.5, -0.5, 0.5, 0.5}, button_text, button_text_font_path)
	{

	}
	Button::Button(zap::Window& window, const std::array<float, 4>& _bounds, const std::string button_text, const std::string button_text_font_path):
		i_bounds(_bounds)
	{
		e_window = &window;
		std::vector<float> i_button_vertices = 
		{
			   0.5f,  0.5f, 0.1f,  // 0, 1, 2
			  0.5f, -0.5f, 0.1f,  // 3, 4, 5
			 -0.5f, -0.5f, 0.1f,  // 6, 7, 8
			 -0.5f,  0.5f, 0.1f   // 9, 10, 11
		};


		//i_button_text = std::make_unique<zap::Text>(button_text_font_path, button_text, e_window->GetSizeRef());

		i_button_mesh = std::make_unique<zap::Mesh>(i_button_vertices, i_button_indices);
		i_button_mesh->SetVertexShaderSource(i_vertex_shader_source);
		i_button_mesh->SetFragmentShaderSource(i_fragment_shader_source);

		i_button_mesh->SetAttribPointer(0, 3, 3, 0);

		i_button_mesh->Finish();

		i_bounds_uniform_location = glGetUniformLocation(i_button_mesh->GetProgram(), "bounds");
	}

	Button::~Button()
	{

	}

	bool Button::Hovered()
	{
		auto mouse_pos = zap::util::pixel_to_gl_coords(e_window->GetSize(), e_window->GetMousePosition());

		return zap::util::between(mouse_pos, i_bounds);
	}

	bool Button::Pressed(zap::Key key)
	{
		if (!Hovered()) return false;

		return e_window->GetInput(key, zap::State::PRESSED);
	}

	bool Button::Released(zap::Key key)
	{
		if (!Hovered()) return false;

		return e_window->GetInput(key, zap::State::RELEASED);
	}

	void Button::SetPosition(float x, float y)
	{
		i_pos = { x, y };

		/*i_bounds[0] = i_pos[0];
		i_bounds[1] += i_bounds[0] - i_pos[0];

		i_bounds[2] = i_pos[1];
		i_bounds[3] += i_bounds[2] - i_pos[1];*/

		i_position_transform = glm::translate(glm::mat4(1.0f), glm::vec3(i_pos[0], i_pos[1], 0.0f));
		
	}

	void Button::Update()
	{
		i_button_mesh->UseProgram();

		glUniformMatrix4fv(i_bounds_uniform_location, 1, GL_FALSE, glm::value_ptr(i_position_transform));
	}

	void Button::Draw()
	{
		i_button_mesh->Bind();
		i_button_mesh->Draw();

		//i_button_text->Draw();
	}
}