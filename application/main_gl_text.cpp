#include "enabler.h"
#ifdef MAIN_GL_TEXT_CPP

// Just a example
// PR = Prototyping -> only for testing 
#include <Zap.h>
#include <Graphics.h>
#include <iostream>
#include <array>
#include <GUI/TextRenderer.h>
#include <Window/Window.h>
#include <os/system_font.h>

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


	// Window settings

	window.UpdateViewport(true);
	window.SetVSync(true);

	zap::Enable(zap::Instruction::DEPTH);
	zap::Enable(zap::Instruction::ANTIALIASING);
	char cFontPath[2048] = "arial.ttf";
	unsigned int ret = -1;
	ret = zap::os::GetSystemFontPath(cFontPath, 2048);

	zap::Text text(cFontPath, "Text", window.GetSize()); // It's better to use GetSize here

	////text.SetCharacterSize(48);
	//text.SetColor(0.0f, 1.0f, 0.0f);
	//text.SetPosition(500.0f, 500.0f); // ToDo: Maybe use gl_coords here
	//
	//text.SetTextureFilter(zap::TextureFilter::LINEAR);
	//text.SetMipmapSettings(zap::MipmapSetting::LINEAR_MIPMAP_LINEAR);
	//
	//text.SetScale(2.0f, 2.0f);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
	while (window.Open())
	{
		//Input
		//All the window/camera magic happens here
		cbi(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);

		//text.SetContent(std::to_string(std::round(window.GetFPS())));
		//text.SetContent("Button");
		text.Draw();

		window.Update();
		window.Draw();

	}

	return 0;

}


#endif