#include "Button.h"

namespace zap
{
	Button::Button(zap::Window& window, const std::string button_text, const std::string button_text_font_path) :
		Button(window, { -0.5, -0.5, 0.5, 0.5}, button_text, button_text_font_path)
	{

	}
	Button::Button(zap::Window& window, const std::array<float, 4>& _bounds, const std::string button_text, const std::string button_text_font_path):
		i_bounds(_bounds), zap::Mesh (
			{
				  1.0f,  1.0f, 0.1f,  // 0, 1, 2
				  1.0f,  0.0f, 0.1f,  // 3, 4, 5
				  0.0f,  0.0f, 0.1f,  // 6, 7, 8
				  0.0f,  1.0f, 0.1f   // 9, 10, 11
			},
			{
				2, 1, 0,
				0, 3, 2
			})
	{

		e_window = &window;

		if (button_text_font_path.empty())
		{
			i_use_text = false;
			i_font_missing_flag = true;
		}

		SetVertexShaderSource(i_vertex_shader_source);
		SetFragmentShaderSource(i_fragment_shader_source);

		SetAttribPointer(0, 3, 3, 0);

		Finish();

		i_moveto_uniform_location = glGetUniformLocation(GetProgram(), "moveto");

		UpdatePosition();

		if(i_use_text) i_button_text = std::make_unique<zap::Text>(button_text_font_path, button_text, e_window->GetSize());
	}


	Button::~Button()
	{

	}
	zap::Text* Button::GetTextObject()
	{
		return i_button_text.get();
	}
	void Button::UpdatePosition()
	{
		float x0 = i_bounds[0], dx = i_bounds[2] - i_bounds[0];
		float y0 = i_bounds[1], dy = i_bounds[3] - i_bounds[1];
		glm::mat4 resize = glm::scale(glm::mat4(1.0f), glm::vec3(dx, dy, 1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(x0, y0, 0.0f));


		Bind();
		// Premultiply in C++ and combine two matrices. Multiply order here is inverse than in shader
		glUniformMatrix4fv(i_moveto_uniform_location, 1, GL_FALSE, glm::value_ptr(translate * resize));

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

	void Button::SetGlSize(float dx, float dy) 
	{
		SetGlSize(std::array<float, 2> {dx, dy});
	}
	void Button::SetGlSize(std::array<float, 2>& dx_dy)
	{
		SetGlPosition(std::array<float, 4>{
			i_bounds[0],            // x_min
			i_bounds[1],            // y_min
			i_bounds[0] + dx_dy[0], // x_min + delta_x
			i_bounds[1] + dx_dy[1]  // y_min + delta_y
		});
	}
	void Button::SetGlWidth(float dx)
	{
		SetGlPosition(std::array<float, 4>{
			i_bounds[0],            // x_min
			i_bounds[1],            // y_min
			i_bounds[0] + dx,       // x_min + delta_x
			i_bounds[3]             // y_max
		});
	}
	void Button::SetGlHeight(float dy)
	{
		SetGlPosition(std::array<float, 4>{
			i_bounds[0],            // x_min
			i_bounds[1],            // y_min
			i_bounds[2],            // x_max
			i_bounds[1] + dy        // y_min + delta_y
		});
	}
	void Button::SetGlPosition(float xmin, float ymin)
	{
		SetGlPosition(std::array<float, 2> {xmin, ymin});
	}
	void Button::SetGlPosition(std::array<float, 2>& gl_xy_min)
	{
		SetGlPosition(std::array<float, 4>{
			gl_xy_min[0],                             // x_min
			gl_xy_min[1],                             // y_min
			gl_xy_min[0] + i_bounds[2] - i_bounds[0], // x_min + delta_x
			gl_xy_min[1] + i_bounds[3] - i_bounds[1]  // y_min + delta_y
		});
	}
	void Button::SetGlPosition(float xmin, float ymin, float xmax, float ymax)
	{// Sets the xmin/ymin = bottom/left and xmax/ymax = top/right point to the given coordinates
		SetGlPosition(std::array<float, 4>{
			xmin, // x_min
			ymin, // y_min
			xmax, // x_max
			ymax  // y_max
		});
		
	}
	void Button::SetGlPosition(std::array<float, 2>& gl_xy_min, std::array<float, 2>& gl_xy_max)
	{// Sets the xmin/ymin = bottom/left and xmax/ymax = top/right point to the given coordinates
		SetGlPosition(std::array<float, 4>{
			gl_xy_min[0], // x_min
			gl_xy_min[1], // y_min
			gl_xy_max[0], // x_max
			gl_xy_max[1]  // y_max
		});
	}
	void Button::SetGlPosition(std::array<float, 4>& gl_xy_min_xy_max)
	{
		ZAP_REQUIRE(gl_xy_min_xy_max[0] < 1.f && gl_xy_min_xy_max[1] < 1.f && "ZY min must not go outside upper right GL corner");
		ZAP_REQUIRE(gl_xy_min_xy_max[2] > -1.f && gl_xy_min_xy_max[3] > -1.f && "ZY max must not go outside lower left GL corner");

		i_bounds = gl_xy_min_xy_max;
		UpdatePosition();
	}

	void Button::UseText(bool state)
	{
		if (i_font_missing_flag)
		{
			messages::PrintMessage("Cannot use text because the text has got no font path", "Button.cpp/ void zap::Button::UseText(...)", MessageTypes::error);

			return;
		}

		i_use_text = state;
	}

	void Button::SetButtonText(const std::string text)
	{
		i_button_text->SetContent(text);
	}

	void Button::SetTextOffset(float x_offset, float y_offset)
	{
		i_text_offset = { x_offset, y_offset };
		i_use_text_offset = true;
	}

	void Button::SetTextColor(zap::TextColors color)
	{
		i_button_text->SetColor(color);
	}

	void Button::SetTextColor(float RED, float GREEN, float BLUE) 
	{
		// Vars not clamped because the text class does it
		i_button_text->SetColor(RED, GREEN, BLUE);
	};


	void Button::Update()
	{
		//auto position = i_text_offset;
		std::array n_bounds = { i_bounds[0], i_bounds[1] }; // using auto here because the util function returns a template dependent array

		if (i_use_text_offset)
		{
			//n_bounds[0] += position[0];
			//n_bounds[1] += position[1];

			auto new_pos = zap::util::gl_coords_to_pixel(e_window->GetOriginalWindowResolution(), i_text_offset[0] + n_bounds[0], i_text_offset[1] + n_bounds[1]);

			i_button_text->SetPosition(new_pos[0], new_pos[1]);
		}
		else 
		{
			i_button_text->SetPosition(n_bounds[0], n_bounds[1]);
		}
	}

	void Button::Draw()
	{
		Update();

		UseProgram();
		Bind();
		zap::Mesh::Draw();
	
		if(i_use_text) i_button_text->Draw();
	}
}