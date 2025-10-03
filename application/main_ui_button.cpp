#include "enabler.h"
#ifdef ZAP_LIBRARY_MAIN_UI_BUTTON_CPP

// Just a example
// PR = Prototyping -> only for testing 
#include <Zap.h>
#include <iostream>
#include <array>


template <typename T> class window_invoker : public zap::util::callback_invoker<T>
{
public:
	inline window_invoker(T _callback) : callback_invoker(_callback) {}
	template<typename A1> void operator () (A1& a1) { callback(a1); }
};
window_invoker  cbi([](zap::Window& window) {
	if (window.isKeyPressed(zap::Key::ESC))
	{
		window.Close();
	}
});

int main()
{
	//ZAP_DISABLE_OUTPUTS(true);

	zap::Init(4, 6);

	zap::Window window(1920, 1080, "Hello Window");
	zap::util::scope_guard zapDeleter (zap::Delete);

	zap::InitGlad();


	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	std::array<int, 2> size = window.GetSize(); // Not a Ref to the window size !


	// Window settings

	window.UpdateViewport(true);
	window.SetVSync(true);
	zap::Enable(zap::Instructions::DEPTH);
	zap::Enable(zap::Instructions::ANTIALIASING);

	glm::vec3 lightPos(1.0f, 0.0f, 2.0f);

	float rotation = 0.0f, sensitivity = 0.1f;

	std::array<double, 2> oldPos = window.GetMousePosition();

	//auto coord = zap::util::pixel_to_gl_coords(window.GetSize(), 1.0f, 1.0f);
	//auto coord_2 = zap::util::gl_coords_to_pixel(window.GetSize(), 0.9f, 0.0f);

	zap::Button button(window, std::array<float, 4> {-0.5, 0.6, 0.2, 0.8 }, "Button", "C:/Windows/Fonts/arial.ttf");
	button.SetTextOffset(0.31f, 0.075f);
	button.SetTextColor(zap::TextColors::PURPLE);



	glDisable(GL_DEPTH_TEST);
	while (window.Open())
	{

		//
		cbi(window);

		zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);

		button.Draw();

		if (button.Pressed(zap::Key::LEFT_MOUSE))
			std::cerr << "Pressed" << std::endl;

		window.Update();
		window.Draw();

		rotation += 1.0f;
	}

	return 0;

}


#endif