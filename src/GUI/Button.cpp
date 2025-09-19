#include "Button.h"

namespace zap
{
	Button::Button(zap::Window& window, const std::string button_text, const std::string button_font_path)
	{
		e_window = &window;

		i_button_text = std::make_unique<zap::Text>(button_font_path, button_text, e_window->GetSizeRef());
	}

	Button::~Button()
	{

	}

	void Button::Update()
	{

	}

	void Button::Draw()
	{
		i_button_text->Draw();
	}
}