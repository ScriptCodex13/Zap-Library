#include "enabler.h"
#ifdef SAMPLE_MAIN_UI_BUTTON_CPP

// Just a example
// PR = Prototyping -> only for testing 
#include <Zap.h>
#include <iostream>
#include <array>
#include <Window/Window.h>

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Window settings

	window.UpdateViewport(true);
	window.SetVSync(true);
	zap::Enable(zap::Instruction::DEPTH);
	zap::Enable(zap::Instruction::ANTIALIASING);


	zap::Button button(std::array<float, 4> {-0.5, 0.6, 0.2, 0.8 }, window.GetSize(), window.GetOriginalSize(),
		"Button", "C:/Windows/Fonts/arial.ttf");

	button.SetTextOffset(0.31f, 0.075f);
	button.SetTextColor(zap::TextColors::RED);

	button.LoadTexture(0, "./textures/Button_Texture.png", zap::TextureFilter::LINEAR, 1);
	button.UseTextureShaders(true);
	button.ActivateTexture(true);

	button.FinishMesh();
	//button.GetTextObject()->SetTextOffset(0.31f, 0.075f);
	//button.GetTextObject()->SetTextColor(zap::TextColors::PURPLE);
	//button.GetTextObject()->GenerateCharacters();
	//button.SetGlPosition(std::array<float, 2> {- 0.9f, 0.7f});
	//button.SetGlPosition(-0.9f, 0.7f, 0.2, 0.8 );
	//button.SetGlSize(std::array<float, 2> {0.1f, 0.1f});
	//button.SetGlWidth(0.7);
	//button.SetGlHeight(0.28);
	//button.SetGlHeight(0.10);
	button.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);

	//subscribe to button event provider
	window.AddButtonEventHandler(button.GetUIListener());
	while (window.Open())
	{
		//
		cbi(window);
		zap::ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT);
		zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);
		window.InvokeHandlers();
		
		button.Draw();

		window.Update();
		window.Draw();
	}

	return 0;

}


#endif