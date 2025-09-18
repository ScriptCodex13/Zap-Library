#include "Button.h"

namespace zap
{
	Button::Button(zap::Window& window, const std::string button_text, const std::string button_font_path)
	{
		e_window = &window;
	}

	Button::~Button()
	{

	}
}